/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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

#ifndef otbSOMMap_hxx
#define otbSOMMap_hxx

#include "otbSOMMap.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
/**
 * Constructor
 */
template <class TNeuron, class TDistance, unsigned int VMapDimension>
SOMMap<TNeuron, TDistance, VMapDimension>::SOMMap()
{
}
/**
 * Destructor
 */
template <class TNeuron, class TDistance, unsigned int VMapDimension>
SOMMap<TNeuron, TDistance, VMapDimension>::~SOMMap()
{
}
/**
 * Get The index of the winning neuron for a sample.
 * \param sample The sample
 * \return The index of the winning neuron.
 */
template <class TNeuron, class TDistance, unsigned int        VMapDimension>
typename SOMMap<TNeuron, TDistance, VMapDimension>::IndexType SOMMap<TNeuron, TDistance, VMapDimension>::GetWinner(const NeuronType& sample)
{
  // Some typedefs
  typedef itk::ImageRegionIteratorWithIndex<Self> IteratorType;

  // Define the euclidean distance used to compute the neural response
  DistancePointerType activation = DistanceType::New();

  // Define the iterator used to walk through the map
  IteratorType it(this, this->GetLargestPossibleRegion());
  it.GoToBegin();

  // Define the minimum distance and position
  IndexType minPos = it.GetIndex();

  double minDistance = activation->Evaluate(sample, it.Get());

  // Iterate through the map to get the minimum distance position
  for (; !it.IsAtEnd(); ++it)
  {
    double tempDistance = activation->Evaluate(sample, it.Get());
    if (tempDistance <= minDistance)
    {
      minDistance = tempDistance;
      minPos      = it.GetIndex();
    }
  }
  // Return the index of the winner
  return minPos;
}
template <class TNeuron, class TDistance, unsigned int VMapDimension>
void SOMMap<TNeuron, TDistance, VMapDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
