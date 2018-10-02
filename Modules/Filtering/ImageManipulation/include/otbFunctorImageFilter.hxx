/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbFunctorImageFilter_hxx
#define otbFunctorImageFilter_hxx

#include "otbFunctorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TFunction>
FunctorImageFilter<TFunction>::FunctorImageFilter()
{
  //  this->SetNumberOfRequiredInputs(m_Functor.GetNumberOfInputs());
  //  this->InPlaceOff();
}

template <class TFunction>
typename FunctorImageFilter<TFunction>::Pointer 
FunctorImageFilter<TFunction>::New(const TFunction& f) 
{
  Pointer p = new FunctorImageFilter<TFunction>(f);
  return p;                                           
}

template <class TFunction>
void
FunctorImageFilter<TFunction>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TFunction>
void
FunctorImageFilter<TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  (void) outputRegionForThread;
  (void) threadId;
}

} // end namespace otb

#endif
