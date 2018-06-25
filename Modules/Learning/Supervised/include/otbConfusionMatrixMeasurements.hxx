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

#ifndef otbConfusionMatrixMeasurements_hxx
#define otbConfusionMatrixMeasurements_hxx

#include "otbConfusionMatrixMeasurements.h"

namespace otb
{
template<class TConfusionMatrix, class TLabel>
ConfusionMatrixMeasurements<TConfusionMatrix, TLabel>
::ConfusionMatrixMeasurements() :
  m_KappaIndex(0.0),
  m_OverallAccuracy(0.0),
  m_FalseNegativeValue(0.0),
  m_TrueNegativeValue(0.0),
  m_FalsePositiveValue(0.0),
  m_TruePositiveValue(0.0),
  m_NumberOfClasses(0),
  m_NumberOfSamples(0)

{
  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses, m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);
}


template <class TConfusionMatrix, class TLabel>
void
ConfusionMatrixMeasurements<TConfusionMatrix, TLabel>
::Compute()
{
  m_NumberOfClasses = m_ConfusionMatrix.Rows();

  m_FalseNegativeValues = MeasurementType(m_NumberOfClasses);
  m_TrueNegativeValues = MeasurementType(m_NumberOfClasses);
  m_FalsePositiveValues = MeasurementType(m_NumberOfClasses);
  m_TruePositiveValues = MeasurementType(m_NumberOfClasses);
  m_FalseNegativeValues.Fill(0);
  m_FalsePositiveValues.Fill(0);
  m_TruePositiveValues.Fill(0);
  m_TrueNegativeValues.Fill(0);

  const double epsilon = 0.0000000001;
  double luckyRate = 0.;
  this->m_NumberOfSamples = 0;
  this->m_OverallAccuracy  = 0.;
  for (unsigned int i = 0; i < m_NumberOfClasses; ++i)
    {
    double sum_along_row_i = 0.;
    double sum_along_column_i = 0.;
    for (unsigned int j = 0; j < m_NumberOfClasses; ++j)
      {
      sum_along_row_i += m_ConfusionMatrix(i, j);
      sum_along_column_i += m_ConfusionMatrix(j, i);
      this->m_NumberOfSamples += m_ConfusionMatrix(i, j);
      // i != j
      if (i != j)
        {
        this->m_FalseNegativeValues[i] += m_ConfusionMatrix(i, j);
        this->m_FalsePositiveValues[i] += m_ConfusionMatrix(j, i);
        }
      // i == j
      else
        {
        this->m_OverallAccuracy += m_ConfusionMatrix(i, i);
        this->m_TruePositiveValues[i] = m_ConfusionMatrix(i, i);
        }
      }
    luckyRate += sum_along_row_i * sum_along_column_i;
    }

  this->m_TrueNegativeValues.Fill(m_NumberOfSamples);
  this->m_TrueNegativeValues -= this->m_FalseNegativeValues
                              + this->m_FalsePositiveValues
                              + this->m_TruePositiveValues;

  if (m_NumberOfClasses == 2)
    {
    this->m_TruePositiveValue = m_ConfusionMatrix(0, 0);
    this->m_FalseNegativeValue = m_ConfusionMatrix(0, 1);
    this->m_FalsePositiveValue = m_ConfusionMatrix(1, 0);
    this->m_TrueNegativeValue = m_ConfusionMatrix(1, 1);
    /*std::cout << "TP= " << this->m_TruePositiveValue << std::endl;
    std::cout << "FN= " << this->m_FalseNegativeValue << std::endl;
    std::cout << "FP= " << this->m_FalsePositiveValue << std::endl;
    std::cout << "TN= " << this->m_TrueNegativeValue << std::endl; */
    }
  else
    {
    /*std::cout << "TP= " << this->m_TruePositiveValues << std::endl;
    std::cout << "FN= " << this->m_FalseNegativeValues << std::endl;
    std::cout << "FP= " << this->m_FalsePositiveValues << std::endl;
    std::cout << "TN= " << this->m_TrueNegativeValues << std::endl; */
    }

  m_Precisions = MeasurementType(m_NumberOfClasses);
  m_Recalls = MeasurementType(m_NumberOfClasses);
  m_FScores = MeasurementType(m_NumberOfClasses);
  m_Precisions.Fill(0.);
  m_Recalls.Fill(0.);
  m_FScores.Fill(0.);

  if (m_NumberOfSamples > 0)
    {
    this->m_OverallAccuracy /= static_cast<double>(m_NumberOfSamples);
    luckyRate /= vcl_pow(m_NumberOfSamples, 2.0);

    if (vcl_abs(1 - luckyRate) > epsilon)
      {
      m_KappaIndex = (m_OverallAccuracy - luckyRate) / (1 - luckyRate);
      }
    else
      {
      this->m_KappaIndex = 1.;
      }
    }
  else
    {
    this->m_OverallAccuracy  = 0.;
    this->m_KappaIndex = 0.;
    // no samples, no need to continue
    return;
    }

  for (unsigned int i = 0; i < m_NumberOfClasses; ++i)
    {
    if (vcl_abs(this->m_TruePositiveValues[i] + this->m_FalsePositiveValues[i]) > epsilon)
      {
      this->m_Precisions[i] = this->m_TruePositiveValues[i] / (this->m_TruePositiveValues[i]
          + this->m_FalsePositiveValues[i]);
      }

    if (vcl_abs(this->m_TruePositiveValues[i] + this->m_FalseNegativeValues[i]) > epsilon)
      {
      this->m_Recalls[i] = this->m_TruePositiveValues[i] / (this->m_TruePositiveValues[i]
          + this->m_FalseNegativeValues[i]);
      }

    if (vcl_abs(this->m_Recalls[i] + this->m_Precisions[i]) > epsilon)
      {
      this->m_FScores[i] = 2 * this->m_Recalls[i] * this->m_Precisions[i]
          / (this->m_Recalls[i] + this->m_Precisions[i]);
      }
    }


  if (m_NumberOfClasses == 2)
    {
    if (vcl_abs(this->m_TruePositiveValue + this->m_FalsePositiveValue) > epsilon)
      {
      this->m_Precision = this->m_TruePositiveValue / (this->m_TruePositiveValue + this->m_FalsePositiveValue);
      }
    if (vcl_abs(this->m_TruePositiveValue + this->m_FalseNegativeValue) > epsilon)
      {
      this->m_Recall = this->m_TruePositiveValue / (this->m_TruePositiveValue + this->m_FalseNegativeValue);
      }
    if (vcl_abs(this->m_Recall + this->m_Precision) > epsilon)
      {
      this->m_FScore = 2 * this->m_Recall * this->m_Precision / (this->m_Recall + this->m_Precision);
      }
    }
}

/*
template <class TConfusionMatrix, class TLabel>
void
ConfusionMatrixMeasurements<TConfusionMatrix, TLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "TODO";
}*/

}

#endif
