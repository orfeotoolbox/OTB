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
#include "otbAddition.h"
#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

Addition::Addition()
{
  this->SetName("Addition");
  this->SetDescription("This is a simple application which adds two numbers and print the sum on standard output");
}

Addition::~Addition()
{
}

void Addition::DoCreateParameters()
{
  std::cout << "Addition::DoCreateParameters" << std::endl;
  ParameterList* params = GetParameterList();
  otb::Wrapper::FloatParameter::Pointer a = otb::Wrapper::FloatParameter::New();
  otb::Wrapper::FloatParameter::Pointer b = otb::Wrapper::FloatParameter::New();

  params->AddParameter(a.GetPointer());
  params->AddParameter(b.GetPointer());
}

void Addition::DoUpdateParameters()
{
  std::cout << "Addition::DoUpdateParameters" << std::endl;
}

void Addition::DoExecute()
{
  std::cout << "Addition::DoExecute" << std::endl;

  ParameterList* params = GetParameterList();

  otb::Wrapper::FloatParameter* pa = dynamic_cast<otb::Wrapper::FloatParameter*>(params->GetParameter(0).GetPointer());
  boost::any avalue = pa->GetAnyValue();
  float a = boost::any_cast<float>(avalue);

  otb::Wrapper::FloatParameter* pb = dynamic_cast<otb::Wrapper::FloatParameter*>(params->GetParameter(1).GetPointer());
  boost::any bvalue = pb->GetAnyValue();
  float b = boost::any_cast<float>(bvalue);

  std::cout << a + b << std::endl;
}

}

}
