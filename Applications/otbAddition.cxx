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

class Addition : public Application
{
public:
  /** Standard class typedefs. */
  typedef Addition                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Addition, otb::Application);

private:
  Addition()
  {
    SetName("Addition");
    SetDescription("This is a simple application which adds two numbers and print the sum on standard output");
  }

  virtual ~Addition()
  {
  }

  void DoCreateParameters()
  {
    std::cout << "Addition::DoCreateParameters" << std::endl;
    AddParameter(ParameterType_Float, "a", "First number");
    AddParameter(ParameterType_Float, "b", "Second number");
  }

  void DoUpdateParameters()
  {
    std::cout << "Addition::DoUpdateParameters" << std::endl;
  }

  void DoExecute()
  {
    std::cout << "Addition::DoExecute" << std::endl;
    GetLogger()->Debug("debug message");
    GetLogger()->Error("error message");
    std::cout << GetParameterFloat("a") + GetParameterFloat("b") << std::endl;
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Addition)
