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
#include "otbKNearestNeighborsMachineLearningModelFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "itkVersion.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue>
::KNearestNeighborsMachineLearningModelFactory()
{

  std::string classOverride = std::string("otbMachineLearningModel");
  std::string subclass = std::string("otbKNearestNeighborsMachineLearningModel");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "KNN ML Model",
                         1,
                         itk::CreateObjectFunction<KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue> >::New());
}

template <class TInputValue, class TOutputValue>
KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue>
::~KNearestNeighborsMachineLearningModelFactory()
{
}

template <class TInputValue, class TOutputValue>
const char*
KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue>
::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TInputValue, class TOutputValue>
const char*
KNearestNeighborsMachineLearningModelFactory<TInputValue,TOutputValue>
::GetDescription() const
{
  return "KNN machine learning model factory";
}

} // end namespace otb
