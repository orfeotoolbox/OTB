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

#ifndef otbImageListToImageListFilter_hxx
#define otbImageListToImageListFilter_hxx

#include "otbImageListToImageListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
ImageListToImageListFilter<TInputImage, TOutputImage>
::ImageListToImageListFilter()
{
  this->SetNumberOfRequiredInputs(1);
}
/**
 * Input Connection
 * \param imageList The input image list.
 */
template <class TInputImage, class TOutputImage>
void
ImageListToImageListFilter<TInputImage, TOutputImage>
::SetInput(const InputImageListType *imageList)
{
  // A single input image
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputImageListType*>(imageList));
}
/**
 * Input image retrieval
 * \return The input image.
 */
template <class TInputImage, class TOutputImage>
typename ImageListToImageListFilter<TInputImage, TOutputImage>::InputImageListType *
ImageListToImageListFilter<TInputImage, TOutputImage>
::GetInput(void)
{
  // If there is no input
  if (this->GetNumberOfInputs() < 1)
    {
    // exit
    return nullptr;
    }
  // else return the first input
  return static_cast<InputImageListType *>
           (this->itk::ProcessObject::GetInput(0));
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
ImageListToImageListFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
