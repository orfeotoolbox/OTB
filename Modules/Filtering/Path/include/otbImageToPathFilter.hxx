/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToPathFilter_hxx
#define otbImageToPathFilter_hxx

#include "otbImageToPathFilter.h"

namespace otb
{
/**
 * Constructor.
 */
template <class TInputImage, class TOutputPath>
ImageToPathFilter<TInputImage, TOutputPath>::ImageToPathFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
/**
 * Input image setter.
 */
template <class TInputImage, class TOutputPath>
void ImageToPathFilter<TInputImage, TOutputPath>::SetInput(const InputImageType* image)
{
  this->ProcessObjectType::SetNthInput(0, const_cast<InputImageType*>(image));
}
/**
 * Input image getter.
 */
template <class TInputImage, class TOutputPath>
const typename ImageToPathFilter<TInputImage, TOutputPath>::InputImageType* ImageToPathFilter<TInputImage, TOutputPath>::GetInput(void)
{
  return static_cast<const TInputImage*>(this->ProcessObjectType::GetInput(0));
}
/**
 * PrintSelf Method.
 */
template <class TInputImage, class TOutputPath>
void ImageToPathFilter<TInputImage, TOutputPath>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
