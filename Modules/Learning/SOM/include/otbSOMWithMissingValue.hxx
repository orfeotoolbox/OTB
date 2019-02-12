/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSOMWithMissingValue_hxx
#define otbSOMWithMissingValue_hxx

#include "otbSOMWithMissingValue.h"

#include "itkNumericTraits.h"
#include "itkNeighborhoodIterator.h"
#include "otbMacro.h"

namespace otb
{
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
SOMWithMissingValue <TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::SOMWithMissingValue(void)
{}

template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
SOMWithMissingValue <TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::~SOMWithMissingValue(void)
{}

/**
 * Update the output map with a new sample by including the case when some
 * components of this new sample may be missing.
 * \param sample The new sample to learn,
 * \param beta The learning coefficient,
 * \param radius The radius of the nieghbourhood.
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOMWithMissingValue<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::UpdateMap(const NeuronType& sample, double beta, SizeType& radius)
{
  // output map pointer
  MapPointerType map = this->GetOutput(0);

  // winner index in the map
  IndexType  position = map->GetWinner(sample);
  NeuronType winner = map->GetPixel(position);

  // Local neighborhood definition
  typedef typename MapType::Superclass            ImageMapType;
  typedef itk::NeighborhoodIterator<ImageMapType> NeighborhoodIteratorType;
  typename MapType::RegionType mapRegion = map->GetLargestPossibleRegion();
  NeighborhoodIteratorType it(radius, map, mapRegion);

  // Here, the periodic update is achieved 'by hand' since
  // PeriodicBoundaryCondition does not allow modifying
  // VectorImage contents
  SizeType  mapSize = mapRegion.GetSize();
  IndexType positionToUpdate;

  // Iterate over the neighborhood of the winner neuron
  it.SetLocation(position);

  for (unsigned int i = 0; i < it.Size(); ++i)
    {
    typename NeighborhoodIteratorType::OffsetType offset = it.GetOffset(i);

    // The neighborhood is of elliptic shape
    double theDistance = itk::NumericTraits<double>::Zero;
    for (unsigned int j = 0; j < MapType::ImageDimension; ++j)
      theDistance += pow(static_cast<double>(offset[j]), 2.0)
                     / pow(static_cast<double>(radius[j]), 2.0);

    if (theDistance <= 1.0)
      {
      for (unsigned int j = 0; j < MapType::ImageDimension; ++j)
        {
        int pos = offset[j] + position[j];
        positionToUpdate[j] = (pos >= 0) ?
                              pos % mapSize[j] :
                              (mapSize[j] - ((-pos) % mapSize[j])) % mapSize[j];
        }

      NeuronType tempNeuron = it.GetPixel(i);
      NeuronType newNeuron(tempNeuron);

      double tempBeta = beta / (1.0 + theDistance);
      for (unsigned int j = 0; j < newNeuron.Size(); ++j)
        {
        if (!DistanceType::IsMissingValue(sample[j]))
          newNeuron[j] += static_cast<typename NeuronType::ValueType>(
            (sample[j] - tempNeuron[j]) * tempBeta);
        }
      map->SetPixel(positionToUpdate, newNeuron);
      }
    }
}

template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOMWithMissingValue<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

} // end PrintSelf

} // end iof namespace otb

#endif
