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

#include "otbOGRDataToSpectralStatisticsFilter.h"

#include "otbStopwatch.h"

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

  typedef otb::Image<unsigned int, 2>         MaskImageType;

  typedef float InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> VectorImageType;
  typedef VectorImageType::PixelType VectorImagePixelType;
  
  typedef OGRDataToSpectralStatisticsFilter<VectorImageType, MaskImageType>                         OGRDataToSpectralStatisticsFilterType;
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputePolygonsSpectralFeatures, otb::Application);

private:
  void DoInit() override
  {
    SetName("ComputePolygonsSpectralFeatures");
    SetDescription("This application computes radiometric features on a set of polygons using a multi-band image");

    // Documentation
    SetDocName("ComputePolygonsSpectralFeatures");
    SetDocLongDescription("This application computes radiometric features on a set of polygons using a multi-band image");
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
    
    AddParameter(ParameterType_String, "minfield", "Min field prefix");
    SetParameterDescription("minfield", "Prefix of the field containing minimum value for each band in the geometry");
    MandatoryOff("minfield");
      
    AddParameter(ParameterType_String, "maxfield", "Max field prefix");
    SetParameterDescription("maxfield", "Prefix of the field containing maximum value for each band in the geometry");
    MandatoryOff("maxfield");
    
    AddParameter(ParameterType_String, "meanfield", "Mean field prefix");
    SetParameterDescription("meanfield", "Prefix of the field containing mean of the values in the geometry for each band");
    MandatoryOff("meanfield");
    
    AddParameter(ParameterType_String, "covfield", "Covariance field prefix");
    SetParameterDescription("covfield", "Prefix of the field containing the covariance matrix between different bands, for samples inside the geometry");
    MandatoryOff("covfield");
    
    AddParameter(ParameterType_String, "nbpixelfield", "Number of pixel field prefix");
    SetParameterDescription("nbpixelfield", "Name of the field containing the number of pixel in each geometries");
    MandatoryOff("nbpixelfield");
    
    AddParameter(ParameterType_Int,"tile","Tile Size");
    SetParameterDescription("tile","Size of the tiles used for streaming");
    MandatoryOff("tile");
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("expr", "\"distance<10\"");
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("out","test.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    // Start Timer for the application
    auto Timer = Stopwatch::StartNew();
    
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
    OGRDataToSpectralStatisticsFilterType::Pointer SpectralStatisticsFilter = OGRDataToSpectralStatisticsFilterType::New();
    SpectralStatisticsFilter->SetInput(this->GetParameterImage("in"));
    if (IsParameterEnabled("tile") && HasValue("tile"))
      SpectralStatisticsFilter->GetStreamer()->SetTileDimensionTiledStreaming(this->GetParameterInt("tile"));
    else
      SpectralStatisticsFilter->GetStreamer()->SetAutomaticTiledStreaming();
    
    // Name of fields  
    if (IsParameterEnabled("minfield") && HasValue("minfield"))
      SpectralStatisticsFilter->SetMinField(GetParameterString("minfield"));
    if (IsParameterEnabled("maxfield") && HasValue("maxfield"))
      SpectralStatisticsFilter->SetMaxField(GetParameterString("maxfield"));
    if (IsParameterEnabled("meanfield") && HasValue("meanfield"))
      SpectralStatisticsFilter->SetMeanField(GetParameterString("meanfield"));
    if (IsParameterEnabled("covfield") && HasValue("covfield"))
      SpectralStatisticsFilter->SetCovField(GetParameterString("covfield"));
    if (IsParameterEnabled("nbpixelfield") && HasValue("nbpixelfield"))
      SpectralStatisticsFilter->SetNbPixelsField(GetParameterString("nbpixelfield"));

    
    SpectralStatisticsFilter->SetOGRData(vectors);
    SpectralStatisticsFilter->SetOutputSamples(output);
    SpectralStatisticsFilter->SetFieldName(field_name);
    SpectralStatisticsFilter->Update();
    Timer.Stop();
    otbAppLogINFO( "Elapsed: "<< float(Timer.GetElapsedMilliseconds())/1000 <<" seconds.");
  }
  
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputePolygonsSpectralFeatures)
