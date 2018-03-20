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
  typedef ComputePolygonsGeometricFeatures                       Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  typedef otb::ogr::DataSource                            OGRDataSourceType;
  typedef otb::ogr::Layer                                 OGRLayerType;
  typedef otb::ogr::Feature                               OGRFeatureType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ComputePolygonsGeometricFeatures, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("ComputePolygonsGeometricFeatures");
    SetDescription("This application performs the vectorization of an input binary mask");

    // Documentation
    SetDocName("Compute Polygons Geometric Features");
    SetDocLongDescription("Given an input binary raster image, this application will output a vector data file containing a polygon for each connected component of the input raster. Additionnaly, the size of each polygon will be computed and added to the raster");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    AddParameter(ParameterType_InputFilename, "in", "Name of the input vector data");
    SetParameterDescription("in","The input vector data.");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file containing class labels");
    SetParameterDescription("out","Output vector data file storing sample values (OGR format)."
      "If not given, the input vector data file is updated.");
    MandatoryOff("out");

    AddRAMParameter();

    // Default values

    // Doc example parameter settings

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
    
    otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Read);
    otb::ogr::Layer layer = source->GetLayer(0);
    
    //otb::ogr::Feature firstFeature = layerTmp.ogr().GetNextFeature();
    
    ogr::DataSource::Pointer output;
    bool updateMode = false;
    // Create new OGRDataSource
    output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
    otb::ogr::Layer newLayer = output->CreateLayer(
    GetParameterString("out"),
    const_cast<OGRSpatialReference*>(layer.GetSpatialRef()),
    layer.GetGeomType());
    // Copy existing fields
    OGRFeatureDefn &inLayerDefn = layer.GetLayerDefn();
    for (int k=0 ; k<inLayerDefn.GetFieldCount() ; k++)
    {
      OGRFieldDefn fieldDefn(inLayerDefn.GetFieldDefn(k));
      newLayer.CreateField(fieldDefn);
    }
    
    otb::ogr::Layer::iterator featItOut = newLayer.begin();
    
    
    otb::ogr::Layer::iterator featIt;
    
    OGRFieldDefn fieldSize("size", OFTReal);
    newLayer.CreateField(fieldSize, true);
    
    OGRFieldDefn fieldPerimeter("perimeter", OFTReal);
    newLayer.CreateField(fieldPerimeter, true);
    
    for(featIt = layer.begin(); featIt!=layer.end(); ++featIt)
    {
      
      double area = static_cast<const OGRPolygon *>((*featIt).GetGeometry())->get_Area();
      double perimeter = static_cast<const OGRPolygon *>((*featIt).GetGeometry())->getExteriorRing()->get_Length();
     /*(*featItOut)["size"].SetValue<double>(area);
      featItOut++;*/
      ogr::Feature dstFeature(newLayer.GetLayerDefn());
      dstFeature.SetFrom( *featIt , TRUE);
      dstFeature.SetFID(featIt->GetFID());
      dstFeature["size"].SetValue<double>(area);
      dstFeature["perimeter"].SetValue<double>(perimeter);
      newLayer.CreateFeature( dstFeature);
    }
    newLayer.ogr().CommitTransaction();
    
    
    /*

    
    else
    {
      // Update mode
      updateMode = true;
      otbAppLogINFO("Update input vector data.");
      // fill temporary buffer for the transfer
      otb::ogr::Layer inputLayer = layer;
      
      // close input data source
      source->Clear();
      // Re-open input data source in update mode
      //output = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Update_LayerUpdate);
    }*/
  }
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputePolygonsGeometricFeatures)
