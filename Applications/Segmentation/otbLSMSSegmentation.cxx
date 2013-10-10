/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbExtractROI.h"
#include "otbConnectedComponentMuParserFunctor.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkChangeLabelImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "otbTileImageFilter.h"
#include "itkLabelToRGBImageFilter.h"
#include "itkScalarConnectedComponentImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"
#include "otbImportGeoInformationImageFilter.h"

#include <time.h>
#include <vcl_algorithm.h>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStandardWriterWatcher.h"
#include <itksys/SystemTools.hxx>


namespace otb
{
namespace Wrapper
{
class LSMSSegmentation : public Application
{
public:
  typedef LSMSSegmentation Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);

  itkTypeMacro(MeanShiftSegmentation, otb::Application);


  typedef float ImagePixelType;
  typedef otb::VectorImage<ImagePixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  typedef otb::ImageFileWriter<ImageType> ImageWriterType;
  
  typedef unsigned int LabelImagePixelType;
  typedef otb::Image<LabelImagePixelType, 2> LabelImageType;
  typedef otb::ImageFileReader<LabelImageType> LabelImageReaderType;
  typedef otb::ImageFileWriter<LabelImageType> LabelImageWriterType;  
  
  typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
  typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;
  
  typedef otb::MultiToMonoChannelExtractROI<ImagePixelType,LabelImagePixelType> MultiToMonoChannelExtractROIFilterType;
  typedef otb::ObjectList<MultiToMonoChannelExtractROIFilterType> MultiToMonoChannelExtractROIFilterListType;
  
  typedef otb::Functor::ConnectedComponentMuParserFunctor<ImageType::PixelType>  CCFunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<ImageType, LabelImageType, CCFunctorType, otb::Image<unsigned int> > CCFilterType;
  
  typedef itk::ScalarConnectedComponentImageFilter<LabelImageType, LabelImageType> ScalarCCFilterType;
  
  typedef otb::BandMathImageFilter<LabelImageType> BandMathImageFilterType;
  typedef itk::StatisticsImageFilter<LabelImageType> StatisticsImageFilterType;
  typedef itk::ChangeLabelImageFilter<LabelImageType,LabelImageType> ChangeLabelImageFilterType;
  typedef otb::TileImageFilter<LabelImageType> TileImageFilterType;
  typedef otb::ImportGeoInformationImageFilter<LabelImageType,ImageType> ImportGeoInformationImageFilterType;
  typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;
  
  typedef itk::RGBPixel<unsigned char>    RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>  RGBImageType;
  typedef itk::LabelToRGBImageFilter<LabelImageType, RGBImageType> LabelToRGBFilterType;
  typedef otb::ImageFileWriter< RGBImageType > RGBWriterType;
  
  typedef otb::ConcatenateVectorImageFilter <ImageType,ImageType,ImageType> ConcatenateType;  

private:      
  LabelImageReaderType::Pointer m_FinalReader;
  ImportGeoInformationImageFilterType::Pointer m_ImportGeoInformationFilter;

  std::string CreateFileName(unsigned int row, unsigned int column, std::string label)
  {;
    // TODO: Beware of file extensions with more than 3 chars
    std::string outfname = GetParameterString("out");
    std::string tilesname = outfname.substr(0,outfname.size() - itksys::SystemTools::GetFilenameExtension(outfname.c_str()).size());
    
    std::stringstream tileOut;
    tileOut<<tilesname<<"_"<<row<<"_"<<column<<"_"<<label<<".tif";
    
    std::vector<std::string> joins;
    if(IsParameterEnabled("tmpdir"))
      {
      joins.push_back(GetParameterString("tmpdir"));
      }
    joins.push_back(tileOut.str());
    
    std::string currentFile =itksys::SystemTools::JoinPath(joins);

    return currentFile;
  }
  
  std::string WriteTile(LabelImageType::Pointer img, unsigned int row, unsigned int column, std::string label)
  { 
    std::string currentFile = CreateFileName(row,column,label);
    
    LabelImageWriterType::Pointer imageWriter = LabelImageWriterType::New();
    imageWriter->SetInput(img);
    imageWriter->SetFileName(currentFile);
    imageWriter->Update();    

    return currentFile;
  }

