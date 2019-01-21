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


#ifndef otbImageToPathListFilter_hxx
#define otbImageToPathListFilter_hxx

#include "otbImageToPathListFilter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPath>
ImageToPathListFilter<TInputImage, TOutputPath>
::ImageToPathListFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs(1);
}

/**
 *
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathListFilter<TInputImage, TOutputPath>
::SetInput(const InputImageType *image)
{
  // We have 1 input:  an image

  // Process object is not const-correct so the const_cast is required here
  this->ProcessObjectType::SetNthInput(0, const_cast<InputImageType *>(image));
}

template <class TInputImage, class TOutputPath>
const typename ImageToPathListFilter<TInputImage, TOutputPath>::InputImageType *
ImageToPathListFilter<TInputImage, TOutputPath>
::GetInput(void)
{

  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const TInputImage *>
           (this->ProcessObjectType::GetInput(0));
}

/**
 *
 */
template <class TInputImage, class TOutputPath>
void
ImageToPathListFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
