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

#ifndef otbRAndGAndNIRIndexImageFilter_hxx
#define otbRAndGAndNIRIndexImageFilter_hxx

#include "otbRAndGAndNIRIndexImageFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::RAndGAndNIRIndexImageFilter()
{
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::SetInputR(const TInputImageR * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetInput1(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::SetInputG(const TInputImageG * image)
{
  this->SetInput2(image);
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template <class TInputImageR, class TInputImageG, class TInputImageNIR,
    class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::SetInputNIR(const TInputImageNIR * image)
{
  this->SetInput3(image);
}

template <class TInputImageR, class TInputImageG, class TInputImageNIR, class TOutputImage, class TFunction>
void
RAndGAndNIRIndexImageFilter<TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}

#endif
