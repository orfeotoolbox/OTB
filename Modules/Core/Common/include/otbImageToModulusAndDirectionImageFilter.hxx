/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageToModulusAndDirectionImageFilter_hxx
#define otbImageToModulusAndDirectionImageFilter_hxx

#include "otbImageToModulusAndDirectionImageFilter.h"
#include "itkProcessObject.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::ImageToModulusAndDirectionImageFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(2);
  this->SetNumberOfRequiredOutputs(2);

  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageDirectionType::New());
}

/** Return the const output image modulus */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
const typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::OutputImageType*
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::GetOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return 0;
  }
  return static_cast<const OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
}
/** Return the output image modulus */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::OutputImageType*
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return nullptr;
  }
  return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
}

/** Return the const output image direction */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
const typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::OutputImageDirectionType*
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::GetOutputDirection() const
{
  if (this->GetNumberOfOutputs() < 2)
  {
    return 0;
  }
  return static_cast<const OutputImageDirectionType*>(this->itk::ProcessObject::GetOutput(1));
}

/** Return the output image direction */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
typename ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::OutputImageDirectionType*
ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::GetOutputDirection()
{
  if (this->GetNumberOfOutputs() < 2)
  {
    return nullptr;
  }
  return static_cast<OutputImageDirectionType*>(this->itk::ProcessObject::GetOutput(1));
}

/**
 * Standard "GenerateInputRequestedRegion" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
