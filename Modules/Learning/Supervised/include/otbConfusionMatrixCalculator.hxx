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

#ifndef otbConfusionMatrixCalculator_hxx
#define otbConfusionMatrixCalculator_hxx

#include "otbConfusionMatrixCalculator.h"

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
  m_Precision(0.0),
  m_Recall(0.0),
  m_FScore(0.0),
  m_NumberOfClasses(0),
  m_NumberOfSamples(0)

{
  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses, m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);
  m_ConfMatMeasurements = ConfusionMatrixMeasurementsType::New();
  m_ReferenceLabels = RefListLabelType::New();
  m_ProducedLabels = ProdListLabelType::New();
}

template <class TRefListLabel, class TProdListLabel>
void
ConfusionMatrixCalculator<TRefListLabel, TProdListLabel>
::Compute()
{
  typename RefListLabelType::ConstIterator  refIterator = m_ReferenceLabels->Begin();
  typename ProdListLabelType::ConstIterator prodIterator = m_ProducedLabels->Begin();

  //check that both lists have the same number of samples
  if ( (m_ReferenceLabels->Size() != m_ProducedLabels->Size()) ||
        (m_ReferenceLabels->Size() == 0 ) ||
        (m_ProducedLabels->Size() == 0 ) )
    {
    otbMsgDebugMacro(<< "refLabels size = " << m_ReferenceLabels->Size() <<
                     " / proLabels size = " << m_ProducedLabels->Size());
    throw itk::ExceptionObject(__FILE__, __LINE__, "ListSample size mismatch", ITK_LOCATION);
    }

  m_NumberOfSamples = m_ReferenceLabels->Size();

  // count the number of classes
  int countClasses = 0;
  while (refIterator != m_ReferenceLabels->End())
    {
    ClassLabelType currentLabel = refIterator.GetMeasurementVector()[0];
    if (m_MapOfClasses.find(currentLabel) == m_MapOfClasses.end())
      {
      m_MapOfClasses[currentLabel] = countClasses;
      m_MapOfIndices[countClasses] = currentLabel;
      ++countClasses;
      }
    ++refIterator;
    }

  m_NumberOfClasses = countClasses;


  // SORTING of m_MapOfClasses and m_MapOfIndices according to increasing class labels
  typename MapOfClassesType::iterator itMapOfClasses;
  itMapOfClasses = m_MapOfClasses.begin();

  unsigned int itElt = 0;
  while (itMapOfClasses != m_MapOfClasses.end())
    {
    ClassLabelType currentLabel = itMapOfClasses->first;
    m_MapOfClasses[currentLabel] = itElt;
    m_MapOfIndices[itElt] = currentLabel;
    ++itMapOfClasses;
    ++itElt;
    }


  std::vector<long int> samplesPerClass;

  for (unsigned int i = 0; i < m_NumberOfClasses; ++i)
    samplesPerClass.push_back(0);

  m_ConfusionMatrix = ConfusionMatrixType(m_NumberOfClasses, m_NumberOfClasses);
  m_ConfusionMatrix.Fill(0);

  refIterator = m_ReferenceLabels->Begin();
  prodIterator = m_ProducedLabels->Begin();

  while (refIterator != m_ReferenceLabels->End())
    {
    int refLabel = refIterator.GetMeasurementVector()[0];
    int prodLabel = prodIterator.GetMeasurementVector()[0];

    int refPos = m_MapOfClasses[refLabel];
    int prodPos = m_MapOfClasses[prodLabel];

    ++samplesPerClass[refPos];
    m_ConfusionMatrix(refPos, prodPos) += 1;

    ++refIterator;
    ++prodIterator;
    }


  m_ConfMatMeasurements->SetConfusionMatrix(m_ConfusionMatrix);
  m_ConfMatMeasurements->Compute();

  this->m_TruePositiveValues = m_ConfMatMeasurements->GetTruePositiveValues();
  this->m_FalseNegativeValues = m_ConfMatMeasurements->GetFalseNegativeValues();
  this->m_TrueNegativeValues = m_ConfMatMeasurements->GetTrueNegativeValues();
  this->m_FalsePositiveValues = m_ConfMatMeasurements->GetFalsePositiveValues();

  this->m_Precisions = m_ConfMatMeasurements->GetPrecisions();
  this->m_Recalls = m_ConfMatMeasurements->GetRecalls();
  this->m_FScores = m_ConfMatMeasurements->GetFScores();


  this->m_TruePositiveValue = m_ConfMatMeasurements->GetTruePositiveValue();
  this->m_FalseNegativeValue = m_ConfMatMeasurements->GetFalseNegativeValue();
  this->m_TrueNegativeValue = m_ConfMatMeasurements->GetTrueNegativeValue();
  this->m_FalsePositiveValue = m_ConfMatMeasurements->GetFalsePositiveValue();

  this->m_Precision = m_ConfMatMeasurements->GetPrecision();
  this->m_Recall = m_ConfMatMeasurements->GetRecall();
  this->m_FScore = m_ConfMatMeasurements->GetFScore();

  this->m_OverallAccuracy = m_ConfMatMeasurements->GetOverallAccuracy();
  this->m_KappaIndex = m_ConfMatMeasurements->GetKappaIndex();

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
