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
  void DoInit()
  {
    SetName("VectorDataSetField");
    SetDescription("Set a field in vector data.");

    // Documentation
    SetDocName("Vector data set field");
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
  }

  void DoUpdateParameters()
  {
    // Nothing to do (for now)
  }
  
  void DoExecute()
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
