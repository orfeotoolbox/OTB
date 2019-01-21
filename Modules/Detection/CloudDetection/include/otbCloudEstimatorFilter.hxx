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

#ifndef otbCloudEstimatorFilter_hxx
#define otbCloudEstimatorFilter_hxx

#include "otbCloudEstimatorFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::CloudEstimatorFilter()
{
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::BeforeThreadedGenerateData()
{
  unsigned int ReferencePixelNumberOfBands = 0;
  ReferencePixelNumberOfBands = this->GetReferencePixel().GetSize();

  if (ReferencePixelNumberOfBands != this->GetInput()->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(
      "The number of bands of the reference pixel is different from the number of bands of the input image. ");
    }
}

/**
 * SetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::SetVariance(double var)
{
  this->GetFunctor().SetVariance(var);
  this->Modified();
}

/**
 * GetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
double
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::GetVariance() const
{
  return this->GetFunctor().GetVariance();
}

/**
 * SetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
void
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::SetReferencePixel(InputPixelType ref)
{
  this->GetFunctor().SetReferencePixel(ref);
  this->Modified();
}

/**
 * GetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
typename CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>::InputPixelType
CloudEstimatorFilter<TInputImage, TOutputImage, TFunction>
::GetReferencePixel() const
{
  return this->GetFunctor().GetReferencePixel();
}

}

#endif
