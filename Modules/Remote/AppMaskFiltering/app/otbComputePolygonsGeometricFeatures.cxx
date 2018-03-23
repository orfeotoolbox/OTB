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
    auto source = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Read);
    auto layer = source->GetLayer(0);
    
    
    // Create new OGRDataSource
    auto output = ogr::DataSource::New();
    auto buffer = ogr::DataSource::New();
    
    bool updateMode = false;
    if (IsParameterEnabled("out") && HasValue("out"))
    {
      // Create new OGRDataSource
      output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
      otb::ogr::Layer newLayer = output->CreateLayer(
        layer.GetName(),
        const_cast<OGRSpatialReference*>(layer.GetSpatialRef()),
        layer.GetGeomType());
      
      // Copy existing fields
      OGRFeatureDefn &inLayerDefn = layer.GetLayerDefn();
      for (int k=0 ; k<inLayerDefn.GetFieldCount() ; k++)
      {
        OGRFieldDefn fieldDefn(inLayerDefn.GetFieldDefn(k));
        newLayer.CreateField(fieldDefn);
      }
    }
    else
    {
      // Update mode
      updateMode = true;
      otbAppLogINFO("Update input vector data.");
      // fill temporary buffer for the transfer
      otb::ogr::Layer inputLayer = layer;
      layer = buffer->CopyLayer(inputLayer, std::string("Buffer"));
      // close input data source
      source->Clear();
      // Re-open input data source in update mode
      output = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Update_LayerUpdate);
    }
    
    auto newLayer = output->GetLayer(0);
    
    // Create the new fields
    std::string sizeField = this->GetParameterString("sizefield");
    OGRFieldDefn fieldSize(sizeField.c_str(), OFTReal);
    newLayer.CreateField(fieldSize, true);
    
    std::string perimeterField = this->GetParameterString("perimeterfield");
    OGRFieldDefn fieldPerimeter(perimeterField.c_str(), OFTReal);
    newLayer.CreateField(fieldPerimeter, true);
    
    // Fill the output layer
    for (auto && feature : layer) 
    {
      auto && geometry = feature.GetGeometry();
      assert(geometry->getGeometryType() == wkbPolygon); // otherwise get_area will crash with no explanation
      const double area = static_cast<OGRPolygon  const*>(geometry)->get_Area();
      const double perimeter = static_cast<OGRPolygon const*>(geometry)->getExteriorRing()->get_Length();
      ogr::Feature dstFeature(newLayer.GetLayerDefn());
      dstFeature.SetFrom( feature , TRUE);
      dstFeature.SetFID(feature.GetFID());
      dstFeature[sizeField].SetValue<double>(area);
      dstFeature[perimeterField].SetValue<double>(perimeter);
      if (updateMode == true)
      {
        // Update the existing feature
        newLayer.SetFeature( dstFeature);
      }
      else
      {
        // Append the new feature
        newLayer.CreateFeature( dstFeature);
      }
    }
    newLayer.ogr().CommitTransaction();
  }
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputePolygonsGeometricFeatures)
