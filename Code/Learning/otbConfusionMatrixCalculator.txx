/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbConfusionMatrixCalculator_txx
#define __otbConfusionMatrixCalculator_txx

#include <algorithm>
#include "otbMacro.h"

namespace otb
{
template<class TRefListLabel, class TProdListLabel>
ConfusionMatrixCalculator<TRefListLabel, TProdListLabel>
::ConfusionMatrixCalculator() :
  m_KappaIndex(0.0),
  m_OverallAccuracy(0.0),
  m_FalseNegativeValue(0.0),
  m_TrueNegativeValue(0.0),
  m_FalsePositiveValue(0.0),
  m_TruePositiveValue(0.0),
  m_NumberOfClasses(0)

{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses, m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);
  m_ReferenceLabels = RefListLabelType::New();
  m_ProducedLabels = ProdListLabelType::New();
}

template <class TRefListLabel, class TProdListLabel>
void
ConfusionMatrixCalculator<TRefListLabel, TProdListLabel>
::Update()
{
  this->GenerateData();
}

template <class TRefListLabel, class TProdListLabel>
void
ConfusionMatrixCalculator<TRefListLabel, TProdListLabel>
::GenerateData()
{

  typename RefListLabelType::ConstIterator  refIterator = m_ReferenceLabels->Begin();
  typename ProdListLabelType::ConstIterator prodIterator = m_ProducedLabels->Begin();

  //check that both lists have the same number of samples

  if  ( (m_ReferenceLabels->Size() != m_ProducedLabels->Size()) ||
        (m_ReferenceLabels->Size() == 0 ) ||
        (m_ProducedLabels->Size() == 0 )  )
    {
    otbMsgDebugMacro(<< "refLabels size = " << m_ReferenceLabels->Size() <<
                     " / proLabels size = " << m_ProducedLabels->Size());
    throw itk::ExceptionObject(__FILE__, __LINE__, "ListSample size missmatch", ITK_LOCATION);
    }

  m_NumberOfSamples = m_ReferenceLabels->Size();

  // count the number of classes
  int countClasses = 0;
  while (refIterator != m_ReferenceLabels->End())
    {

    int currentLabel = refIterator.GetMeasurementVector()[0];
    if (m_MapOfClasses.find(currentLabel) == m_MapOfClasses.end())
      {
      m_MapOfClasses[currentLabel] = countClasses;
      ++countClasses;
      }
    ++refIterator;

    }

  m_NumberOfClasses = countClasses;

  std::vector<long int> samplesPerClass;

  for (unsigned int i = 0; i < m_NumberOfClasses; ++i)
    samplesPerClass.push_back(0);

  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses, m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);

  m_FalseNegativeValues = MeasurementType(m_NumberOfClasses);
  m_TrueNegativeValues = MeasurementType(m_NumberOfClasses);
  m_FalsePositiveValues = MeasurementType(m_NumberOfClasses);
  m_TruePositiveValues = MeasurementType(m_NumberOfClasses);
  m_FalseNegativeValues.Fill(0);
  m_FalsePositiveValues.Fill(0);
  m_TruePositiveValues.Fill(0);
  m_TrueNegativeValues.Fill(0);

  refIterator = m_ReferenceLabels->Begin();
  prodIterator = m_ProducedLabels->Begin();

  while (refIterator != m_ReferenceLabels->End())
    {
    int refLabel = refIterator.GetMeasurementVector()[0];
    int prodLabel = prodIterator.GetMeasurementVector()[0];

    int refPos = m_MapOfClasses[refLabel];
    int prodPos = m_MapOfClasses[prodLabel];

    ++samplesPerClass[refPos];
    m_ConfusionMatrix(prodPos, refPos) += 1;

    ++refIterator;
    ++prodIterator;

    }

  this->m_OverallAccuracy  = 0.;
  for (unsigned int i = 0; i < m_NumberOfClasses; ++i)
    {
    this->m_OverallAccuracy += m_ConfusionMatrix(i, i);
    this->m_TruePositiveValues[i] = m_ConfusionMatrix(i, i);
    }
  if (m_NumberOfClasses == 2)
    {
    this->m_TruePositiveValue = this->m_TruePositiveValues[0];
    }

  this->m_OverallAccuracy /= static_cast<double>(m_NumberOfSamples);


  double luckyRate = 0.;
  for (unsigned int i = 0; i < m_NumberOfClasses; ++i)
    {
    double sum_ij = 0.;
    double sum_ji = 0.;
    for (unsigned int j = 0; j < m_NumberOfClasses; ++j)
      {
      sum_ij += m_ConfusionMatrix(i, j);
      sum_ji += m_ConfusionMatrix(j, i);
      if (i != j)
        {
        this->m_FalseNegativeValues[i] += m_ConfusionMatrix(i, j);
        this->m_FalsePositiveValues[i] += m_ConfusionMatrix(j, i);
        }
      }
    luckyRate += sum_ij * sum_ji;
    this->m_TrueNegativeValues[i] = m_NumberOfSamples;
    }
  this->m_TrueNegativeValues -= this->m_FalseNegativeValues
                              + this->m_FalsePositiveValues
                              + this->m_TruePositiveValues;

  if (m_NumberOfClasses == 2)
    {
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

  const double epsilon = 0.0000000001;

  if (m_NumberOfClasses != 2)
    {
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
    }
  else
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

  luckyRate /= vcl_pow(m_NumberOfSamples, 2.0);

  if (vcl_abs(luckyRate-1) > epsilon)
    {
    m_KappaIndex = (m_OverallAccuracy - luckyRate) / (1 - luckyRate);
    }
}

template <class TRefListLabel, class TProdListLabel>
void
ConfusionMatrixCalculator<TRefListLabel, TProdListLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "TODO";
}

}

#endif