  void RemoveTile(std::string tile)
  {
    // Cleanup
    if(IsParameterEnabled("cleanup"))
      {
        // Try to remove the geom file if existing
      std::string geomfile = tile.substr(0,tile.size() - itksys::SystemTools::GetFilenameExtension(tile.c_str()).size()).append(".geom");

      if(itksys::SystemTools::FileExists(geomfile.c_str()))
        {
        itksys::SystemTools::RemoveFile(geomfile.c_str());
        }
      if(itksys::SystemTools::FileExists(tile.c_str()))
        {
        itksys::SystemTools::RemoveFile(tile.c_str());
        }
      }
  }

  std::string WriteVRTFile(unsigned int nbTilesX,unsigned int nbTilesY, unsigned long tileSizeX,unsigned long tileSizeY, unsigned long imageSizeX,unsigned long imageSizeY)
  {
    ImageType::Pointer imageIn = GetParameterImage("in");

    std::string outfname = GetParameterString("out");
    std::string vrtfname = outfname.substr(0,outfname.size() - itksys::SystemTools::GetFilenameExtension(outfname.c_str()).size()).append(".vrt");

    std::cout<<"Creating vrt: "<<vrtfname<<std::endl;

    std::ofstream ofs(vrtfname.c_str());

    ofs<<"<VRTDataset rasterXSize=\""<<imageSizeX<<"\" rasterYSize=\""<<imageSizeY<<"\">"<<std::endl;
    ofs<<"\t<VRTRasterBand dataType=\"UInt32\" band=\"1\">"<<std::endl;
    ofs<<"\t\t<ColorInterp>Gray</ColorInterp>"<<std::endl;

     for(unsigned int column = 0; column < nbTilesX ; ++column)
      {
      for(unsigned int row = 0; row < nbTilesY ; ++row)
        {
        ofs<<"\t\t<SimpleSource>"<<std::endl;
        ofs<<"\t\t\t<SourceFilename relativeToVRT=\"1\">"<<CreateFileName(row,column,"FINAL")<<"</SourceFilename>"<<std::endl;
        ofs<<"\t\t\t<SourceBand>1</SourceBand>"<<std::endl;
        ofs<<"\t\t\t<SrcRect xOff=\""<<0<<"\" yOff=\""<<0<<"\" xSize=\""<<tileSizeX<<"\" ySize=\""<<tileSizeY<<"\"/>"<<std::endl;
        ofs<<"\t\t\t<DstRect xOff=\""<<column*tileSizeX<<"\" yOff=\""<<row*tileSizeY<<"\" xSize=\""<<tileSizeX<<"\" ySize=\""<<tileSizeY<<"\"/>"<<std::endl;
        ofs<<"\t\t</SimpleSource>"<<std::endl;
        }
      }
     ofs<<"\t</VRTRasterBand>"<<std::endl;
     ofs<<"</VRTDataset>"<<std::endl;

    ofs.close();

    return vrtfname;
  }

