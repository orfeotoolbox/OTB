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

#ifndef otbConfusionMatrixToMassOfBelief_hxx
#define otbConfusionMatrixToMassOfBelief_hxx

#include "otbConfusionMatrixToMassOfBelief.h"

namespace otb
{

template <class TConfusionMatrix, class TLabel>
ConfusionMatrixToMassOfBelief<TConfusionMatrix, TLabel>::ConfusionMatrixToMassOfBelief()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  this->m_ConfMatMeasurements = ConfusionMatrixMeasurementsType::New();
  this->m_DefinitionMethod    = this->PRECISION;
}

template <class TConfusionMatrix, class TLabel>
void ConfusionMatrixToMassOfBelief<TConfusionMatrix, TLabel>::Update()
{
  this->GenerateData();
}

template <class TConfusionMatrix, class TLabel>
void ConfusionMatrixToMassOfBelief<TConfusionMatrix, TLabel>::GenerateData()
{
  this->m_ConfMatMeasurements->SetConfusionMatrix(m_ConfusionMatrix);
  this->m_ConfMatMeasurements->Compute();

  typename MapOfIndicesType::iterator itMapOfIndices;

  MassType currentMass = 0.;
  this->m_MapMassOfBelief.clear();
  for (itMapOfIndices = m_MapOfIndices.begin(); itMapOfIndices != m_MapOfIndices.end(); ++itMapOfIndices)
  {
    switch (m_DefinitionMethod)
    {
    case PRECISION:
      // Masses of Belief = Precision Rate of each label (TP / [TP + FP])
      currentMass = m_ConfMatMeasurements->GetPrecisions()[itMapOfIndices->first];
      break;
    case RECALL:
      // Masses of Belief = Recall Rate of each label (TP / [TP + FN])
      currentMass = m_ConfMatMeasurements->GetRecalls()[itMapOfIndices->first];
      break;
    case ACCURACY:
      // Masses of Belief = Overall Accuracy of the confusion matrix (SUM[TP] / nbSamples)
      currentMass = m_ConfMatMeasurements->GetOverallAccuracy();
      break;
    case KAPPA:
      // Masses of Belief = Kappa Index of the confusion matrix
      currentMass = m_ConfMatMeasurements->GetKappaIndex();
      break;

    default:
      // Masses of Belief = Precision Rate of each label (TP / [TP + FP]
      currentMass = m_ConfMatMeasurements->GetPrecisions()[itMapOfIndices->first];
      break;
    } // END switch (m_DefinitionMethod)

    this->m_MapMassOfBelief[itMapOfIndices->second] = currentMass;
  }
}
} // end namespace otb

#endif
