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

#ifndef otbSinclairReciprocalImageFilter_txx
#define otbSinclairReciprocalImageFilter_txx

#include "otbSinclairReciprocalImageFilter.h"
#include "otbMath.h"

namespace otb
{

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputHH(const TInputImageHH * image)
{
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputHV(const TInputImageHV_VH * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputVH(const TInputImageHV_VH * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputHV_VH(const TInputImageHV_VH * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::SetInputVV(const TInputImageVV * image)
{
  this->SetInput3(image);
}

/**
 * Printself
 */
template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <class TInputImageHH, class TInputImageHV_VH, class TInputImageVV, class TOutputImage, class TFunction>
void
SinclairReciprocalImageFilter<TInputImageHH, TInputImageHV_VH, TInputImageVV, TOutputImage, TFunction>
::GenerateOutputInformation()
{
  // Call to the superclass implementation
  Superclass::GenerateOutputInformation();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  // initialize the number of channels of the output image
  outputPtr->SetNumberOfComponentsPerPixel( this->GetFunctor().GetNumberOfComponentsPerPixel() );

}

} // end namespace otb

#endif
