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

#ifndef otbNormalizeVectorImageFilter_hxx
#define otbNormalizeVectorImageFilter_hxx
#include "otbNormalizeVectorImageFilter.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
NormalizeVectorImageFilter<TInputImage, TOutputImage>::NormalizeVectorImageFilter()
{
  m_IsGivenMean   = false;
  m_IsGivenStdDev = false;

  m_UseMean   = true;
  m_UseStdDev = true;

  m_CovarianceEstimator = CovarianceEstimatorFilterType::New();
}


template <class TInputImage, class TOutputImage>
void NormalizeVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  if ((m_UseMean && !m_IsGivenMean) || (m_UseStdDev && !m_IsGivenStdDev))
  {
    m_CovarianceEstimator->SetInput(const_cast<InputImageType*>(this->GetInput()));
    m_CovarianceEstimator->Update();
  }

  if (!m_UseMean)
  {
    typename TInputImage::PixelType vector(this->GetInput()->GetNumberOfComponentsPerPixel());
    vector.Fill(itk::NumericTraits<typename TInputImage::PixelType::ValueType>::Zero);
    this->GetFunctor().SetMean(vector);
  }

  if (!m_UseStdDev)
  {
    typename TInputImage::PixelType vector(this->GetInput()->GetNumberOfComponentsPerPixel());
    vector.Fill(itk::NumericTraits<typename TInputImage::PixelType::ValueType>::One);
    this->GetFunctor().SetStdDev(vector);
  }

  if (!m_IsGivenMean)
  {
    this->GetFunctor().SetMean(m_CovarianceEstimator->GetMean());

    if (!m_IsGivenStdDev && m_UseStdDev)
    {
      typename StreamingStatisticsVectorImageFilter<InputImageType>::RealPixelType sigma(this->GetInput()->GetNumberOfComponentsPerPixel());
      for (unsigned int i = 0; i < sigma.Size(); ++i)
        sigma[i]          = std::sqrt(m_CovarianceEstimator->GetCovariance()(i, i));

      this->GetFunctor().SetStdDev(sigma);
    }
  }
}

} // end of namespace otb

#endif // otbNormalizeVectorImageFilter_hxx
