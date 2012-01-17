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

#include "otbConcatenateVectorDataFilter.h"

namespace otb
{
namespace Wrapper
{

class ConcatenateVectorData : public Application
{

public:
  /** Standard class typedefs. */
  typedef ConcatenateVectorData         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConcatenateVectorData, otb::Application);

  /** VectorData Concatenate filter*/
  typedef otb::ConcatenateVectorDataFilter<VectorDataType>  ConcatenateFilterType;

private:
  void DoInit()
  {
    SetName("ConcatenateVectorData");
    SetDescription("Concatenate VectorDatas");

    SetDocName("Concatenate");
    SetDocLongDescription("This application concatenates a list of VectorData to produce a unique VectorData as output"
                          "Note that the VectorDatas must be of the same type (Storing polygons only, lines only, or points only)");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputVectorDataList, "vd", "Input VectorDatas to concatenate");
    SetParameterDescription("vd", "VectorData files to be concatenated in an unique VectorData");

    AddParameter(ParameterType_OutputVectorData, "out", "Concatenated VectorData");

    // Doc example parameter settings
    SetDocExampleParameterValue("vd", "ToulousePoints-examples.shp ToulouseRoad-examples.shp");
    SetDocExampleParameterValue("out", "ConcatenatedVectorData.shp");
 
  }


  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute()
  {
    // Get the input VectorData list
    VectorDataListType* vectorDataList = GetParameterVectorDataList("vd");
    
    // Concatenate filter object
    m_ConcatenateFilter = ConcatenateFilterType::New();
    
    for (unsigned int idx = 0; idx < vectorDataList->Size(); idx++)
      {
      m_ConcatenateFilter->AddInput(vectorDataList->GetNthElement(idx));
      }

    // Set the output
    SetParameterOutputVectorData("out", m_ConcatenateFilter->GetOutput());
  }
  
  ConcatenateFilterType::Pointer m_ConcatenateFilter;
  };

}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::ConcatenateVectorData)
