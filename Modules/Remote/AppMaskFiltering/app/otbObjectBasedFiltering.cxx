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

namespace otb
{
namespace Wrapper
{

class ObjectBasedFiltering : public Application
{
public:
  /** Standard class typedefs. */
  typedef ObjectBasedFiltering                       Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  typedef otb::ogr::DataSource                            OGRDataSourceType;
  typedef otb::ogr::Layer                                 OGRLayerType;
  typedef otb::ogr::Feature                               OGRFeatureType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ObjectBasedFiltering, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("ObjectBasedFiltering");
    SetDescription("This application filters a layer of polygons using a mathematical expression on its features");

    // Documentation
    SetDocName("ObjectBasedFiltering");
    SetDocLongDescription("This application filters a layer of polygons using a mathematical expression on its features. It applies an sql query on the first layer contained in the vector data");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    AddParameter(ParameterType_InputFilename, "in", "input vector data");
    SetParameterDescription("in","Name of the input vector data.");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data");
    SetParameterDescription("out","Output filtered vector data, if no output vector is given the input layer is overwritten");
    MandatoryOff("out");

    AddParameter(ParameterType_String, "expr", "filter expression");
    SetParameterDescription("expr","Mathemetical expression used for filtering.");

    AddRAMParameter();
    
    // Doc example parameter settings
    SetDocExampleParameterValue("expr", "size<10");
    SetDocExampleParameterValue("in", "vectorData.shp");
    SetDocExampleParameterValue("out","filtered_data.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
    // Start Timer for the application
    auto Timer = Stopwatch::StartNew();
    
    // Open Input Datasource
    auto source = otb::ogr::DataSource::New(GetParameterString("in"), otb::ogr::DataSource::Modes::Read);
    auto layerIn = source->GetLayer(0);
    std::string layerName = layerIn.GetName();
    
    auto expr = this->GetParameterString("expr");
    
    // Copy input layer and close input Datasource, so we can re-open it in overwrite mode later (if not output vector is specified)
    auto buffer = ogr::DataSource::New();
    auto layerInCpy = buffer->CopyLayer( layerIn, layerName);
    source->Clear();
    
    // Apply SQL query, we use 'WHERE NOT' to delete element verifying the filtering expression
    std::ostringstream sqloss;
    sqloss.str("");
    sqloss<<"SELECT * FROM \""<<layerName<<"\" WHERE NOT "<< expr;
    auto layerTmp=buffer->ExecuteSQL(sqloss.str().c_str(), ITK_NULLPTR, ITK_NULLPTR);
    
    // Get Ouput path (new File or Update)
    std::string outPath = "";
    
    if (IsParameterEnabled("out") && HasValue("out"))
    {  
      outPath = this->GetParameterString("out");
    }
    else
    {
      outPath = this->GetParameterString("in");
    }
    
    // Create output DataSource
    auto sourceOut = otb::ogr::DataSource::New(outPath, otb::ogr::DataSource::Modes::Overwrite);
    auto layerOut = sourceOut->CopyLayer( layerTmp, layerName);
    otbAppLogINFO("filtering done")
    
    sourceOut->SyncToDisk();
    
    Timer.Stop();
    otbAppLogINFO( "Elapsed: "<< float(Timer.GetElapsedMilliseconds())/1000 <<" seconds.");
  }
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ObjectBasedFiltering)