  void DoInit()
  {
    SetName("LSMSSegmentation");
    SetDescription("First step of the exact large-scale Mean-Shift segmentation workflow.");

    SetDocName("Exact large-scale Mean-Shift segmentation, step 1");
    SetDocLongDescription("This application performs the first step of the exact large-scale Mean-Shift segmentation workflow. Filtered range image and spatial image should be created with the MeanShiftSmoothing application, with modesearch parameter disabled. If spatial image is not set, the application will only process the range image and spatial radius parameter will not be taken into account. This application will produce a labeled image where neighbor pixels whose range distance is bellow range radius (and optionnaly spatial distance bellow spatial radius) will be grouped together into the same cluster. For large images one can use the nbtilesx and nbtilesy parameters for tile-wise processing, with the guarantees of identical results.");
    SetDocLimitations("This application is part of the Large-Scale Mean-Shift segmentation workflow (LSMS) and may not be suited for any other purpose.");
    SetDocAuthors("David Youssefi");
    SetDocSeeAlso(" ");
    AddDocTag(Tags::Segmentation);
    AddDocTag("LSMS");

    AddParameter(ParameterType_InputImage,  "in",    "Filtered image");
    SetParameterDescription( "in", "The filtered image (cf. Adaptive MeanShift Smoothing application)." );
    AddParameter(ParameterType_InputImage,  "inpos",    "Spatial image");
    SetParameterDescription( "inpos", " The spatial image. Spatial input is the displacement map (output of the Adaptive MeanShift Smoothing application)." );
    MandatoryOff("inpos");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription( "out", "The output image. The output image is the segmentation of the filtered image." );

    AddParameter(ParameterType_Float, "ranger", "Range radius");
    SetParameterDescription("ranger", "Range radius defining the radius (expressed in radiometry unit) in the multi-spectral space.");
    SetDefaultParameterFloat("ranger", 15);
    SetMinimumParameterFloatValue("ranger", 0);
    MandatoryOff("ranger");

    AddParameter(ParameterType_Float, "spatialr", "Spatial radius");
    SetParameterDescription("spatialr", "Spatial radius of the neighborhood.");
    SetDefaultParameterFloat("spatialr", 5);
    SetMinimumParameterFloatValue("spatialr", 0);
    MandatoryOff("spatialr");

    AddParameter(ParameterType_Int, "minsize", "Minimum Region Size");
    SetParameterDescription("minsize", "Minimum Region Size. If, after the segmentation, a region is of size lower than this criterion, the region is deleted.");  
    SetDefaultParameterInt("minsize", 0);
    SetMinimumParameterIntValue("minsize", 0);
    MandatoryOff("minsize");
    
    AddParameter(ParameterType_Int, "nbtilesx", "Number of Tiles (X-axis)");
    SetParameterDescription("nbtilesx", "Number of Tiles along the X-axis.");
    SetDefaultParameterInt("nbtilesx", 10);
    SetMinimumParameterIntValue("nbtilesx", 1);
    MandatoryOff("nbtilesx");

    AddParameter(ParameterType_Int, "nbtilesy", "Number of Tiles (Y-axis)");
    SetParameterDescription("nbtilesy", "Number of Tiles along the Y-axis.");
    SetDefaultParameterInt("nbtilesy", 10);
    SetMinimumParameterIntValue("nbtilesy", 1);
    MandatoryOff("nbtilesy");

    AddParameter(ParameterType_Directory,"tmpdir","Directory where to write temporary files");
    SetParameterDescription("tmpdir","This applications need to write some temporary files for each tile. This parameters allows to choose the path where to write those files. If disabled, the current path will be used.");
    MandatoryOff("tmpdir");
    DisableParameter("tmpdir");

    AddParameter(ParameterType_Empty,"cleanup","Activate temporary files cleaning");
    SetParameterDescription("cleanup","If activated, the application will try to clean all temporary files it created");
    MandatoryOff("cleanup");

    // Doc example parameter settings
    SetDocExampleParameterValue("in","smooth.tif");
    SetDocExampleParameterValue("inpos","position.tif");
    SetDocExampleParameterValue("out","segmentation.tif");
    SetDocExampleParameterValue("ranger","15");
    SetDocExampleParameterValue("spatialr","5");
    SetDocExampleParameterValue("minsize","0");
    SetDocExampleParameterValue("nbtilesx","4");
    SetDocExampleParameterValue("nbtilesy","4");
    
  }

  void DoUpdateParameters()
  {      
  }

