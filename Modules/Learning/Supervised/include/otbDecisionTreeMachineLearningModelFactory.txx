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
#include "otbDecisionTreeMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbDecisionTreeMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::DecisionTreeMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("otbDecisionTreeMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Decision Tree ML Model",
                         1,
                         itk::CreateObjectFunction<DecisionTreeMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::~DecisionTreeMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
DecisionTreeMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "Decision Tree machine learning model factory";
}

} // end namespace otb
