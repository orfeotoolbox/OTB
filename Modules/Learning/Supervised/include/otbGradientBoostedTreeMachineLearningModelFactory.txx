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
#include "otbGradientBoostedTreeMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbGradientBoostedTreeMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::GradientBoostedTreeMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("otbGradientBoostedTreeMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Gradient Boosted Tree ML Model",
                         1,
                         itk::CreateObjectFunction<GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::~GradientBoostedTreeMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
GradientBoostedTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "Gradient Boosted Tree machine learning model factory";
}

} // end namespace otb
