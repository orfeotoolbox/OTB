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
#ifndef otbSharkKMeansMachineLearningModelFactory_txx
#define otbSharkKMeansMachineLearningModelFactory_txx

#include "otbSharkKMeansMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbSharkKMeansMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue>
::SharkKMeansMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("otbSharkKMeansMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "Shark KMeans Machine Learning Model",
                         1,
                         itk::CreateObjectFunction<SharkKMeansMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue>
::~SharkKMeansMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
SharkKMeansMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "Shark KMeans unsupervised machine learning model factory";
}

} // end namespace otb

#endif
