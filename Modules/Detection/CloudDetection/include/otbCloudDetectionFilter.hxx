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

#ifndef otbCloudDetectionFilter_hxx
#define otbCloudDetectionFilter_hxx

#include "otbCloudDetectionFilter.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction>
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::CloudDetectionFilter()
{
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction>
void CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TFunction>
void CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::BeforeThreadedGenerateData()
{
  unsigned int ReferencePixelNumberOfBands = 0;
  ReferencePixelNumberOfBands              = this->GetReferencePixel().GetSize();

  if (ReferencePixelNumberOfBands != this->GetInput()->GetNumberOfComponentsPerPixel())
  {
    itkExceptionMacro("The number of bands of the reference pixel is different from the number of bands of the input image. ");
  }
}

/**
 * SetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
void CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::SetVariance(double var)
{
  this->GetFunctor().SetVariance(var);
}

/**
 * GetVariance
 */
template <class TInputImage, class TOutputImage, class TFunction>
double CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::GetVariance()
{
  return this->GetFunctor().GetVariance();
}

/**
 * SetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
void CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::SetReferencePixel(InputPixelType ref)
{
  this->GetFunctor().SetReferencePixel(ref);
}

/**
 * GetReferencePixel
 */
template <class TInputImage, class TOutputImage, class TFunction>
typename CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::InputPixelType
CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::GetReferencePixel()
{
  return this->GetFunctor().GetReferencePixel();
}

/**
 * SetMinThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
void CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::SetMinThreshold(double threshold)
{
  this->GetFunctor().SetMinThreshold(threshold);
}

/**
 * SetMaxThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
void CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::SetMaxThreshold(double threshold)
{
  this->GetFunctor().SetMaxThreshold(threshold);
}

/**
 * GetMinThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
double CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::GetMinThreshold()
{
  return this->GetFunctor().GetMinThreshold();
}

/**
 * GetMaxThreshold
 */
template <class TInputImage, class TOutputImage, class TFunction>
double CloudDetectionFilter<TInputImage, TOutputImage, TFunction>::GetMaxThreshold()
{
  return this->GetFunctor().GetMaxThreshold();
}
}

#endif
