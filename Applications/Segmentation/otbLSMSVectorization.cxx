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

#include "otbMultiChannelExtractROI.h"
#include "otbExtractROI.h"

#include "otbStreamingStatisticsImageFilter.h"
#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include <time.h>
#include <vcl_algorithm.h>

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

  typedef FloatVectorImageType              ImageType;
  typedef ImageType::InternalPixelType      ImagePixelType;
  typedef UInt32ImageType                   LabelImageType;
  typedef LabelImageType::InternalPixelType LabelImagePixelType;

  typedef otb::ImageFileReader<LabelImageType> LabelImageReaderType;

  typedef otb::MultiChannelExtractROI <ImagePixelType,ImagePixelType > MultiChannelExtractROIFilterType;
  typedef otb::ExtractROI<LabelImagePixelType,LabelImagePixelType> ExtractROIFilterType;

  typedef otb::StreamingStatisticsImageFilter<LabelImageType> StatisticsImageFilterType;

  typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;
  typedef itk::ImageRegionConstIterator<ImageType> ImageIterator;

  typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType> LabelImageToOGRDataSourceFilterType;


  itkNewMacro(Self);

  itkTypeMacro(Vectorization, otb::Application);

private:
  void DoInit()
  {
    SetName("LSMSVectorization");
    SetDescription("Fourth step of the exact Large-Scale Mean-Shift segmentation workflow.");

    SetDocName("Exact Large-Scale Mean-Shift segmentation, step 4");
    SetDocLongDescription("This application performs the fourth step of the exact Large-Scale Mean-Shift segmentation workflow (LSMS). Given a segmentation result (label image), that may have been processed for small regions merging or not, it will convert it to a GIS vector file containing one polygon per segment. Each polygon contains additional fields: mean and variance of each channels from input image (in parameter), segmentation image label, number of pixels in the polygon. For large images one can use the nbtilesx and nbtilesy parameters for tile-wise processing, with the guarantees of identical results.");
    SetDocLimitations("This application is part of the Large-Scale Mean-Shift segmentation workflow (LSMS) and may not be suited for any other purpose.");
    SetDocAuthors("David Youssefi");
    SetDocSeeAlso("MeanShiftSmoothing, LSMSSegmentation, LSMSSmallRegionsMerging");
    AddDocTag(Tags::Segmentation);
    AddDocTag("LSMS");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription( "in", "The input image." );
    AddParameter(ParameterType_InputImage,  "inseg",    "Segmented image");
    SetParameterDescription( "inseg", " The segmented image input. Segmented image input is the segmentation of the input image." );

    AddParameter(ParameterType_OutputFilename, "out", "Output GIS vector file");
    SetParameterDescription( "out", "The output GIS vector file, representing the vectorized version of the segmented image where the features of the polygons are the radiometric means and variances." );

    AddParameter(ParameterType_Int, "tilesizex", "Size of tiles in pixel (X-axis)");
    SetParameterDescription("tilesizex", "Size of tiles along the X-axis.");
    SetDefaultParameterInt("tilesizex", 500);
    SetMinimumParameterIntValue("tilesizex", 1);

    AddParameter(ParameterType_Int, "tilesizey", "Size of tiles in pixel (Y-axis)");
    SetParameterDescription("tilesizey", "Size of tiles along the Y-axis.");
    SetDefaultParameterInt("tilesizey", 500);
    SetMinimumParameterIntValue("tilesizey", 1);

    // Doc example parameter settings
    SetDocExampleParameterValue("in","avions.tif");
    SetDocExampleParameterValue("inseg","merged.tif");
    SetDocExampleParameterValue("out","vector.shp");
    SetDocExampleParameterValue("tilesizex","256");
    SetDocExampleParameterValue("tilesizey","256");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    clock_t tic = clock();

    std::string shapefile(GetParameterString("out"));

    unsigned long sizeTilesX = GetParameterInt("tilesizex");
    unsigned long sizeTilesY = GetParameterInt("tilesizey");


    LabelImageType::Pointer labelIn = GetParameterUInt32Image("inseg");
    labelIn->UpdateOutputInformation();

    unsigned long sizeImageX = labelIn->GetLargestPossibleRegion().GetSize()[0];
    unsigned long sizeImageY = labelIn->GetLargestPossibleRegion().GetSize()[1];

    unsigned int nbTilesX = sizeImageX/sizeTilesX + (sizeImageX%sizeTilesX > 0 ? 1 : 0);
    unsigned int nbTilesY = sizeImageY/sizeTilesY + (sizeImageY%sizeTilesY > 0 ? 1 : 0);

    otbAppLogINFO(<<"Number of tiles: "<<nbTilesX<<" x "<<nbTilesY);

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
      {
      nbPixels[curLabel] = 0;
      }

    ImageType::PixelType defaultValue(numberOfComponentsPerPixel);
    defaultValue.Fill(0);

    std::vector<ImageType::PixelType>sum(regionCount+1,defaultValue);
    std::vector<ImageType::PixelType>sum2(regionCount+1,defaultValue);

    otb::ogr::DataSource::Pointer ogrDS;
    otb::ogr::Layer layer(NULL, false);

    OGRSpatialReference oSRS(projRef.c_str());
    std::vector<std::string> options;

    ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Overwrite);
    std::string layername = itksys::SystemTools::GetFilenameName(shapefile.c_str());
    std::string extension = itksys::SystemTools::GetFilenameLastExtension(shapefile.c_str());
    layername = layername.substr(0,layername.size()-(extension.size()));
    layer = ogrDS->CreateLayer(layername, &oSRS, wkbMultiPolygon, options);

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

    //Vectorization per tile
    otbAppLogINFO(<<"Vectorization ...");
    for(unsigned int row = 0; row < nbTilesY; row++)
      {
      for(unsigned int column = 0; column < nbTilesX; column++)
       {
        unsigned long startX = column*sizeTilesX;
        unsigned long startY = row*sizeTilesY;
        unsigned long sizeX = vcl_min(sizeTilesX,sizeImageX-startX);
          unsigned long sizeY = vcl_min(sizeTilesY,sizeImageY-startY);

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

        //Sums calculation for the mean and the variance calculation per label
        LabelImageIterator itLabel( labelImageROI->GetOutput(), labelImageROI->GetOutput()->GetLargestPossibleRegion());
        ImageIterator itImage( imageROI->GetOutput(), imageROI->GetOutput()->GetLargestPossibleRegion());
        for (itLabel.GoToBegin(), itImage.GoToBegin(); !itImage.IsAtEnd(); ++itLabel, ++itImage)
          {
          nbPixels[itLabel.Value()]++;
          for(unsigned int comp = 0; comp<numberOfComponentsPerPixel; ++comp)
            {
            sum[itLabel.Value()][comp]+=itImage.Get()[comp];
            sum2[itLabel.Value()][comp]+=itImage.Get()[comp]*itImage.Get()[comp];
            }
          }

        labelImageROI = ExtractROIFilterType::New();
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
       }
      }

    //Sorting by increasing label of the features
    std::ostringstream sqloss;
    sqloss.str("");
    sqloss<<"SELECT * FROM \""<<layername<<"\" ORDER BY label";
    otb::ogr::Layer layerTmp=ogrDS->ExecuteSQL(sqloss.str().c_str(), NULL, NULL);
    otb::ogr::Feature firstFeature = layerTmp.ogr().GetNextFeature();

    //Geometry fusion
    otbAppLogINFO("Merging polygons across tiles ...");
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
        else
          {
          merging=false;
          }
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

    layer.ogr().CommitTransaction();

    if(extension==".shp"){
    sqloss.str("");
    sqloss<<"REPACK "<<layername;
    ogrDS->ogr().ExecuteSQL(sqloss.str().c_str(), NULL, NULL);
    }

    ogrDS->SyncToDisk();

    clock_t toc = clock();

    otbAppLogINFO(<<"Elapsed time: "<<(double)(toc - tic) / CLOCKS_PER_SEC<<" seconds");
  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LSMSVectorization)
