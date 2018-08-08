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

#ifndef otbSinclairImageFilter_hxx
#define otbSinclairImageFilter_hxx

#include "otbSinclairImageFilter.h"
#include "otbMath.h"

namespace otb
{


/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputHH(const TInputImageHH * image)
{
  OTB_DISABLE_DYNAMIC_MT;
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputHV(const TInputImageHV * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputVH(const TInputImageVH * image)
{
  this->SetInput3(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::SetInputVV(const TInputImageVV * image)
{
  this->SetInput4(image);
}

/**
 * Printself
 */
template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <class TInputImageHH, class TInputImageHV, class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction>
void
SinclairImageFilter<TInputImageHH, TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction>
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
