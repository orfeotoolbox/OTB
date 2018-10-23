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
{}

template <class TFunction>
typename FunctorImageFilter<TFunction>::Pointer
FunctorImageFilter<TFunction>::New(const TFunction& f, itk::Size<2> radius) 
{
  Pointer p = new FunctorImageFilter<TFunction>(f,radius);
  return p;                                           
}

template <class TFunction>
void
FunctorImageFilter<TFunction>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // Get requested region for output
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  auto requestedRegion = outputPtr->GetRequestedRegion();

  // Propagate to each variadic inputs, including possible radius
  functor_filter_details::SetInputRequestedRegions(this->GetVInputs(),requestedRegion, {{0,0}});
}

template <class TFunction>
void
FunctorImageFilter<TFunction>::GenerateOutputInformation()
{
  // Call Superclass implementation
  Superclass::GenerateOutputInformation();

  // Get All variadic inputs
  auto inputs = this->GetVInputs();

  // Retrieve an array of number of components per input
  auto inputNbComps = functor_filter_details::GetNumberOfComponentsPerInput(inputs);

  // Call the helper to set the number of components for the output image
  functor_filter_details::NumberOfOutputComponents<TFunction,OutputImageType,inputNbComps.size()>::Set(m_Functor,this->GetOutput(),inputNbComps);
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TFunction>
void
FunctorImageFilter<TFunction>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Build output iterator
  itk::ImageScanlineIterator<OutputImageType> outIt(this->GetOutput(),outputRegionForThread);
  itk::ProgressReporter p(this,threadId,outputRegionForThread.GetNumberOfPixels());

  auto inputIterators = functor_filter_details::MakeIterators(this->GetVInputs(),outputRegionForThread);
  
  while(!outIt.IsAtEnd())
    {
    for(;!outIt.IsAtEndOfLine();++outIt,functor_filter_details::MoveIterators(inputIterators))
      {
      outIt.Set(functor_filter_details::CallOperator(m_Functor,inputIterators));
      // Update progress
      p.CompletedPixel();
      }
    outIt.NextLine();
    }
}

} // end namespace otb

#endif
