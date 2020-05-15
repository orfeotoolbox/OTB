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


#ifndef otbHistogramStatisticsFunction_hxx
#define otbHistogramStatisticsFunction_hxx

#include "otbHistogramStatisticsFunction.h"

namespace otb
{

template <class TInputHistogram, class TOutput>
HistogramStatisticsFunction<TInputHistogram, TOutput>::HistogramStatisticsFunction()
{
  m_IsModified = true;
}

template <class TInputHistogram, class TOutput>
typename HistogramStatisticsFunction<TInputHistogram, TOutput>::OutputType HistogramStatisticsFunction<TInputHistogram, TOutput>::GetEntropy()
{
  if (m_IsModified == true)
  {
    this->Update();
  }
  return m_entropy;
}

template <class TInputHistogram, class TOutput>
typename HistogramStatisticsFunction<TInputHistogram, TOutput>::OutputType HistogramStatisticsFunction<TInputHistogram, TOutput>::GetMean()
{
  if (m_IsModified == true)
  {
    this->Update();
  }
  return m_mean;
}

template <class TInputHistogram, class TOutput>
typename HistogramStatisticsFunction<TInputHistogram, TOutput>::OutputType HistogramStatisticsFunction<TInputHistogram, TOutput>::GetCovariance()
{
  if (m_IsModified == true)
  {
    this->Update();
  }
  return m_covariance;
}

template <class TInputHistogram, class TOutput>
void HistogramStatisticsFunction<TInputHistogram, TOutput>::CalculateEntropy()
{
  typename TInputHistogram::ConstPointer histogram = m_InputHistogram;

  typename TInputHistogram::ConstIterator iter = histogram->Begin();
  typename TInputHistogram::ConstIterator end  = histogram->End();

  RealType      entropy         = itk::NumericTraits<RealType>::Zero;
  FrequencyType globalFrequency = histogram->GetTotalFrequency();
  if (globalFrequency == 0)
  {
    itkExceptionMacro(<< "Histogram must contain at least 1 element.");
  }
  while (iter != end)
  {
    RealType Proba = static_cast<RealType>(iter.GetFrequency());
    Proba /= static_cast<RealType>(globalFrequency);
    if (Proba != 0.0)
    {
      entropy -= Proba * std::log(Proba);
    }
    ++iter;
  }
  m_entropy.resize(1);
  m_entropy[0] = static_cast<TOutput>(entropy);
}

template <class TInputHistogram, class TOutput>
void HistogramStatisticsFunction<TInputHistogram, TOutput>::CalculateMean()
{
  typename TInputHistogram::ConstPointer histogram = m_InputHistogram;

  unsigned int NumberOfDimension = histogram->GetSize().GetSize();
  m_mean.resize(NumberOfDimension);

  if (histogram->GetTotalFrequency() == 0)
  {
    itkExceptionMacro(<< "Histogram must contain at least 1 element.");
  }

  if (NumberOfDimension > 2)
  {
    itkExceptionMacro(<< "Histogram must have 1 or 2 dimension.");
  }

  for (unsigned int noDim = 0; noDim < NumberOfDimension; noDim++)
  {
    MeasurementType mean = itk::NumericTraits<MeasurementType>::Zero;
    for (unsigned int i = 0; i < histogram->GetSize()[noDim]; ++i)
    {
      MeasurementType val  = histogram->GetMeasurement(i, noDim);
      FrequencyType   freq = histogram->GetFrequency(i, noDim);
      mean += val * freq;
    }
    mean /= histogram->GetTotalFrequency();
    m_mean[noDim] = static_cast<TOutput>(mean);
  }
}

template <class TInputHistogram, class TOutput>
void HistogramStatisticsFunction<TInputHistogram, TOutput>::CalculateCovariance()
{
  CalculateMean();

  typename TInputHistogram::ConstPointer histogram = m_InputHistogram;

  unsigned int NumberOfDimension = histogram->GetSize().GetSize();
  m_covariance.resize(NumberOfDimension * NumberOfDimension);

  if (histogram->GetTotalFrequency() == 0)
  {
    itkExceptionMacro(<< "Histogram must contain at least 1 element.");
  }

  for (unsigned int noDimX = 0; noDimX < NumberOfDimension; noDimX++)
    for (unsigned int noDimY = 0; noDimY < NumberOfDimension; noDimY++)
    {
      MeasurementType covariance = itk::NumericTraits<MeasurementType>::Zero;
      for (unsigned int i = 0; i < histogram->GetSize()[noDimX]; ++i)
        for (unsigned int j = 0; j < histogram->GetSize()[noDimY]; ++j)
        {
          MeasurementType valX  = histogram->GetMeasurement(i, noDimX);
          MeasurementType valY  = histogram->GetMeasurement(j, noDimY);
          FrequencyType   freqX = histogram->GetFrequency(i, noDimX);
          FrequencyType   freqY = histogram->GetFrequency(j, noDimY);

          valX -= static_cast<MeasurementType>(m_mean[noDimX]);
          valY -= static_cast<MeasurementType>(m_mean[noDimY]);
          covariance += ((valX * freqX) * (valY * freqY));
        }
      covariance /= histogram->GetTotalFrequency();
      m_covariance[noDimX * NumberOfDimension + noDimY] = static_cast<TOutput>(covariance);
    }
}

template <class TInputHistogram, class TOutput>
void HistogramStatisticsFunction<TInputHistogram, TOutput>::GenerateData()
{

  CalculateEntropy();
  CalculateMean();
  CalculateCovariance();
  m_IsModified = false;
}

template <class TInputHistogram, class TOutput>
void HistogramStatisticsFunction<TInputHistogram, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
