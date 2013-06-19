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
#include "otbNormalBayesMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue>
::NormalBayesMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("otbNormalBayesMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Normal Bayes ML Model",
                         1,
                         itk::CreateObjectFunction<NormalBayesMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue>
::~NormalBayesMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
NormalBayesMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "Normal Bayes machine learning model factory";
}

} // end namespace otb
