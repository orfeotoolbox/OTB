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

#include "otbStreamingStatisticsImageFilter.h"
#include "otbSystem.h"
#include "itkChangeLabelImageFilter.h"

#include "otbTileImageFilter.h"

#include <time.h>
#include <vcl_algorithm.h>
#include <climits>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStandardWriterWatcher.h"


namespace otb
{
namespace Wrapper
{
class LSMSMerging : public Application
{
public:
  typedef LSMSMerging Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self)
;

  itkTypeMacro(Merging, otb::Application)
;

private:
  void DoInit()
  {
    SetName("LSMSMerging");
    SetDescription("Performs the small region pruning by merging of a segmentation.");

    SetDocName("Merging");
    SetDocLongDescription("This application performs the small region pruning by merging of a segmentation. Regions are merged by increasing sizes, starting with regions of size 1, until regions of the minimal acceptable size.");
    SetDocLimitations("");
    SetDocAuthors("David Youssefi");
    SetDocSeeAlso(" ");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage,  "in",    "Input image");
    SetParameterDescription( "in", "The input image." );
    AddParameter(ParameterType_InputImage,  "seg",    "Segmented image");
    SetParameterDescription( "seg", " The segmented image input. Segmented image input is the segmentation of the input image." );
   
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription( "out", "The output image. The output image is the input image where the minimal regions have been merged." );

    AddParameter(ParameterType_Int, "minsize", "Minimum Region Size");
    SetParameterDescription("minsize", "Minimum Region Size. If, after the segmentation, a region is of size lower than this criterion, the region is merged with the \"nearest\" region (radiometrically).");  
    SetDefaultParameterInt("minsize", 50);
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


    // Doc example parameter settings
    SetDocExampleParameterValue("in","smooth.tif");
    SetDocExampleParameterValue("seg","segmentation.tif");
    SetDocExampleParameterValue("out","merged.tif");
    SetDocExampleParameterValue("minsize","20");
    SetDocExampleParameterValue("nbtilesx","4");
    SetDocExampleParameterValue("nbtilesy","4");

  }

  void DoUpdateParameters()
  {      
  }

