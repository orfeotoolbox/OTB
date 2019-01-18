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

#ifndef otbModulusAndDirectionImageToImageFilter_hxx
#define otbModulusAndDirectionImageToImageFilter_hxx

#include "otbModulusAndDirectionImageToImageFilter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
ModulusAndDirectionImageToImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::
ModulusAndDirectionImageToImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->SetNthOutput(0, OutputImageType::New());
}

template <class TInputImage, class TInputImageDirection, class TOutputImage>
void
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
SetInput(const InputImageType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputImageType *>(input));
}

template <class TInputImage, class TInputImageDirection, class TOutputImage>
void
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
SetInputDirection(const InputImageDirectionType *direction)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<InputImageDirectionType *>(direction));
}

/** Return the input image modulus */
template <class TInputImage, class TInputImageDirection, class TOutputImage>
const typename ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::InputImageType *
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return nullptr;
    }

  return static_cast<const TInputImage *>
           (this->itk::ProcessObject::GetInput(0));
}

/** Return the intput image direction */
template <class TInputImage, class TInputImageDirection, class TOutputImage>
const typename ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection,
    TOutputImage>::InputImageDirectionType *
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
GetInputDirection(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return nullptr;
    }

  return static_cast<const TInputImageDirection *>
           (this->itk::ProcessObject::GetInput(1));

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TInputImageDirection, class TOutputImage>
void
ModulusAndDirectionImageToImageFilter<TInputImage, TInputImageDirection, TOutputImage>::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}

} // end namespace otb

#endif
