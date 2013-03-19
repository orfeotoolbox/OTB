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
  m_ConfusionMatrixCalculator = ConfusionMatrixCalculatorType::New();
  m_DefinitionMethod = this->PRECISION;
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
  MapOfIndicesType mapOfIndices = m_ConfusionMatrixCalculator->GetMapOfIndices();
  typename MapOfIndicesType::iterator itMapOfIndices;

  MassType currentMass = 0;
  for (itMapOfIndices = mapOfIndices.begin(); itMapOfIndices != mapOfIndices.end(); ++itMapOfIndices)
    {
    // Masses of Belief = Precision Rate of each label (TP/[TP + FP])
    if (m_DefinitionMethod == this->PRECISION)
      {
      currentMass = m_ConfusionMatrixCalculator->GetPrecisions()[itMapOfIndices->first];
      }
    else
      {
      // Masses of Belief = Recall Rate of each label (TP/[TP + FN])
      if (m_DefinitionMethod == this->RECALL)
        {
        currentMass = m_ConfusionMatrixCalculator->GetRecalls()[itMapOfIndices->first];
        }
      }

    m_MapMassOfBelief[itMapOfIndices->second] = currentMass;
    }
}
} // end namespace otb

#endif
