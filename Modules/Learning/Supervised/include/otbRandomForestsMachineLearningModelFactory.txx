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
#include "otbRandomForestsMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue>
::RandomForestsMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("otbRandomForestsMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "RF ML Model",
                         1,
                         itk::CreateObjectFunction<RandomForestsMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue>
::~RandomForestsMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
RandomForestsMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "Random Forest machine learning model factory";
}

} // end namespace otb
