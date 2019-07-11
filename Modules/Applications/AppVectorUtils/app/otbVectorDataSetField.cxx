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


#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbVectorData.h"

namespace otb
{
namespace Wrapper
{

class VectorDataSetField : public Application
{
public:
  /** Standard class typedefs. */
  typedef VectorDataSetField            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataSetField, otb::Application);

private:
  void DoInit() override
  {
    SetName("VectorDataSetField");
    SetDescription("Set a field in vector data.");

    // Documentation
    SetDocLongDescription("Set a specified field to a specified value on all features of a vector data.");
    SetDocLimitations("Doesn't work with KML files yet");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputVectorData, "in", "Input");
    SetParameterDescription("in", "Input Vector Data");
    AddParameter(ParameterType_OutputVectorData, "out", "Output");
    SetParameterDescription("out", "Output Vector Data");

    AddParameter(ParameterType_String, "fn", "Field");
    SetParameterDescription("fn", "Field name");
    AddParameter(ParameterType_String, "fv", "Value");
    SetParameterDescription("fv", "Field value");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract_classification.shp");
    SetDocExampleParameterValue("out", "VectorDataSetField.shp");
    SetDocExampleParameterValue("fn", "Info");
    SetDocExampleParameterValue("fv", "Sample polygon");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do (for now)
  }

  void DoExecute() override
  {
    m_InputData = GetParameterVectorData("in");

    typedef VectorDataType::DataTreeType            DataTreeType;
    typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;
    TreeIteratorType it(m_InputData->GetDataTree());

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
      it.Get()->SetFieldAsString(GetParameterAsString("fn"), GetParameterAsString("fv"));
    }

    SetParameterOutputVectorData("out", m_InputData);

  }

  VectorDataType::Pointer m_InputData;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataSetField)
