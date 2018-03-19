/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"


#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbConnectedComponentMuParserFunctor.h"

#include "otbOGRDataToSpectralStatisticsFilter.h"



namespace otb
{
namespace Wrapper
{

class ComputePolygonsSpectralFeatures : public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputePolygonsSpectralFeatures                       Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  typedef otb::ogr::DataSource                            OGRDataSourceType;
  typedef otb::ogr::Layer                                 OGRLayerType;
  typedef otb::ogr::Feature                               OGRFeatureType;

  //typedef itk::BinaryShapeM_BinaryOpeningFilterImageFilter<UInt8LabelImageType>    BinaryM_BinaryOpeningFilterFilterType;
  typedef otb::Image<unsigned int, 2>                                 LabelImageType;
  typedef otb::Image<unsigned int, 2>         MaskImageType;

  typedef float InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> VectorImageType;
  typedef VectorImageType::PixelType VectorImagePixelType;
  
  
  
  typedef OGRDataToSpectralStatisticsFilter<VectorImageType, MaskImageType>                         OGRDataToSpectralStatisticsFilterType;
  
  
  typedef Functor::ConnectedComponentMuParserFunctor<VectorImagePixelType> FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<
      VectorImageType,
      LabelImageType,
      FunctorType,
      MaskImageType > ConnectedComponentFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputePolygonsSpectralFeatures, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("ComputePolygonsSpectralFeatures");
    SetDescription("This application performs the vectorization of an input binary mask");

    // Documentation
    SetDocName("Mask Filtering");
    SetDocLongDescription("Given an input binary raster image, this application will output a vector data file containing a polygon for each connected component of the input raster. Additionnaly, the size of each polygon will be computed and added to the raster");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_InputFilename, "vec", "Input sampling positions");
    SetParameterDescription("vec","Vector data file containing sampling"
                                  "positions. (OGR format)");


    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file ");
    SetParameterDescription("out","Output vector");
    MandatoryOff("out");
    

    AddParameter(ParameterType_Int,"tile","Tile Size");
    SetParameterDescription("tile","Size of the tiles used for streaming");
    MandatoryOff("tile");
    
    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("tile", 0);
    // Doc example parameter settings
    SetDocExampleParameterValue("expr", "\"distance<10\"");
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("out","test.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
    clock_t tic = clock();
    
    
    // Create the OGR DataSource with the appropriate fields
    std::string projRef = this->GetParameterImage("in")->GetProjectionRef();
    OGRSpatialReference oSRS(projRef.c_str());
    std::string layer_name = "layer";
    std::string field_name = "field";
    
    OGRDataSourceType::Pointer vectors;
    OGRDataSourceType::Pointer output;
    if (IsParameterEnabled("out") && HasValue("out"))
      {
      vectors = ogr::DataSource::New(this->GetParameterString("vec"));
      output = ogr::DataSource::New(this->GetParameterString("out"),
                                    ogr::DataSource::Modes::Overwrite);
      }
    else
      {
      // Update mode
      vectors = ogr::DataSource::New(this->GetParameterString("vec"),
                                    ogr::DataSource::Modes::Update_LayerUpdate);
      output = vectors;
      }
    //ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::Update_LayerUpdate);
    //ogrDS = otb::ogr::DataSource::New(GetParameterString("out"), otb::ogr::DataSource::Modes::Update_LayerUpdate);
    //OGRLayerType layer = ogrDS->CreateLayer(layer_name, &oSRS, wkbMultiPolygon);
    /*OGRLayerType layer = ogrDS->CreateLayer(layer_name, &oSRS, wkbPolygon);
    OGRFieldDefn field(field_name.c_str(), OFTInteger);
    layer.CreateField(field, true);
*/


    ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();
    connected->SetInput(this->GetParameterImage("in"));
    connected->GetFunctor().SetExpression("distance<11");
    connected->Update();



    OGRDataToSpectralStatisticsFilterType::Pointer SpectralStatisticsFilter = OGRDataToSpectralStatisticsFilterType::New();
    SpectralStatisticsFilter->SetInput(this->GetParameterImage("in"));
      SpectralStatisticsFilter->UpdateLargestPossibleRegion() ;
    SpectralStatisticsFilter->SetOGRData(vectors);
    SpectralStatisticsFilter->SetOutputSamples(output);
    //SpectralStatisticsFilter->SetOGRLayer(layer);
    SpectralStatisticsFilter->SetFieldName(field_name);
    std::cout << this->GetParameterImage("in")->GetLargestPossibleRegion() << std::endl;
    SpectralStatisticsFilter->Update();
    
    clock_t toc = clock();
    
    otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");
  }
  
  

}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputePolygonsSpectralFeatures)
