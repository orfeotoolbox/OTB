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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbImage.h"
#include "otbVectorImage.h"

#include "otbMultiChannelExtractROI.h"
#include "otbExtractROI.h"

#include "otbStreamingStatisticsImageFilter.h"
#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbSystem.h"

#include <time.h>
#include <vcl_algorithm.h>

#define PI 3.14159265359

namespace otb
{
namespace Wrapper
{
class LSMSVectorization : public Application
{
public:
  typedef LSMSVectorization Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self)
;

  itkTypeMacro(Vectorization, otb::Application)
;

private:
  void DoInit()
  {
    SetName("LSMSVectorization");
    SetDescription("Performs the vectorization of a segmentation.");

    SetDocName("Vectorization");
    SetDocLongDescription("This application performs the vectorization of a segmentation. It converts the labelled regions to polygons where the features are the radiometric means and variances.");
    SetDocLimitations("");
    SetDocAuthors("David Youssefi");
    SetDocSeeAlso(" ");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription( "in", "The input image." );
    AddParameter(ParameterType_InputImage,  "seg",    "Segmented image");
    SetParameterDescription( "seg", " The segmented image input. Segmented image input is the segmentation of the input image." );

    AddParameter(ParameterType_OutputFilename, "out", "Shapefile Name");
    SetParameterDescription( "out", "The shapefile name. The shapefile is the vectorized version of the segmented image where the features of the polygones are the radiometric means and variances." );

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
    SetDocExampleParameterValue("in","avions.tif");
    SetDocExampleParameterValue("seg","merged.tif");
    SetDocExampleParameterValue("out","vecto.shp");
    SetDocExampleParameterValue("nbtilesx","4");
    SetDocExampleParameterValue("nbtilesy","4");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    clock_t tic = clock();

    const char * shapefile = GetParameterString("out").c_str();
    
    unsigned int nbTilesX       = GetParameterInt("nbtilesx");
    unsigned int nbTilesY       = GetParameterInt("nbtilesy");
   
    typedef float ImagePixelType;
    typedef otb::VectorImage<ImagePixelType, 2> ImageType;
    typedef otb::ImageFileReader<ImageType> ImageReaderType;

    typedef unsigned int LabelImagePixelType;
    typedef otb::Image<LabelImagePixelType, 2> LabelImageType;
    typedef otb::ImageFileReader<LabelImageType> LabelImageReaderType;

    typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
    typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;

    typedef otb::StreamingStatisticsImageFilter<LabelImageType> StatisticsImageFilterType;

    typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;
    typedef itk::ImageRegionConstIterator<ImageType> ImageIterator;

    typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType> LabelImageToOGRDataSourceFilterType;

    LabelImageType::Pointer labelIn = GetParameterUInt32Image("seg");
    labelIn->UpdateOutputInformation();
    unsigned long sizeImageX = labelIn->GetLargestPossibleRegion().GetSize()[0],
      sizeImageY = labelIn->GetLargestPossibleRegion().GetSize()[1];
    StatisticsImageFilterType::Pointer stats = StatisticsImageFilterType::New();
    stats->SetInput(labelIn);	
    stats->Update();
    unsigned int regionCount=stats->GetMaximum();

    ImageType::Pointer imageIn = GetParameterImage("in");
    imageIn->UpdateOutputInformation();

    unsigned long numberOfComponentsPerPixel = imageIn->GetNumberOfComponentsPerPixel();
    std::string projRef = imageIn->GetProjectionRef();

    std::vector<int>nbPixels; 
    nbPixels.clear();                
    nbPixels.resize(regionCount+1);
    for(LabelImagePixelType curLabel = 1; curLabel <= regionCount; ++curLabel)
      nbPixels[curLabel] = 0;

    ImageType::PixelType defaultValue(numberOfComponentsPerPixel);
    defaultValue.Fill(0);

    std::vector<ImageType::PixelType>sum(regionCount+1,defaultValue);
    std::vector<ImageType::PixelType>sum2(regionCount+1,defaultValue);
 
    otb::ogr::DataSource::Pointer ogrDS;
    otb::ogr::Layer layer(NULL, false);

    OGRSpatialReference oSRS(projRef.c_str());
    std::vector<std::string> options;

    ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Overwrite);
    std::string layername = otb::System::GetShortFileName(shapefile);
    std::string extension = otb::System::GetExtension(shapefile);
    layername = layername.substr(0,layername.size()-(extension.size()+1));
    layer = ogrDS->CreateLayer(layername, &oSRS, wkbMultiPolygon, options);

