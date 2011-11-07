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
  ConcatenateVectorData()
  {
    SetName("ConcatenateVectorData");
    SetDescription("Concatenate VectorDatas");

//     SetDocName("Rescale Image Application");
//     SetDocLongDescription("This application scale the given image pixel intensity between two given values. "
//                           "By default min (resp. max) value is set to 0 (resp. 255).");
//     SetDocLimitations("None");
//     SetDocAuthors("OTB-Team");
//     SetDocSeeAlso(" ");
//     SetDocCLExample("otbApplicationLauncherCommandLine Rescale ${OTB-BIN}/bin"
//                     " --in ${OTB-DATA}/Input/poupees.tif --out rescaledImage.tif --outmin 20 --outmax 150");
//     AddDocTag("Image Manipulation");
  }

  virtual ~ConcatenateVectorData()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputVectorDataList, "vd", "Input VectorDatas to concatenate");
    SetParameterDescription("vd", "Vector Data of sample used to validate the estimator.");

    AddParameter(ParameterType_OutputVectorData, "out", "Concatenated VectorData");
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
