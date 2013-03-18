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
#include "otbSVMMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbSVMMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
SVMMachineLearningModelFactory<TInputValue,TOutputValue>
::SVMMachineLearningModelFactory()
{

  static std::string classOverride = std::string("otbMachineLearningModel");
  static std::string subclass = std::string("otbSVMMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "SVM ML Model",
                         1,
                         itk::CreateObjectFunction<SVMMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
SVMMachineLearningModelFactory<TInputValue,TOutputValue>
::~SVMMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
SVMMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
SVMMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "SVM machine learning model factory";
}

} // end namespace otb
