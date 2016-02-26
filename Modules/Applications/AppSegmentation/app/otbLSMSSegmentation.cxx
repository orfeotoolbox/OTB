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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbExtractROI.h"
#include "otbConnectedComponentMuParserFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkChangeLabelImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkScalarConnectedComponentImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbAffineFunctor.h"

#include "otbMultiToMonoChannelExtractROI.h"
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

  itkTypeMacro(LSMSSegmentation, otb::Application);

  typedef FloatVectorImageType              ImageType;
  typedef ImageType::InternalPixelType      ImagePixelType;
  typedef UInt32ImageType                   LabelImageType;
  typedef LabelImageType::InternalPixelType LabelImagePixelType;
  typedef otb::ImageFileReader<ImageType> ImageReaderType;
  typedef otb::ImageFileWriter<ImageType> ImageWriterType;
  typedef otb::ImageFileReader<LabelImageType> LabelImageReaderType;
  typedef otb::ImageFileWriter<LabelImageType> LabelImageWriterType;
  typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
  typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;
  typedef otb::MultiToMonoChannelExtractROI<ImagePixelType,LabelImagePixelType> MultiToMonoChannelExtractROIFilterType;
  typedef otb::Functor::ConnectedComponentMuParserFunctor<ImageType::PixelType>  CCFunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<ImageType, LabelImageType, CCFunctorType, otb::Image<unsigned int> > CCFilterType;
  typedef itk::ScalarConnectedComponentImageFilter<LabelImageType, LabelImageType> ScalarCCFilterType;
  typedef itk::StatisticsImageFilter<LabelImageType> StatisticsImageFilterType;
  typedef itk::ChangeLabelImageFilter<LabelImageType,LabelImageType> ChangeLabelImageFilterType;
  typedef otb::ImportGeoInformationImageFilter<LabelImageType,ImageType> ImportGeoInformationImageFilterType;
  typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;

  typedef otb::ConcatenateVectorImageFilter <ImageType,ImageType,ImageType> ConcatenateType;
  typedef otb::Functor::AffineFunctor<
    LabelImagePixelType,
    LabelImagePixelType,
    LabelImagePixelType>                      AffineFunctorType;
  typedef itk::UnaryFunctorImageFilter<
    LabelImageType,
    LabelImageType,
    AffineFunctorType>                        LabelShiftFilterType;

  LSMSSegmentation(): m_FinalReader(),m_ImportGeoInformationFilter(),m_FilesToRemoveAfterExecute(),m_TmpDirCleanup(false){}

  virtual ~LSMSSegmentation(){}