    OGRFieldDefn classField("class", OFTInteger);
    layer.CreateField(classField, true);
    OGRFieldDefn labelField("label", OFTInteger);
    layer.CreateField(labelField, true);
    OGRFieldDefn nbPixelsField("nbPixels", OFTInteger);
    layer.CreateField(nbPixelsField, true);
  
    for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp){
      std::ostringstream fieldoss;
      fieldoss<<"meanB"<<comp;
      OGRFieldDefn field(fieldoss.str().c_str(), OFTReal);
      layer.CreateField(field, true);
    }

    for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp){
      std::ostringstream fieldoss;
      fieldoss<<"varB"<<comp;
      OGRFieldDefn field(fieldoss.str().c_str(), OFTReal);
      layer.CreateField(field, true);
    }

    long nbStars = 50;

    //Vectorization per tile
    std::cout<<"Vectorization  (0%) [";
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
	  {MultiChannelExtractROIFilterType::Pointer imageROI = MultiChannelExtractROIFilterType::New();
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

	    //Sums calculation for the mean and the variance calculation per label
	    LabelImageIterator itLabel( labelImageROI->GetOutput(), labelImageROI->GetOutput()->GetLargestPossibleRegion());
	    ImageIterator itImage( imageROI->GetOutput(), imageROI->GetOutput()->GetLargestPossibleRegion());
	    for (itLabel.GoToBegin(), itImage.GoToBegin(); !itImage.IsAtEnd(); ++itLabel, ++itImage){	
	      nbPixels[itLabel.Value()]++;
	      for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
		sum[itLabel.Value()][comp]+=itImage.Get()[comp];
	      for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
		sum2[itLabel.Value()][comp]+=itImage.Get()[comp]*itImage.Get()[comp];
	    }}

	  ExtractROIFilterType::Pointer labelImageROI = ExtractROIFilterType::New();
	  labelImageROI->SetInput(labelIn);
	  labelImageROI->SetStartX(startX);
	  labelImageROI->SetStartY(startY);
	  labelImageROI->SetSizeX(sizeX+1);
	  labelImageROI->SetSizeY(sizeY+1);
	  labelImageROI->Update();

	  //Raster->Vecteur conversion
	  LabelImageToOGRDataSourceFilterType::Pointer labelToOGR = LabelImageToOGRDataSourceFilterType::New();
	  labelToOGR->SetInput(labelImageROI->GetOutput());
	  labelToOGR->SetInputMask(labelImageROI->GetOutput());
	  labelToOGR->SetFieldName("label");
	  labelToOGR->Update();

	  otb::ogr::DataSource::ConstPointer ogrDSTmp = labelToOGR->GetOutput();
	  otb::ogr::Layer layerTmp = ogrDSTmp->GetLayerChecked(0);

	  otb::ogr::Layer::const_iterator featIt = layerTmp.begin();
	  for(; featIt!=layerTmp.end(); ++featIt)
	    {
	      otb::ogr::Feature dstFeature(layer.GetLayerDefn());
	      dstFeature.SetFrom( *featIt, TRUE );
	      layer.CreateFeature( dstFeature );
	    }
	
	  count++;
	  unsigned int progress = (count*100)/(nbTilesX*nbTilesY);
	  std::cout<<"\rVectorization  ("<<progress<<"%) [";
	  for(unsigned int add=0;add<(progress*nbStars)/100;add++)
	    std::cout<<"*";
	  for(unsigned int add=(progress*nbStars)/100;add<nbStars;add++)
	    std::cout<<" ";
	  std::cout<<"]"<<std::flush;
	}
    std::cout<<std::endl;

    //Sorting by increasing label of the features
    std::ostringstream sqloss;
    sqloss.str("");
    sqloss<<"SELECT * FROM "<<layername<<" ORDER BY label";
    otb::ogr::Layer layerTmp=ogrDS->ExecuteSQL(sqloss.str().c_str(), NULL, NULL);
 
    bool goesOn = true;
    int nbFeatures = layerTmp.ogr().GetFeatureCount(true);
    int nb = 0;
    otb::ogr::Feature firstFeature = layerTmp.ogr().GetNextFeature();

    //Geometry fusion
    std::cout<<"Fusion (0%) [";
    for(unsigned int add=0;add<nbStars;add++)
      std::cout<<" ";
    std::cout<<"]"<<std::flush;
    count=0;
    unsigned int progress;

    while(firstFeature.addr())
      {
	LabelImagePixelType curLabel = firstFeature.ogr().GetFieldAsInteger("label");
     
	//Creation of a multipolygon where are stored the geometries to be merged
	OGRMultiPolygon geomToMerge;
	geomToMerge.addGeometry(firstFeature.GetGeometry());
	bool merging = true;
	otb::ogr::Feature nextFeature(NULL);
	bool haveMerged=false;

	while(merging)
	  {  
	    nextFeature = layerTmp.ogr().GetNextFeature();
	    nb++;
	    progress = (nb*100)/nbFeatures;
	    std::cout<<"\rFusion ("<<progress<<"%) ("<<nb<<"/"<<nbFeatures<<") [";
	    for(unsigned int add=0;add<(progress*nbStars)/100;add++)
	      std::cout<<"*";
	    for(unsigned int add=(progress*nbStars)/100;add<nbStars;add++)
	      std::cout<<" ";
	    std::cout<<"]"<<std::flush;
	  
	    if(nextFeature.addr())
	      {
		LabelImagePixelType newLabel = nextFeature.ogr().GetFieldAsInteger("label");
		merging=(newLabel==curLabel);
	      
		//Storing of the new geometry if labels are identical
		if(merging)
		  {
		    geomToMerge.addGeometry(nextFeature.GetGeometry());
		    layer.DeleteFeature(nextFeature.GetFID());
		    haveMerged=true;
		  }
		//If storing made and new label -> polygons fusion
		else if(haveMerged)
		  {
		    otb::ogr::UniqueGeometryPtr fusionPolygon = otb::ogr::UnionCascaded(geomToMerge);
		    firstFeature.SetGeometry(fusionPolygon.get());
		  }
	      }
	    //If end of list : end of loop
	    else merging=false;
	  }

	//Features calculation
	//Number of pixels per label
	firstFeature.ogr().SetField("nbPixels",nbPixels[curLabel]);

	//Radiometric means per label
	for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp){
	  std::ostringstream fieldoss;
	  fieldoss<<"meanB"<<comp;
	  firstFeature.ogr().SetField(fieldoss.str().c_str(),sum[curLabel][comp]/nbPixels[curLabel]);
	}

	//Variances per label
	for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp){
	  std::ostringstream fieldoss;
	  fieldoss<<"varB"<<comp;
	  float var = 0;
	  if (nbPixels[curLabel]!=1)
	    var = (sum2[curLabel][comp]-sum[curLabel][comp]*sum[curLabel][comp]/nbPixels[curLabel])/(nbPixels[curLabel]-1);
	  firstFeature.ogr().SetField(fieldoss.str().c_str(),var);
	}
      
	//Geometries simplification
	otb::ogr::UniqueGeometryPtr geom = otb::ogr::Simplify(*firstFeature.GetGeometry(),0);
	firstFeature.SetGeometryDirectly(otb::ogr::Simplify(*geom,0));	  
	  
	layer.SetFeature(firstFeature);
      
	//Next geometry
	firstFeature=nextFeature;
      }
    std::cout<<std::endl;
  
    layer.ogr().CommitTransaction();
  
    if(extension=="shp"){
      sqloss.str("");
      sqloss<<"REPACK "<<layername;
      ogrDS->ogr().ExecuteSQL(sqloss.str().c_str(), NULL, NULL);
    }
  
    ogrDS->SyncToDisk();
  
    clock_t toc = clock();

    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSVectorization)


