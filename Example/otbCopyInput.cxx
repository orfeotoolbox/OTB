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


namespace otb
{
namespace Wrapper
{


class CopyInput : public Application
{
public:
  /** Standard class typedefs. */
  typedef CopyInput                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(CopyInput, otb::Application);

private:
   CopyInput()
  {
    SetName("CopyInput");
    SetDescription("Copy the input image into output.");
  }

  virtual ~CopyInput()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  { 
    VectorImageType::Pointer inImage = GetParameterImage("in");
      
    SetParameterOutputImage("out", inImage);
  }
  
};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CopyInput)

