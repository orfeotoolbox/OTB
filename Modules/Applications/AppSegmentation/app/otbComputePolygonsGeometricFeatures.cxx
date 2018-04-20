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

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbMacro.h"

#include "otbStopwatch.h"

#include "otbOGRDataToPolygonGeometricFeaturesFilter.h"

namespace otb
{
namespace Wrapper
{

class ComputePolygonsGeometricFeatures : public Application
{
public:
  /** Standard class typedefs. */
  typedef ComputePolygonsGeometricFeatures                        Self;
  typedef Application                                             Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;
  
  typedef otb::ogr::DataSource                                    OGRDataSourceType;
  typedef otb::ogr::Layer                                         OGRLayerType;
  typedef otb::ogr::Feature                                       OGRFeatureType;
  
  typedef OGRDataToPolygonGeometricFeaturesFilter                         OGRDataToPolygonGeometricFeaturesFilterType;
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputePolygonsGeometricFeatures, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("ComputePolygonsGeometricFeatures");
    SetDescription("This application computes geometric features on the input vector data.");

    // Documentation
    SetDocName("Compute Polygons Geometric Features");
    SetDocLongDescription("This application computes geometric features on the input vector data.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    AddParameter(ParameterType_InputFilename, "in", "Name of the input vector data");
    SetParameterDescription("in","The input vector data.");
    
    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file containing class labels");
    SetParameterDescription("out","Output vector data file storing sample values (OGR format)."
      "If not given, the input vector data file is updated.");
    MandatoryOff("out");
    
    AddParameter(ParameterType_String, "sizefield", "Size field name");
    SetParameterDescription("sizefield", "Name of the field containing polygon size");
   
    AddParameter(ParameterType_String, "perimeterfield", "Perimeter field name");
    SetParameterDescription("perimeterfield", "Name of the field containing polygon perimeter");
   
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "vectorData.shp");
    SetDocExampleParameterValue("out", "features.sqlite");
    SetDocExampleParameterValue("sizefield","size");
    SetDocExampleParameterValue("perimeterfield","perimeter");
    
    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  { 
    OGRDataSourceType::Pointer vectors;
    OGRDataSourceType::Pointer output;
    if (IsParameterEnabled("out") && HasValue("out"))
    {
      vectors = ogr::DataSource::New(this->GetParameterString("in"));
      output = ogr::DataSource::New(this->GetParameterString("out"),
                                    ogr::DataSource::Modes::Overwrite);
    }
    else
    {
      // Update mode
      vectors = ogr::DataSource::New(this->GetParameterString("in"),
                                    ogr::DataSource::Modes::Update_LayerUpdate);
      output = vectors;
    }
    // With a threaded filter
    auto GeometricFeaturesFilter = OGRDataToPolygonGeometricFeaturesFilterType::New();
    GeometricFeaturesFilter->SetSizeField( this->GetParameterString("sizefield"));
    GeometricFeaturesFilter->SetPerimeterField( this->GetParameterString("perimeterfield"));
    GeometricFeaturesFilter->SetInput(vectors);
    GeometricFeaturesFilter->SetOutput(output);
    GeometricFeaturesFilter->Update();
  }
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputePolygonsGeometricFeatures)
