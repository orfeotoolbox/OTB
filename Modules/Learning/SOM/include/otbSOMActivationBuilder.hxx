/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSOMActivationBuilder_hxx
#define otbSOMActivationBuilder_hxx

#include "otbSOMActivationBuilder.h"
#include "itkImageRegionIterator.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>::SOMActivationBuilder()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNthOutput(0, OutputImageType::New());
}
/**
 * Destructor.
 */
template <class TInputImage, class TInputMap, class TOutputImage>
SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>::~SOMActivationBuilder()
{
}
/**
 * Main computation method
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>::GenerateData(void)
{
  // Retrieve the inputs and output pointers
  InputMapType*          map    = const_cast<InputMapType*>(this->GetInput());
  OutputImagePointerType output = this->GetOutput();

  // output initialization
  output->SetRegions(map->GetLargestPossibleRegion());
  output->Allocate();
  output->FillBuffer(static_cast<typename OutputImageType::PixelType>(0));

  // Iterator typedef
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  typename InputMapType::IndexType                  index;
  // Iterators instantiation
  OutputIteratorType outIt(output, output->GetLargestPossibleRegion());

  // For each vector in the set
  for (typename ListSampleType::Iterator it = m_ListSample->Begin(); it != m_ListSample->End(); ++it)
  {
    // Retrieve the index of the winner
    index = map->GetWinner(it.GetMeasurementVector());
    // increment the activation map
    outIt.SetIndex(index);
    outIt.Set(outIt.Get() + 1);
  }
}
/**
 *PrintSelf method
 */
template <class TInputImage, class TInputMap, class TOutputImage>
void SOMActivationBuilder<TInputImage, TInputMap, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