private:
  LabelImageReaderType::Pointer m_FinalReader;
  ImportGeoInformationImageFilterType::Pointer m_ImportGeoInformationFilter;
  std::vector<std::string> m_FilesToRemoveAfterExecute;
  bool m_TmpDirCleanup;

  std::string CreateFileName(unsigned int row, unsigned int column, std::string label)
  {
    std::string outfname = GetParameterString("out");
    std::string tilesname = itksys::SystemTools::GetFilenameWithoutExtension(outfname.c_str());

    std::stringstream tileOut;
    tileOut<<tilesname<<"_"<<row<<"_"<<column<<"_"<<label<<".tif";

    std::vector<std::string> joins;
    if(IsParameterEnabled("tmpdir"))
      {
      std::string tmpdir = GetParameterString("tmpdir");

      if(tmpdir.size() > 1 && tmpdir[tmpdir.size()-1] != '/')
        {
        tmpdir.append("/");
        }
      joins.push_back(tmpdir);
      }
    joins.push_back(tileOut.str());

    std::string currentFile = itksys::SystemTools::JoinPath(joins);

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

  void RemoveFile(std::string tile)
  {
    // Cleanup
    if(IsParameterEnabled("cleanup"))
      {
        // Try to remove the geom file if existing
      std::string geomfile = tile.substr(0,tile.size() - itksys::SystemTools::GetFilenameExtension(tile.c_str()).size()).append(".geom");

      if(itksys::SystemTools::FileExists(geomfile.c_str()))
        {
        bool res = itksys::SystemTools::RemoveFile(geomfile.c_str());
        if (!res)
          {
          otbAppLogINFO(<<"Unable to remove file  "<<geomfile);
          }
        }
      if(itksys::SystemTools::FileExists(tile.c_str()))
        {
        bool res = itksys::SystemTools::RemoveFile(tile.c_str());
        if (!res)
          {
          otbAppLogINFO(<<"Unable to remove file  "<<tile);
          }
        }
      }
  }

  std::string WriteVRTFile(unsigned int nbTilesX,unsigned int nbTilesY, unsigned long tileSizeX,unsigned long tileSizeY, unsigned long imageSizeX,unsigned long imageSizeY)
  {
    ImageType::Pointer imageIn = GetParameterImage("in");

    std::string outfname = GetParameterString("out");

    std::stringstream vrtfOut;
    vrtfOut<<itksys::SystemTools::GetFilenameWithoutExtension(outfname.c_str())<<".vrt";

    std::vector<std::string> joins;
    if(IsParameterEnabled("tmpdir"))
      {
      std::string tmpdir = GetParameterString("tmpdir");

      if(tmpdir.size() > 1 && tmpdir[tmpdir.size()-1] != '/')
        {
        tmpdir.append("/");
        }
      joins.push_back(tmpdir);
      }
    joins.push_back(vrtfOut.str());

    std::string vrtfname = itksys::SystemTools::JoinPath(joins);
    otbAppLogINFO(<<"Creating temporary vrt file: "<<vrtfname);

    std::ofstream ofs(vrtfname.c_str());

    ofs<<"<VRTDataset rasterXSize=\""<<imageSizeX<<"\" rasterYSize=\""<<imageSizeY<<"\">"<<std::endl;
    ofs<<"\t<VRTRasterBand dataType=\"UInt32\" band=\"1\">"<<std::endl;
    ofs<<"\t\t<ColorInterp>Gray</ColorInterp>"<<std::endl;

     for(unsigned int column = 0; column < nbTilesX; ++column)
      {
      for(unsigned int row = 0; row < nbTilesY; ++row)
        {
        ofs<<"\t\t<SimpleSource>"<<std::endl;
        ofs<<"\t\t\t<SourceFilename relativeToVRT=\"1\">"<< itksys::SystemTools::GetFilenameName(CreateFileName(row,column,"FINAL")) <<"</SourceFilename>"<<std::endl;
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
    SetDescription("Second step of the exact Large-Scale Mean-Shift segmentation workflow.");

    SetDocName("Exact Large-Scale Mean-Shift segmentation, step 2");
    SetDocLongDescription("This application performs the second step of the exact Large-Scale Mean-Shift segmentation workflow (LSMS). Filtered range image and spatial image should be created with the MeanShiftSmoothing application, with modesearch parameter disabled. If spatial image is not set, the application will only process the range image and spatial radius parameter will not be taken into account. This application will produce a labeled image where neighbor pixels whose range distance is below range radius (and optionally spatial distance below spatial radius) will be grouped together into the same cluster. For large images one can use the nbtilesx and nbtilesy parameters for tile-wise processing, with the guarantees of identical results. Please note that this application will generate a lot of temporary files (as many as the number of tiles), and will therefore require twice the size of the final result in term of disk space. The cleanup option (activated by default) allows removing all temporary file as soon as they are not needed anymore (if cleanup is activated, tmpdir set and tmpdir does not exists before running the application, it will be removed as well during cleanup). The tmpdir option allows defining a directory where to write the temporary files. Please also note that the output image type should be set to uint32 to ensure that there are enough labels available.");
    SetDocLimitations("This application is part of the Large-Scale Mean-Shift segmentation workflow (LSMS) and may not be suited for any other purpose.");
    SetDocAuthors("David Youssefi");
    SetDocSeeAlso("MeanShiftSmoothing, LSMSSmallRegionsMerging, LSMSVectorization");
    AddDocTag(Tags::Segmentation);
    AddDocTag("LSMS");

    AddParameter(ParameterType_InputImage,  "in",    "Filtered image");
    SetParameterDescription( "in", "The filtered image (cf. Adaptive MeanShift Smoothing application)." );
    AddParameter(ParameterType_InputImage,  "inpos",    "Spatial image");
    SetParameterDescription( "inpos", " The spatial image. Spatial input is the displacement map (output of the Adaptive MeanShift Smoothing application)." );
    MandatoryOff("inpos");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription( "out", "The output image. The output image is the segmentation of the filtered image. It is recommended to set the pixel type to uint32." );
    SetDefaultOutputPixelType("out",ImagePixelType_uint32);

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

    AddParameter(ParameterType_Int, "tilesizex", "Size of tiles in pixel (X-axis)");
    SetParameterDescription("tilesizex", "Size of tiles along the X-axis.");
    SetDefaultParameterInt("tilesizex", 500);
    SetMinimumParameterIntValue("tilesizex", 1);

    AddParameter(ParameterType_Int, "tilesizey", "Size of tiles in pixel (Y-axis)");
    SetParameterDescription("tilesizey", "Size of tiles along the Y-axis.");
    SetDefaultParameterInt("tilesizey", 500);
    SetMinimumParameterIntValue("tilesizey", 1);

    AddParameter(ParameterType_Directory,"tmpdir","Directory where to write temporary files");
    SetParameterDescription("tmpdir","This applications need to write temporary files for each tile. This parameter allows choosing the path where to write those files. If disabled, the current path will be used.");
    MandatoryOff("tmpdir");
    DisableParameter("tmpdir");

    AddParameter(ParameterType_Empty,"cleanup","Temporary files cleaning");
    EnableParameter("cleanup");
    SetParameterDescription("cleanup","If activated, the application will try to clean all temporary files it created");
    MandatoryOff("cleanup");

    // Doc example parameter settings
    SetDocExampleParameterValue("in","smooth.tif");
    SetDocExampleParameterValue("inpos","position.tif");
    SetDocExampleParameterValue("out","segmentation.tif");
    SetDocExampleParameterValue("ranger","15");
    SetDocExampleParameterValue("spatialr","5");
    SetDocExampleParameterValue("minsize","0");
    SetDocExampleParameterValue("tilesizex","256");
    SetDocExampleParameterValue("tilesizey","256");

  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    m_FilesToRemoveAfterExecute.clear();

    clock_t tic = clock();

    const float ranger         = GetParameterFloat("ranger");
    const float spatialr       = GetParameterFloat("spatialr");

    unsigned int minRegionSize = GetParameterInt("minsize");

    unsigned long sizeTilesX   = GetParameterInt("tilesizex");
    unsigned long sizeTilesY   = GetParameterInt("tilesizey");


    // Ensure that temporary directory exists if activated:
    if(IsParameterEnabled("tmpdir"))
      {
      if(!itksys::SystemTools::FileExists(GetParameterString("tmpdir").c_str()))
        {
        m_TmpDirCleanup = true;
        }
      otbAppLogINFO(<<"Temporary directory "<<GetParameterString("tmpdir")<<" will be used");
      itksys::SystemTools::MakeDirectory(GetParameterString("tmpdir").c_str());
      }

    //Three steps :
    // 1-Tiles segmentation
    // 2-Tiles relabelling
    // 3-Minimal size region suppression

    ImageType::Pointer spatialIn;

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

    unsigned int nbTilesX = sizeImageX/sizeTilesX + (sizeImageX%sizeTilesX > 0 ? 1 : 0);
    unsigned int nbTilesY = sizeImageY/sizeTilesY + (sizeImageY%sizeTilesY > 0 ? 1 : 0);

    otbAppLogINFO(<<"Number of tiles: "<<nbTilesX<<" x "<<nbTilesY);

    unsigned long regionCount = 0;

    //Segmentation by the connected component per tile and label
    //shifting
    otbAppLogINFO(<<"Tile shifting ...");

    for(unsigned int row = 0; row < nbTilesY; ++row)
      for(unsigned int column = 0; column < nbTilesX; ++column)
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
        for(unsigned int i=1; i<nbComp; i++)
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

        //Shifting
        LabelShiftFilterType::Pointer labelShiftFilter = LabelShiftFilterType::New();
        labelShiftFilter->SetInput(ccFilter->GetOutput());
        labelShiftFilter->GetFunctor().SetA(1);
        labelShiftFilter->GetFunctor().SetB(regionCount);
        labelShiftFilter->Update();

        //Maximum label calculation for the shifting
        StatisticsImageFilterType::Pointer stats = StatisticsImageFilterType::New();
        stats->SetInput(ccFilter->GetOutput());
        stats->Update();
        regionCount+=stats->GetMaximum();

        std::string filename = WriteTile(labelShiftFilter->GetOutput(),row,column,"SEG");
        }


    // Step 2: create the look-up table for all overlaps
    otbAppLogINFO(<<"LUT creation ...");
    std::vector<LabelImagePixelType> LUT;
    LUT.clear();
    LUT.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      LUT[curLabel] = curLabel;

    for(unsigned int row = 0; row < nbTilesY; row++)
      {
      for(unsigned int column = 0; column < nbTilesX; column++)
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

          for(pixelIndexIn[0]=0; pixelIndexIn[0]<static_cast<long>(sizeX-1); ++pixelIndexIn[0])
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

          for(pixelIndexIn[1]=0; pixelIndexIn[1]<static_cast<long>(sizeY-1); ++pixelIndexIn[1])
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
    otbAppLogINFO(<<"LUT size: "<<LUT.size()<<" segments");

    // These variables will be used to estimate the size of each
    // region on the flow
    std::vector<unsigned long> sizePerRegion(regionCount+1,0);

    otbAppLogINFO(<<"Tiles relabelisation ...");
    for(unsigned int column = 0; column < nbTilesX; ++column)
      {
      for(unsigned int row = 0; row < nbTilesY; ++row)
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
        for(LabelImagePixelType label = 1; label<regionCount+1; ++label)
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
        for (it.GoToBegin(); !it.IsAtEnd(); ++it)
          {
          sizePerRegion[it.Value()]+=1;
          }

        // Write tile
        WriteTile(changeLabel->GetOutput(),row,column,"RELAB");

        // Remove previous tile (not needed anymore)
        readerIn = 0; // release the input file
        RemoveFile(tileIn);
        }
      }

    // Clear lut, we do not need it anymore
    LUT.clear();

    unsigned int smallCount = 0;

      // Create the LUT to filter small regions and assign min labels
      otbAppLogINFO(<<"Small regions pruning ...");
      LabelImagePixelType newLab=1;
      std::vector<LabelImagePixelType> newLabels(regionCount+1,0);
      for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
        {
        if(sizePerRegion[curLabel]<minRegionSize)
          {
          newLabels[curLabel]=0;
          ++smallCount;
          }
        else
          {
          newLabels[curLabel]=newLab;
          newLab+=1;
          }
        }

      otbAppLogINFO(<<smallCount<<" small regions will be removed");

      // Clear sizePerRegion, we do not need it anymore
      sizePerRegion.clear();

      for(unsigned int column = 0; column < nbTilesX; ++column)
        {
        for(unsigned int row = 0; row < nbTilesY; ++row)
          {
          std::string tileIn = CreateFileName(row,column,"RELAB");

          LabelImageReaderType::Pointer readerIn = LabelImageReaderType::New();
          readerIn->SetFileName(tileIn);

          ChangeLabelImageFilterType::Pointer changeLabel = ChangeLabelImageFilterType::New();
          changeLabel->SetInput(readerIn->GetOutput());
          for(LabelImagePixelType label = 1; label<regionCount+1; ++label)
            {
            if(label != newLabels[label])
              {
              changeLabel->SetChange(label,newLabels[label]);
              }
            }

          // Write the relabeled tile
          std::string tmpfile = WriteTile(changeLabel->GetOutput(),row,column,"FINAL");
          m_FilesToRemoveAfterExecute.push_back(tmpfile);

          // Clean previous tiles (not needed anymore)
          readerIn = 0; // release the input file
          RemoveFile(tileIn);
          }
        }

      // Clear newLabels, we do not need it anymore
      newLabels.clear();

      // Here we write a temporary vrt file that will be used to
      // stitch together all the tiles
      std::string vrtfile = WriteVRTFile(nbTilesX,nbTilesY,sizeTilesX,sizeTilesY,sizeImageX,sizeImageY);

      m_FilesToRemoveAfterExecute.push_back(vrtfile);

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

  void AfterExecuteAndWriteOutputs()
  {
    // Release input files
    m_FinalReader = 0;

    if(IsParameterEnabled("cleanup"))
      {
      otbAppLogINFO(<<"Final clean-up ...");

      for(std::vector<std::string>::iterator it = m_FilesToRemoveAfterExecute.begin();
          it!=m_FilesToRemoveAfterExecute.end(); ++it)
        {
        RemoveFile(*it);
        }

      if(IsParameterEnabled("tmpdir") && m_TmpDirCleanup)
        {
        otbAppLogINFO(<<"Removing tmp directory "<<GetParameterString("tmpdir")<<", since it has been created by the application");
        itksys::SystemTools::RemoveADirectory(GetParameterString("tmpdir").c_str());
        }
      }

    m_FilesToRemoveAfterExecute.clear();
    m_TmpDirCleanup = false;
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSSegmentation)
