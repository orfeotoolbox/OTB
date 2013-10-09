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
  typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;
  
  typedef itk::RGBPixel<unsigned char>    RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>  RGBImageType;
  typedef itk::LabelToRGBImageFilter<LabelImageType, RGBImageType> LabelToRGBFilterType;
  typedef otb::ImageFileWriter< RGBImageType > RGBWriterType;
  
  typedef otb::ConcatenateVectorImageFilter <ImageType,ImageType,ImageType> ConcatenateType;  

private:
  TileImageFilterType::Pointer tileFusion;
  TileImageFilterType::Pointer tileFusion2;
  ChangeLabelImageFilterType::Pointer changeLabel1;
  ChangeLabelImageFilterType::Pointer changeLabel2;
  
  std::vector<LabelImageReaderType::Pointer> readers;



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
  
    std::string tilesname = GetParameterString("out");
    tilesname = tilesname.substr(0,tilesname.size()-4);

    const float ranger        = GetParameterFloat("ranger");
    const float spatialr      = GetParameterFloat("spatialr");
    
    unsigned int minRegionSize   = GetParameterInt("minsize");
    
    unsigned int nbTilesX       = GetParameterInt("nbtilesx");
    unsigned int nbTilesY       = GetParameterInt("nbtilesy");

    unsigned int step = 0;

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

    std::vector<std::string> createdFiles;
        
    // TODO: Check this
    if(!HasValue("inpos"))
      {
	if(HasValue("in"))
	  {
	    typedef float ImagePixelType;
	    typedef otb::VectorImage<ImagePixelType, 2> ImageType;
	    typedef otb::ImageFileReader<ImageType> ImageReaderType;
	    typedef otb::ImageFileWriter<ImageType> ImageWriterType;
	    
	    FloatVectorImageType* input = GetParameterImage("in");
	    unsigned long sizeImageX = input->GetLargestPossibleRegion().GetSize()[0],
	    sizeImageY = input->GetLargestPossibleRegion().GetSize()[1];
	    SetParameterInt("spatialr",sqrt(sizeImageX*sizeImageX+sizeImageY*sizeImageY)+1);
	    spatialIn = GetParameterImage("in");
	  }
      }
    else spatialIn = GetParameterImage("inpos");      

    //Acquisition of the input image dimensions
    ImageType::Pointer imageIn = GetParameterImage("in");
    imageIn->UpdateOutputInformation();
    unsigned long sizeImageX = imageIn->GetLargestPossibleRegion().GetSize()[0],
      sizeImageY = imageIn->GetLargestPossibleRegion().GetSize()[1];
    unsigned int nbComp = imageIn->GetNumberOfComponentsPerPixel();

    long nbStars = 50;


    unsigned long sizeTilesX = (sizeImageX+nbTilesX-1)/nbTilesX, sizeTilesY = (sizeImageY+nbTilesY-1)/nbTilesY;  
    unsigned long regionCount = 0;
  
    tileFusion = TileImageFilterType::New();
    tileFusion2 = TileImageFilterType::New();
    changeLabel1 = ChangeLabelImageFilterType::New();
    changeLabel2 = ChangeLabelImageFilterType::New();

    if((step==0)||(step==1))
      {
	//Segmentation by the connected component per tile and label
	//shifting
      otbAppLogINFO(<<"Tile shifting ...");
      
	LabelImageType::SizeType layout; layout[0] = nbTilesX; layout[1] = nbTilesY;

	tileFusion->SetLayout(layout);
	unsigned int nbTile=0;
      
	LabelImageWriterType::Pointer ImageWriter = LabelImageWriterType::New();

      
	for(unsigned int row = 0; row < nbTilesY ; row++)
	  for(unsigned int column = 0; column < nbTilesX ; column++)
	    {		
	      unsigned long startX = column*sizeTilesX, startY = row*sizeTilesY;
	      unsigned long sizeX = vcl_min(sizeTilesX+1,sizeImageX-startX+1),sizeY = vcl_min(sizeTilesY+1,sizeImageY-startY+1);
	    
	      //Tiles extraction of :
	      //- the input image (filtering image)
	      MultiChannelExtractROIFilterType::Pointer extractROIFilter = MultiChannelExtractROIFilterType::New();
	      extractROIFilter->SetInput(imageIn);
	      extractROIFilter->SetStartX(startX);
	      extractROIFilter->SetStartY(startY);
	      extractROIFilter->SetSizeX(sizeX);
	      extractROIFilter->SetSizeY(sizeY);
	      extractROIFilter->Update();

	      //- the spatial image (final positions)
	      MultiChannelExtractROIFilterType::Pointer extractROIFilter2 = MultiChannelExtractROIFilterType::New();
	      extractROIFilter2->SetInput(spatialIn);
	      extractROIFilter2->SetStartX(startX);
	      extractROIFilter2->SetStartY(startY);
	      extractROIFilter2->SetSizeX(sizeX);
	      extractROIFilter2->SetSizeY(sizeY);
	      extractROIFilter2->Update();
	    
	      //Concatenation of the two input images
	      ConcatenateType::Pointer concat = ConcatenateType::New();
	      concat->SetInput1(extractROIFilter->GetOutput());
	      concat->SetInput2(extractROIFilter2->GetOutput());
	      concat->Update();
	    
	      //Expression 1 : radiometric distance < ranger
	      std::stringstream expr;
	      expr<<"sqrt((p1b1-p2b1)*(p1b1-p2b1)";
	      for(unsigned int i=1;i<nbComp;i++)
		expr<<"+(p1b"<<i+1<<"-p2b"<<i+1<<")*(p1b"<<i+1<<"-p2b"<<i+1<<")";
	      expr<<")"<<"<"<<ranger<<" and sqrt(";

	      //Expression 2 : final positions < spatialr
	      expr<<"(p1b"<<nbComp+1<<"-p2b"<<nbComp+1<<")*(p1b"<<nbComp+1<<"-p2b"<<nbComp+1<<")+";
	      expr<<"(p1b"<<nbComp+2<<"-p2b"<<nbComp+2<<")*(p1b"<<nbComp+2<<"-p2b"<<nbComp+2<<"))"<<"<"<<spatialr;
	    
	      //Segmentation
	      CCFilterType::Pointer ccFilter = CCFilterType::New();
	      ccFilter->SetInput(concat->GetOutput());
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

              std::stringstream tileOut;
              tileOut<<tilesname<<"_"<<row<<"_"<<column<<"_SEG.tif";

              std::vector<std::string> joins;
              if(IsParameterEnabled("tmpdir"))
                {
                joins.push_back(GetParameterString("tmpdir"));
                }
              joins.push_back(tileOut.str());
              
              std::string currentFile =itksys::SystemTools::JoinPath(joins);
              createdFiles.push_back(currentFile);
              
              ImageWriter->SetInput(labelBandMath->GetOutput());
              ImageWriter->SetFileName(currentFile);
              ImageWriter->Update();

              LabelImageReaderType::Pointer currentReader = LabelImageReaderType::New();
              currentReader->SetFileName(currentFile);
              
              readers.push_back(currentReader);
              
	      tileFusion->SetInput(nbTile++,currentReader->GetOutput());
	    }
      
	// std::stringstream seg1;
	// seg1<<tilesname<<"_tmp1.tif";

        // std::vector<std::string> joins;
        // if(IsParameterEnabled("tmpdir"))
        //   {
        //   joins.push_back(GetParameterString("tmpdir"));
        //   }
        // joins.push_back(seg1.str());

        // std::string currentFile =itksys::SystemTools::JoinPath(joins);
        
        // createdFiles.push_back(currentFile);
            
	// //Writing of the tiles
	// ImageWriter->SetInput(tileFusion->GetOutput());
	// ImageWriter->SetFileName(currentFile);
	// ImageWriter->Update();

        // readers.clear();
      }

    if((step==0)||(step==2))
      {
	LabelImageType::SizeType layout; layout[0] = nbTilesX; layout[1] = nbTilesY;
	tileFusion2->SetLayout(layout);
	unsigned int nbTile=0;
      
	LabelImageWriterType::Pointer ImageWriter = LabelImageWriterType::New();

	// std::stringstream seg1;
	// seg1<<tilesname<<"_tmp1.tif";
      
	// LabelImageReaderType::Pointer seg1Reader = LabelImageReaderType::New();
	// seg1Reader->SetFileName(seg1.str());
	// seg1Reader->UpdateOutputInformation();
	unsigned long seg1X = tileFusion->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
	  seg1Y = tileFusion->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

	StatisticsImageFilterType::Pointer stats = StatisticsImageFilterType::New();
	stats->SetInput(tileFusion->GetOutput());	
	stats->Update();
	regionCount = stats->GetMaximum();

	std::vector<LabelImagePixelType> LUT; 
	LUT.clear();
	LUT.resize(regionCount+1);
	for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
	  LUT[curLabel] = curLabel;
      
        otbAppLogINFO(<<"LUT creation ...");

	for(unsigned int row = 1; row < nbTilesY ; row++)
	  {
	    unsigned long startY = row*(sizeTilesY+1);
	  
	    ExtractROIFilterType::Pointer extractROI = ExtractROIFilterType::New();
	    extractROI->SetInput(tileFusion->GetOutput());
	    extractROI->SetStartX(0);
	    extractROI->SetStartY(startY-1);
	    extractROI->SetSizeX(seg1X);
	    extractROI->SetSizeY(2);
	    extractROI->Update();

	    unsigned long e1X = extractROI->GetOutput()->GetLargestPossibleRegion().GetSize()[0],
	      e1Y = extractROI->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
      
	    //Equivalence between current tile and the tile of the top
	    LabelImageType::IndexType pixelIndex1;
	    LabelImageType::IndexType pixelIndex2;
	  
            pixelIndex1[1]=0;
            pixelIndex2[1]=1;

	    for(pixelIndex1[0]=0;pixelIndex1[0]<static_cast<long>(seg1X);pixelIndex1[0]++){
	      pixelIndex2[0] = pixelIndex1[0];
	      LabelImagePixelType curCanLabel = extractROI->GetOutput()->GetPixel(pixelIndex1);
	      while(LUT[curCanLabel] != curCanLabel) curCanLabel = LUT[curCanLabel];
	      LabelImagePixelType adjCanLabel = extractROI->GetOutput()->GetPixel(pixelIndex2);
	      while(LUT[adjCanLabel] != adjCanLabel) adjCanLabel = LUT[adjCanLabel];
	      if(curCanLabel < adjCanLabel) LUT[adjCanLabel] = curCanLabel;
	      else{LUT[LUT[curCanLabel]] = adjCanLabel; LUT[curCanLabel] = adjCanLabel;}
	    }
	
	  }

        otbAppLogINFO(<<"LUT processing ...");

	for(unsigned int column = 1; column < nbTilesX ; column++)
	  {
	    unsigned long startX = column*(sizeTilesX+1);
	  
	    ExtractROIFilterType::Pointer extractROI = ExtractROIFilterType::New();
	    extractROI->SetInput(tileFusion->GetOutput());
	    extractROI->SetStartX(startX-1);
	    extractROI->SetStartY(0);
	    extractROI->SetSizeX(2);
	    extractROI->SetSizeY(seg1Y);
	    extractROI->Update();
	
	    //Equivalence between the current tile and the left tile
	    LabelImageType::IndexType pixelIndex1;
	    LabelImageType::IndexType pixelIndex2;

	  
	    pixelIndex1[0] = 0; 
            pixelIndex2[0] = 1;
	  
	    for(pixelIndex1[1]=0;pixelIndex1[1]<static_cast<long>(seg1Y);pixelIndex1[1]++)
              {
              pixelIndex2[1] = pixelIndex1[1];
              LabelImagePixelType curCanLabel = extractROI->GetOutput()->GetPixel(pixelIndex1);
              while(LUT[curCanLabel] != curCanLabel) curCanLabel = LUT[curCanLabel];
	      LabelImagePixelType adjCanLabel = extractROI->GetOutput()->GetPixel(pixelIndex2);
	      while(LUT[adjCanLabel] != adjCanLabel) adjCanLabel = LUT[adjCanLabel];
	      if(curCanLabel < adjCanLabel) LUT[adjCanLabel] = curCanLabel;
	      else {LUT[LUT[curCanLabel]] = adjCanLabel; LUT[curCanLabel] = adjCanLabel;}
	    }
	  
	  }
    
	for(LabelImagePixelType label = 1; label < regionCount+1; ++label)
	  {                                                                                                                                  
	    LabelImagePixelType can = label;
	    while(LUT[can] != can) can = LUT[can];                                                  
	    LUT[label] = can;
	  }
 
	for(unsigned int row = 0; row < nbTilesY ; row++)
	  for(unsigned int column = 0; column < nbTilesX ; column++)
	    {
	      unsigned long startX = column*(sizeTilesX+1), startY = row*(sizeTilesY+1);
	      unsigned long sizeX = vcl_min(sizeTilesX,seg1X-startX),sizeY = vcl_min(sizeTilesY,seg1Y-startY);
	    
	      //Tiles extraction (without overlapping)
	      ExtractROIFilterType::Pointer extractROI = ExtractROIFilterType::New();
	      extractROI->SetInput(tileFusion->GetOutput());
	      extractROI->SetStartX(startX);
	      extractROI->SetStartY(startY);
	      extractROI->SetSizeX(sizeX);
	      extractROI->SetSizeY(sizeY);
	      extractROI->Update();

              std::stringstream tileOut;
              tileOut<<tilesname<<"_"<<row<<"_"<<column<<"_RELAB.tif";

              std::vector<std::string> joins;
              if(IsParameterEnabled("tmpdir"))
                {
                joins.push_back(GetParameterString("tmpdir"));
                }
              joins.push_back(tileOut.str());
              
              std::string currentFile =itksys::SystemTools::JoinPath(joins);
              createdFiles.push_back(currentFile);
              
              ImageWriter->SetInput(extractROI->GetOutput());
              ImageWriter->SetFileName(currentFile);
              ImageWriter->Update();

              LabelImageReaderType::Pointer currentReader = LabelImageReaderType::New();
              currentReader->SetFileName(currentFile);
              
              readers.push_back(currentReader);
              
	      tileFusion2->SetInput(nbTile++,currentReader->GetOutput());
	    }
     
	std::stringstream seg2;
	seg2<<tilesname<<"_tmp2.tif";

        std::vector<std::string> joins;
        if(IsParameterEnabled("tmpdir"))
          {
          joins.push_back(GetParameterString("tmpdir"));
          }
        joins.push_back(seg2.str());

        std::string currentFile =itksys::SystemTools::JoinPath(joins);
        createdFiles.push_back(currentFile);


	//Tiles relabelling
	changeLabel1->SetInput(tileFusion2->GetOutput());
	for(LabelImagePixelType label = 1;label<regionCount+1; ++label)
	  if(label!=LUT[label]) changeLabel1->SetChange(label,LUT[label]);
      }

    if((step==0)||(step==3))
      {
	LabelImageWriterType::Pointer ImageWriter = LabelImageWriterType::New();

	 std::stringstream seg2;
	 seg2<<tilesname<<"_tmp2.tif";

	 LabelImageReaderType::Pointer seg2Reader = LabelImageReaderType::New();
	 seg2Reader->SetFileName(seg2.str());

	std::vector<LabelImagePixelType> sizePerRegion; 
	sizePerRegion.clear();                
	sizePerRegion.resize(regionCount+1);
      
	for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
	  sizePerRegion[curLabel] = 0;

	std::vector<LabelImagePixelType> newLabel; 
	newLabel.clear();                
	newLabel.resize(regionCount+1);
      
	for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
	  newLabel[curLabel] = 0;

        otbAppLogINFO(<<"Small regions pruning ...");
      
	for(unsigned int column = 0; column < nbTilesX ; column++)
	  for(unsigned int row = 0; row < nbTilesY ; row++)
	    {
	      unsigned long startX = column*sizeTilesX, startY = row*sizeTilesY;
	      unsigned long sizeX = vcl_min(sizeTilesX,sizeImageX-startX),sizeY = vcl_min(sizeTilesY,sizeImageY-startY);
	    
	      ExtractROIFilterType::Pointer extractROI = ExtractROIFilterType::New();
	      extractROI->SetInput(changeLabel1->GetOutput());
	      extractROI->SetStartX(startX);
	      extractROI->SetStartY(startY);
	      extractROI->SetSizeX(sizeX);
	      extractROI->SetSizeY(sizeY);
	      extractROI->Update();
	    
	      //Size per region calculation
	      LabelImageIterator it( extractROI->GetOutput(), extractROI->GetOutput()->GetLargestPossibleRegion());
	      for (it = it.Begin(); !it.IsAtEnd(); ++it) sizePerRegion[it.Value()]++;
            }
      
	LabelImagePixelType newLab=1;
	for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
	  if(sizePerRegion[curLabel]<minRegionSize) newLabel[curLabel]=0;
	  else{newLabel[curLabel]=newLab++;}

	//Relabelling
	changeLabel2->SetInput(changeLabel1->GetOutput());
	for(LabelImagePixelType label = 1;label<regionCount+1; ++label)
	  if(label!=newLabel[label]) changeLabel2->SetChange(label,newLabel[label]);
      
      }


    SetParameterOutputImage("out", changeLabel2->GetOutput());
        
    clock_t toc = clock();
  
    otbAppLogINFO(<<"Elapsed time: "<<(double)(toc - tic) / CLOCKS_PER_SEC<<" seconds");
  
    // Cleanup
    if(IsParameterEnabled("cleanup"))
      {
      otbAppLogINFO(<<"Cleaning temporary files");
      for(std::vector<string>::const_iterator it = createdFiles.begin();
          it!=createdFiles.end();++it)
        {
        // Try to remove the geom file if existing
        std::string geomfile = it->substr(0,it->size() - itksys::SystemTools::GetFilenameExtension(it->c_str()).size()).append(".geom");

        if(itksys::SystemTools::FileExists(geomfile.c_str()))
          {
          itksys::SystemTools::RemoveFile(geomfile.c_str());
          }

        if(itksys::SystemTools::FileExists(it->c_str()))
          {
          itksys::SystemTools::RemoveFile(it->c_str());
          }
          
        }
      }
  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSSegmentation)