  void DoExecute()
  {
    clock_t tic = clock();
  
    const float ranger        = GetParameterFloat("ranger");
    const float spatialr      = GetParameterFloat("spatialr");
    
    unsigned int minRegionSize   = GetParameterInt("minsize");
    
    unsigned int nbTilesX       = GetParameterInt("nbtilesx");
    unsigned int nbTilesY       = GetParameterInt("nbtilesy");


    // Ensure that temporary directory exists if activated:
    if(IsParameterEnabled("tmpdir"))
      {
      otbAppLogINFO(<<"Temporary directory "<<GetParameterString("tmpdir")<<" will be used");
      itksys::SystemTools::MakeDirectory(GetParameterString("tmpdir").c_str());
      }

    //Three steps :
    // 1-Tiles segmentation
    // 2-Tiles relabelling
    // 3-Minimal size region suppression

    ImageType::Pointer spatialIn;

    std::vector<std::string> createdFiles1;
    std::vector<std::string> createdFiles2;
        
    if(HasValue("inpos"))
      {
      spatialIn = GetParameterImage("inpos");
      }

    //Acquisition of the input image dimensions
    ImageType::Pointer imageIn = GetParameterImage("in");
    imageIn->UpdateOutputInformation();

    unsigned long sizeImageX = imageIn->GetLargestPossibleRegion().GetSize()[0];
    unsigned long sizeImageY = imageIn->GetLargestPossibleRegion().GetSize()[1];
    unsigned int nbComp      = imageIn->GetNumberOfComponentsPerPixel();

    unsigned long sizeTilesX = (sizeImageX+nbTilesX-1)/nbTilesX, sizeTilesY = (sizeImageY+nbTilesY-1)/nbTilesY;  
    unsigned long regionCount = 0;
  
    //Segmentation by the connected component per tile and label
    //shifting
    otbAppLogINFO(<<"Tile shifting ...");
      
    for(unsigned int row = 0; row < nbTilesY ; ++row)
      for(unsigned int column = 0; column < nbTilesX ; ++column)
        {		
        // Compute extraction parameters
        unsigned long startX = column*sizeTilesX;
        unsigned long startY = row*sizeTilesY;
        unsigned long sizeX = vcl_min(sizeTilesX+1,sizeImageX-startX+1);
        unsigned long sizeY = vcl_min(sizeTilesY+1,sizeImageY-startY+1);
	    
        //Tiles extraction of :
        //- the input image (filtering image)
        MultiChannelExtractROIFilterType::Pointer extractROIFilter = MultiChannelExtractROIFilterType::New();
        extractROIFilter->SetInput(imageIn);
        extractROIFilter->SetStartX(startX);
        extractROIFilter->SetStartY(startY);
        extractROIFilter->SetSizeX(sizeX);
        extractROIFilter->SetSizeY(sizeY);
        extractROIFilter->Update();

        //- the spatial image (final positions) if available
        MultiChannelExtractROIFilterType::Pointer extractROIFilter2 = MultiChannelExtractROIFilterType::New();
        ConcatenateType::Pointer concat = ConcatenateType::New();
        CCFilterType::Pointer ccFilter = CCFilterType::New();

        if(HasValue("inpos"))
          {
          extractROIFilter2->SetInput(spatialIn);
          extractROIFilter2->SetStartX(startX);
          extractROIFilter2->SetStartY(startY);
          extractROIFilter2->SetSizeX(sizeX);
          extractROIFilter2->SetSizeY(sizeY);
          extractROIFilter2->Update();
	  
          //Concatenation of the two input images
          concat->SetInput1(extractROIFilter->GetOutput());
          concat->SetInput2(extractROIFilter2->GetOutput());
          concat->Update();
          
          ccFilter->SetInput(concat->GetOutput());
          }
        else
          {
	  ccFilter->SetInput(extractROIFilter->GetOutput());
          }
        
        //Expression 1 : radiometric distance < ranger
        std::stringstream expr;
        expr<<"sqrt((p1b1-p2b1)*(p1b1-p2b1)";
        for(unsigned int i=1;i<nbComp;i++)
          expr<<"+(p1b"<<i+1<<"-p2b"<<i+1<<")*(p1b"<<i+1<<"-p2b"<<i+1<<")";
        expr<<")"<<"<"<<ranger;

        if(HasValue("inpos"))
          {
          //Expression 2 : final positions < spatialr
          expr<<" and sqrt((p1b"<<nbComp+1<<"-p2b"<<nbComp+1<<")*(p1b"<<nbComp+1<<"-p2b"<<nbComp+1<<")+";
          expr<<"(p1b"<<nbComp+2<<"-p2b"<<nbComp+2<<")*(p1b"<<nbComp+2<<"-p2b"<<nbComp+2<<"))"<<"<"<<spatialr; 
          }
        
        //Segmentation
        ccFilter->GetFunctor().SetExpression(expr.str());
        ccFilter->Update();	    
    
        std::stringstream ssexpr;
        ssexpr<<"label+"<<regionCount;
	    
        //Shifting
        BandMathImageFilterType::Pointer labelBandMath = BandMathImageFilterType::New();
        labelBandMath->SetNthInput(0,ccFilter->GetOutput(),"label");
        labelBandMath->SetExpression(ssexpr.str());
        labelBandMath->Update();

        //Maximum label calculation for the shifting
        StatisticsImageFilterType::Pointer stats = StatisticsImageFilterType::New();
        stats->SetInput(ccFilter->GetOutput());	
        stats->Update();
        regionCount+=stats->GetMaximum();

        std::string filename = WriteTile(labelBandMath->GetOutput(),row,column,"SEG");

        createdFiles1.push_back(filename);
        }
    

    // Step 2: create the look-up table for all overlaps
    otbAppLogINFO(<<"LUT creation ...");
    std::vector<LabelImagePixelType> LUT; 
    LUT.clear();
    LUT.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      LUT[curLabel] = curLabel;
      
    for(unsigned int row = 0; row < nbTilesY ; row++)
      {
      for(unsigned int column = 0; column < nbTilesX ; column++)
        {
        unsigned long startX = column*sizeTilesX;
        unsigned long startY = row*sizeTilesY;
        unsigned long sizeX = vcl_min(sizeTilesX+1,sizeImageX-startX+1);
        unsigned long sizeY = vcl_min(sizeTilesY+1,sizeImageY-startY+1);
      
        std::string tileIn = CreateFileName(row,column,"SEG");

        // Read current tile
        LabelImageReaderType::Pointer tileInReader = LabelImageReaderType::New();
        tileInReader->SetFileName(tileIn);
        tileInReader->Update();

        // Analyse intersection between in and up tiles
        if(row>0)
          {
          std::string tileUp = CreateFileName(row-1,column,"SEG");
	  LabelImageReaderType::Pointer tileUpReader = LabelImageReaderType::New();
	  tileUpReader->SetFileName(tileUp);
          tileUpReader->Update();

          LabelImageType::IndexType pixelIndexIn;
          LabelImageType::IndexType pixelIndexUp;

          pixelIndexIn[1] = 0;
          pixelIndexUp[1] = sizeTilesY;

          for(pixelIndexIn[0]=0;pixelIndexIn[0]<static_cast<long>(sizeX-1);++pixelIndexIn[0])
            {
            pixelIndexUp[0] = pixelIndexIn[0];

            LabelImagePixelType curCanLabel = tileInReader->GetOutput()->GetPixel(pixelIndexIn);
	    while(LUT[curCanLabel] != curCanLabel) 
              {
              curCanLabel = LUT[curCanLabel];
              }
	    LabelImagePixelType adjCanLabel = tileUpReader->GetOutput()->GetPixel(pixelIndexUp);
            
	    while(LUT[adjCanLabel] != adjCanLabel)
              {
              adjCanLabel = LUT[adjCanLabel];
              }
	    if(curCanLabel < adjCanLabel) 
              {
              LUT[adjCanLabel] = curCanLabel;
              }
	    else
              {
              LUT[LUT[curCanLabel]] = adjCanLabel; LUT[curCanLabel] = adjCanLabel;
              }
            }
          }

        // Analyse intersection between in and left tiles
         if(column>0)
          {
          std::string tileLeft = CreateFileName(row,column-1,"SEG");
	  LabelImageReaderType::Pointer tileLeftReader = LabelImageReaderType::New();
	  tileLeftReader->SetFileName(tileLeft);
          tileLeftReader->Update();

          LabelImageType::IndexType pixelIndexIn;
          LabelImageType::IndexType pixelIndexUp;

          pixelIndexIn[0] = 0;
          pixelIndexUp[0] = sizeTilesX;

          for(pixelIndexIn[1]=0;pixelIndexIn[1]<static_cast<long>(sizeY-1);++pixelIndexIn[1])
            {
            pixelIndexUp[1] = pixelIndexIn[1];

            LabelImagePixelType curCanLabel = tileInReader->GetOutput()->GetPixel(pixelIndexIn);
	    while(LUT[curCanLabel] != curCanLabel) 
              {
              curCanLabel = LUT[curCanLabel];
              }
	    LabelImagePixelType adjCanLabel = tileLeftReader->GetOutput()->GetPixel(pixelIndexUp);
	    while(LUT[adjCanLabel] != adjCanLabel)
              { 
              adjCanLabel = LUT[adjCanLabel];
              }
	    if(curCanLabel < adjCanLabel) 
              {
              LUT[adjCanLabel] = curCanLabel;
              }
	    else
              {
              LUT[LUT[curCanLabel]] = adjCanLabel; LUT[curCanLabel] = adjCanLabel;
              }
            }
          }
        }
      }
    
    // Reduce LUT to canonical labels
    for(LabelImagePixelType label = 1; label < regionCount+1; ++label)
      {             
      LabelImagePixelType can = label;
      while(LUT[can] != can) 
        {
        can = LUT[can];
        }
      LUT[label] = can;
      }
    otbAppLogINFO(<<"LUT size: "<<LUT.size());
 
    // These variables will be used to estimate the size of each
    // region on the flow
    std::vector<unsigned long> sizePerRegion(regionCount+1,0);

    otbAppLogINFO(<<"Tiles relabelisation");
    for(unsigned int column = 0; column < nbTilesX ; ++column)
      {
      for(unsigned int row = 0; row < nbTilesY ; ++row)
        {
	unsigned long startX = column*sizeTilesX;
        unsigned long startY = row*sizeTilesY;
	unsigned long sizeX = vcl_min(sizeTilesX,sizeImageX-startX);
        unsigned long sizeY = vcl_min(sizeTilesY,sizeImageY-startY);

        std::string tileIn = CreateFileName(row,column,"SEG");

        LabelImageReaderType::Pointer readerIn = LabelImageReaderType::New();
        readerIn->SetFileName(tileIn);

        // Remove extra margin now that lut is built
        ExtractROIFilterType::Pointer labelImage = ExtractROIFilterType::New();
	labelImage->SetInput(readerIn->GetOutput());
	labelImage->SetStartX(0);
	labelImage->SetStartY(0);
	labelImage->SetSizeX(sizeX);
	labelImage->SetSizeY(sizeY);
        
        // Relabel tile according to look-up table
        ChangeLabelImageFilterType::Pointer changeLabel = ChangeLabelImageFilterType::New();
	changeLabel->SetInput(labelImage->GetOutput());
	
        // Fill LUT
        for(LabelImagePixelType label = 1;label<regionCount+1; ++label)
          {
	  if(label!=LUT[label]) 
            {
            changeLabel->SetChange(label,LUT[label]);
            }
          }

        // Here we need to update the filter to be able to update
        // region sizes table
        changeLabel->Update();

        // Update region sizes
        LabelImageIterator it( changeLabel->GetOutput(), changeLabel->GetOutput()->GetLargestPossibleRegion());
	for (it = it.Begin(); !it.IsAtEnd(); ++it) 
          {
          sizePerRegion[it.Value()]+=1;
          }

        // Write tile
        WriteTile(changeLabel->GetOutput(),row,column,"RELAB");

        // Remove previous tile (not needed anymore)
        RemoveTile(tileIn);
        }
      }

    // Clear lut, we do not need it anymore
    LUT.clear();

      // Create the LUT to filter small regions and assign min labels
      otbAppLogINFO(<<"Small regions pruning ...");
      LabelImagePixelType newLab=1;
      std::vector<LabelImagePixelType> newLabels(regionCount+1,0);
      for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
        {
        if(sizePerRegion[curLabel]<minRegionSize) 
          {
          newLabels[curLabel]=0;
          }
        else
          {
          newLabels[curLabel]=newLab;
          newLab+=1;
          }
        }
      otbAppLogINFO(<<"LUT size: "<<newLabels.size());


      // Clear sizePerRegion, we do not need it anymore
      sizePerRegion.clear();
     
      for(unsigned int column = 0; column < nbTilesX ; ++column)
        {
        for(unsigned int row = 0; row < nbTilesY ; ++row)
          {
          std::string tileIn = CreateFileName(row,column,"RELAB");

          LabelImageReaderType::Pointer readerIn = LabelImageReaderType::New();
          readerIn->SetFileName(tileIn);

          ChangeLabelImageFilterType::Pointer changeLabel = ChangeLabelImageFilterType::New();
          changeLabel->SetInput(readerIn->GetOutput());
          for(LabelImagePixelType label = 1;label<regionCount+1; ++label)
            {
            if(label != newLabels[label])
              {
              changeLabel->SetChange(label,newLabels[label]);
              }
            }
          
          // Write the relabeled tile
          WriteTile(readerIn->GetOutput(),row,column,"FINAL");
          
          // Clean previous tiles (not needed anymore)
          RemoveTile(tileIn);
          }
        }

      // Clear newLabels, we do not need it anymore
      newLabels.clear();
  
      // Here we write a temporary vrt file that will be used to
      // stitch together all the tiles
      std::string vrtfile = WriteVRTFile(nbTilesY,nbTilesY,sizeTilesX,sizeTilesY,sizeImageX,sizeImageY);

      clock_t toc = clock();
      
      otbAppLogINFO(<<"Elapsed time: "<<(double)(toc - tic) / CLOCKS_PER_SEC<<" seconds");

      // Final writing
      m_FinalReader = LabelImageReaderType::New();
      m_FinalReader->SetFileName(vrtfile);

      m_ImportGeoInformationFilter = ImportGeoInformationImageFilterType::New();
      m_ImportGeoInformationFilter->SetInput(m_FinalReader->GetOutput());
      m_ImportGeoInformationFilter->SetSource(imageIn);

      SetParameterOutputImage("out",m_ImportGeoInformationFilter->GetOutput());
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSSegmentation)


