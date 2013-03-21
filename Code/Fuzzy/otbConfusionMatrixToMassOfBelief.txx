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
#ifndef __otbConfusionMatrixToMassOfBelief_txx
#define __otbConfusionMatrixToMassOfBelief_txx

#include "otbConfusionMatrixToMassOfBelief.h"

namespace otb
{

template<class TConfusionMatrix>
ConfusionMatrixToMassOfBelief<TConfusionMatrix>
::ConfusionMatrixToMassOfBelief()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);
  this->m_ConfMatMeasurements = ConfusionMatrixMeasurementsType::New();
  this->m_DefinitionMethod = this->PRECISION;
}

template <class TConfusionMatrix>
void
ConfusionMatrixToMassOfBelief<TConfusionMatrix>
::Update()
{
  this->GenerateData();
}

template <class TConfusionMatrix>
void
ConfusionMatrixToMassOfBelief<TConfusionMatrix>
::GenerateData()
{
  this->m_ConfMatMeasurements->SetConfusionMatrix(m_ConfusionMatrix);
  this->m_ConfMatMeasurements->Update();

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
      }// END switch (m_DefinitionMethod)

    this->m_MapMassOfBelief[itMapOfIndices->second] = currentMass;
    }
}
} // end namespace otb

#endif