  void DoExecute()
  {
    clock_t tic = clock();
  
    unsigned int minSize   = GetParameterInt("minsize");
    
    unsigned int nbTilesX       = GetParameterInt("nbtilesx");
    unsigned int nbTilesY       = GetParameterInt("nbtilesy");

    typedef float ImagePixelType;
    typedef otb::VectorImage<ImagePixelType, 2> ImageType;
    typedef otb::ImageFileReader<ImageType> ImageReaderType;

    typedef unsigned int LabelImagePixelType;
    typedef otb::Image<LabelImagePixelType, 2> LabelImageType;
    typedef otb::ImageFileReader<LabelImageType> LabelImageReaderType;
    typedef otb::ImageFileWriter<LabelImageType> LabelImageWriterType;

    typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
    typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;

    typedef otb::StreamingStatisticsImageFilter<LabelImageType> StatisticsImageFilterType;

    typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;
    typedef itk::ImageRegionConstIterator<ImageType> ImageIterator;

    typedef itk::ChangeLabelImageFilter<LabelImageType,LabelImageType> ChangeLabelImageFilterType;
    typedef otb::TileImageFilter<LabelImageType> TileImageFilterType;

    //Acquisition of the input image dimensions
    ImageType::Pointer imageIn = GetParameterImage("in");
    imageIn->UpdateOutputInformation();
    unsigned long sizeImageX = imageIn->GetLargestPossibleRegion().GetSize()[0],
      sizeImageY = imageIn->GetLargestPossibleRegion().GetSize()[1];
    unsigned int numberOfComponentsPerPixel = imageIn->GetNumberOfComponentsPerPixel();

    LabelImageType::Pointer labelIn = GetParameterUInt32Image("seg");

    StatisticsImageFilterType::Pointer stats = StatisticsImageFilterType::New();
    stats->SetInput(labelIn);	
    stats->Update();
    unsigned int regionCount=stats->GetMaximum();

    std::vector<int>nbPixels; 
    nbPixels.clear();                
    nbPixels.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      nbPixels[curLabel] = 0;

    ImageType::PixelType defaultValue(numberOfComponentsPerPixel);
    defaultValue.Fill(0);

    std::vector<ImageType::PixelType>sum(regionCount+1,defaultValue);
   
    long nbStars = 50;

    //Sums calculation per label
    std::cout<<"Sums calculation (0%) [";
    for(unsigned int add=0;add<nbStars;add++)
      std::cout<<" ";
    std::cout<<"]"<<std::flush;
    unsigned int count=0;

    unsigned long sizeTilesX = (sizeImageX+nbTilesX-1)/nbTilesX, sizeTilesY = (sizeImageY+nbTilesY-1)/nbTilesY;  

    for(unsigned int row = 0; row < nbTilesY ; row++)
      for(unsigned int column = 0; column < nbTilesX ; column++)
	{	
	  unsigned long startX = column*sizeTilesX, startY = row*sizeTilesY;
	  unsigned long sizeX = vcl_min(sizeTilesX,sizeImageX-startX),sizeY = vcl_min(sizeTilesY,sizeImageY-startY);

	  //Tiles extraction of the input image
	  MultiChannelExtractROIFilterType::Pointer imageROI = MultiChannelExtractROIFilterType::New();
	  imageROI->SetInput(imageIn);
	  imageROI->SetStartX(startX);
	  imageROI->SetStartY(startY);
	  imageROI->SetSizeX(sizeX);
	  imageROI->SetSizeY(sizeY);
	  imageROI->Update();

	  //Tiles extraction of the segmented image
	  ExtractROIFilterType::Pointer labelImageROI = ExtractROIFilterType::New();
	  labelImageROI->SetInput(labelIn);
	  labelImageROI->SetStartX(startX);
	  labelImageROI->SetStartY(startY);
	  labelImageROI->SetSizeX(sizeX);
	  labelImageROI->SetSizeY(sizeY);
	  labelImageROI->Update();

	  //Sums calculation for the mean calculation per label
	  LabelImageIterator itLabel( labelImageROI->GetOutput(), labelImageROI->GetOutput()->GetLargestPossibleRegion());
	  ImageIterator itImage( imageROI->GetOutput(), imageROI->GetOutput()->GetLargestPossibleRegion());
	  for (itLabel.GoToBegin(), itImage.GoToBegin(); !itLabel.IsAtEnd(); ++itLabel, ++itImage){	
	    nbPixels[itLabel.Value()]++;
	    for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
	      sum[itLabel.Value()][comp]+=itImage.Get()[comp];
	  }
	
	  count++;
	  unsigned int progress = (count*100)/(nbTilesX*nbTilesY);
	  std::cout<<"\rSums calculation ("<<progress<<"%) [";
	  for(unsigned int add=0;add<(progress*nbStars)/100;add++)
	    std::cout<<"*";
	  for(unsigned int add=(progress*nbStars)/100;add<nbStars;add++)
	    std::cout<<" ";
	  std::cout<<"]"<<std::flush;
	}
    std::cout<<std::endl;

    //LUT creation for the final relabelling
    std::vector<LabelImagePixelType> LUT;
    LUT.clear();
    LUT.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      LUT[curLabel] = curLabel;

    std::cout<<"Region suppression (0%) [";
    for(unsigned int add=0;add<nbStars;add++)
      std::cout<<" ";
    std::cout<<"]"<<std::flush;
    count=0;

    //Minimal size region suppression
    for (int size=1;size<minSize;size++)
      {      
	//LUTtmp creation in order to modify the LUT only at the end of the pass
	std::vector<LabelImagePixelType> LUTtmp;
	LUTtmp.clear();
	LUTtmp.resize(regionCount+1);
	for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
	  LUTtmp[curLabel] = LUT[curLabel];

	for(unsigned int row = 0; row < nbTilesY ; row++)
	  for(unsigned int column = 0; column < nbTilesX ; column++)
	    {		
	      std::set<int> minLabel, edgeLabel, labelMerged;
	      std::map<int,std::set<int> > adjMap;

	      unsigned long startX = column*sizeTilesX, startY = row*sizeTilesY;
	      unsigned long sizeX = vcl_min(sizeTilesX+size+1,sizeImageX-startX),
		sizeY = vcl_min(sizeTilesY+size+1,sizeImageY-startY);

	      ExtractROIFilterType::Pointer labelImageROI = ExtractROIFilterType::New();
	      labelImageROI->SetInput(labelIn);
	      labelImageROI->SetStartX(startX);
	      labelImageROI->SetStartY(startY);
	      labelImageROI->SetSizeX(sizeX);
	      labelImageROI->SetSizeY(sizeY);
	      labelImageROI->Update();
	   
	      LabelImageType::IndexType pixelIndex; pixelIndex[0]=0;pixelIndex[1]=0;
	      long &i = pixelIndex[0], &j = pixelIndex[1];
	   
	      //"Adjacency map" creation for the region with nbPixels=="size"
	      for(i=0;i<sizeX;i++)
		for(j=0;j<sizeY;j++)
		  {
		    LabelImagePixelType curLabel = labelImageROI->GetOutput()->GetPixel(pixelIndex);
		 
		    if(labelMerged.count(LUT[curLabel])) edgeLabel.insert(LUT[curLabel]);
		    if((i==0)&&(startX!=0)) edgeLabel.insert(LUT[curLabel]);
		    if((j==0)&&(startY!=0)) edgeLabel.insert(LUT[curLabel]);

		    if(i==sizeX-1){if(startX!=(nbTilesX-1)*sizeTilesX) edgeLabel.insert(LUT[curLabel]);}
		    else{
		      i++;LabelImagePixelType adjLabelX = labelImageROI->GetOutput()->GetPixel(pixelIndex);i--;
		      if(LUT[adjLabelX]!=LUT[curLabel])
			{
			  if((nbPixels[LUT[curLabel]]>0)&&(nbPixels[LUT[curLabel]]==size))
			    {adjMap[LUT[curLabel]].insert(LUT[adjLabelX]);minLabel.insert(LUT[curLabel]);}
			  if((nbPixels[LUT[adjLabelX]]>0)&&(nbPixels[LUT[adjLabelX]]==size))
			    {adjMap[LUT[adjLabelX]].insert(LUT[curLabel]);minLabel.insert(LUT[adjLabelX]);}
			}}
		    if(j==sizeY-1){if(startY!=(nbTilesY-1)*sizeTilesY) edgeLabel.insert(LUT[curLabel]);}
		    else{
		      j++;LabelImagePixelType adjLabelY = labelImageROI->GetOutput()->GetPixel(pixelIndex);j--;
		      if(LUT[adjLabelY]!=LUT[curLabel])
			{
			  if((nbPixels[LUT[curLabel]]>0)&&(nbPixels[LUT[curLabel]]==size))
			    {adjMap[LUT[curLabel]].insert(LUT[adjLabelY]);minLabel.insert(LUT[curLabel]);}
			  if((nbPixels[LUT[adjLabelY]]>0)&&(nbPixels[LUT[adjLabelY]]==size))
			    {adjMap[LUT[adjLabelY]].insert(LUT[curLabel]);minLabel.insert(LUT[adjLabelY]);}
			}}
		  }

	      //Searching the "nearest" region
	      for (std::set<int>::iterator itMinLabel=minLabel.begin(); itMinLabel!=minLabel.end(); ++itMinLabel)
		{   
		  LabelImagePixelType curLabel = *itMinLabel, adjLabel, err = INT_MAX;
		  if(edgeLabel.count(curLabel)==0){
		    if(nbPixels[curLabel]==size){
		      edgeLabel.insert(curLabel);
		      for (std::set<int>::iterator itAdjLabel=adjMap[curLabel].begin(); itAdjLabel!=adjMap[curLabel].end(); ++itAdjLabel)
			{
			  int tmpError = 0; LabelImagePixelType tmpLabel = *itAdjLabel;
			  if(tmpLabel!=curLabel){
			    for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
			      {
				int curComp = sum[curLabel][comp]/nbPixels[curLabel];
				int tmpComp = sum[tmpLabel][comp]/nbPixels[tmpLabel];
				tmpError += (curComp-tmpComp)*(curComp-tmpComp);
			      }
			    if(tmpError<err)
			      {
				err = tmpError;
				adjLabel = tmpLabel;
			      }}
			}
		   
		      //Fusion of the two regions
		      if(adjLabel!=curLabel){
			int curLabelLUT = curLabel, adjLabelLUT = adjLabel;
			while(LUTtmp[curLabelLUT] != curLabelLUT) curLabelLUT = LUTtmp[curLabelLUT];
			while(LUTtmp[adjLabelLUT] != adjLabelLUT) adjLabelLUT = LUTtmp[adjLabelLUT];
			if(curLabelLUT < adjLabelLUT) LUTtmp[adjLabelLUT] = curLabelLUT;
			else{LUTtmp[LUTtmp[curLabelLUT]] = adjLabelLUT; LUTtmp[curLabelLUT] = adjLabelLUT;}}
		    }}
		}
	       
	      for(LabelImagePixelType label = 1; label < regionCount+1; ++label)
		{
		  LabelImagePixelType can = label;
		  while(LUTtmp[can] != can) can = LUTtmp[can];
		  LUTtmp[label] = can;
		}

	    }    
	 
	for(LabelImagePixelType label = 1; label < regionCount+1; ++label)
	  {
	    LUT[label]=LUTtmp[label];
	    if((nbPixels[label]!=0)&&(LUT[label]!=label))
	      {
		nbPixels[LUT[label]]+=nbPixels[label];
		nbPixels[label]=0;
		for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
		  sum[LUT[label]][comp]+=sum[label][comp];
	      }
	  } 
        
	unsigned int progress = (size*100)/(minSize-1);
	std::cout<<"\rRegion suppression ("<<progress<<"%) [";
	for(unsigned int add=0;add<(progress*nbStars)/100;add++)
	  std::cout<<"*";
	for(unsigned int add=(progress*nbStars)/100;add<nbStars;add++)
	  std::cout<<" ";
	std::cout<<"]"<<std::flush;
      }
    std::cout<<std::endl;

    //Relabelling
    ChangeLabelImageFilterType::Pointer changeLabel = ChangeLabelImageFilterType::New();
    changeLabel->SetInput(labelIn);
    for(LabelImagePixelType label = 1;label<regionCount+1; ++label)
      if(label!=LUT[label]) changeLabel->SetChange(label,LUT[label]);
    changeLabel->Update();
    
    SetParameterOutputImage("out", changeLabel->GetOutput());

    clock_t toc = clock();

    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSMerging)


