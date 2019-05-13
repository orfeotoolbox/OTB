/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataToSamplePositionFilter.h"

namespace otb
{
namespace Wrapper
{

  

class TrainImagesRegression : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef TrainImagesRegression Self;
  typedef CompositeApplication Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  /** Standard macro */
  itkTypeMacro( TrainImagesRegression, Superclass );
  
protected:

  void AddRegressionField(const std::string &inVectorFileName,
                       const std::string &outVectorFileName,
                       const std::string &fieldName)
  {
    auto setFieldAppli = GetInternalApplication("setfield");
    setFieldAppli->SetParameterString("in", inVectorFileName);
    setFieldAppli->SetParameterString("out", outVectorFileName);

    setFieldAppli->SetParameterString("fn", fieldName);
    
    setFieldAppli->SetParameterString("fv", "0");
    setFieldAppli->ExecuteAndWriteOutput();
    
  }

  void InitIO()
  {
    AddParameter( ParameterType_Group, "io", "Input and output data" );
    SetParameterDescription( "io", "This group of parameters allows setting input and output data." );

    AddParameter( ParameterType_InputImageList, "io.il", "Input Image List" );
    SetParameterDescription( "io.il", "A list of input images." );
    MandatoryOn( "io.il" );
    
    AddParameter( ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List" );
    SetParameterDescription( "io.vd", "A list of vector data to select the training samples." );
    MandatoryOn( "io.vd" );
  }
  
  void InitSampling()
  {  
    AddApplication( "VectorDataSetField", "setfield", "Set additional vector field");
    //AddApplication( "PolygonClassStatistics", "polystat", "Polygon analysis" );
    //AddApplication( "MultiImageSamplingRate", "rates", "Sampling rates" );
    //AddApplication( "SampleSelection", "select", "Sample selection" );
    //AddApplication( "SampleExtraction", "extraction", "Sample extraction" );
    
  }
  
  void InitLearning()
  {
  }

private:

  void DoInit() override
  {
    SetName("TrainImagesRegression");
    SetDescription(" ");

    SetDocLongDescription(" ");
    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    
    AddDocTag(Tags::Learning);
    
    SetOfficialDocLink();
    
    ClearApplications();
    
    InitIO();
    InitSampling();
    InitLearning();
  }
  
  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    std::string predictorFieldName = "predict";
    
    auto fileNames = GetParameterStringList( "io.vd" );
    for (auto file: fileNames)
    {
      std::string outfile = "tmp.shp";
      AddRegressionField(file, outfile, predictorFieldName);
    }
  }
};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainImagesRegression)
