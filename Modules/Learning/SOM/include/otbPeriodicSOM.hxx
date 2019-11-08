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


#ifndef otbPeriodicSOM_hxx
#define otbPeriodicSOM_hxx

#include "itkNumericTraits.h"
#include "itkNeighborhoodIterator.h"

#include "otbPeriodicSOM.h"

namespace otb
{

/**
 * Update the output map with a new sample.
 * \param sample The new sample to learn,
 * \param beta The learning coefficient,
 * \param radius The radius of the neighbourhood.
 */
template <class TListSample, class TMap, class TSOMLearningBehaviorFunctor, class TSOMNeighborhoodBehaviorFunctor>
void PeriodicSOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>::UpdateMap(const NeuronType& sample, double beta,
                                                                                                             SizeType& radius)
{
  unsigned int i, j;

  // output map pointer
  MapPointerType map = this->GetOutput(0);

  // winner index in the map
  IndexType  position = map->GetWinner(sample);
  NeuronType winner   = map->GetPixel(position);

  // Local neighborhood definition
  typedef typename MapType::Superclass            ImageMapType;
  typedef itk::NeighborhoodIterator<ImageMapType> NeighborhoodIteratorType;
  typename MapType::RegionType                    mapRegion = map->GetLargestPossibleRegion();
  NeighborhoodIteratorType                        it(radius, map, mapRegion);

  // Here, the periodic update is achieved 'by hand' since
  // PeriodicBoundaryCondition does not allow modifying
  // VectorImage contents
  SizeType  mapSize = mapRegion.GetSize();
  IndexType positionToUpdate;

  // Iterate over the neighborhood of the winner neuron
  it.SetLocation(position);

  for (i = 0; i < it.Size(); ++i)
  {
    typename NeighborhoodIteratorType::OffsetType offset = it.GetOffset(i);

    // The neighborhood is of elliptic shape
    double theDistance = itk::NumericTraits<double>::Zero;
    for (j = 0; j < MapType::ImageDimension; ++j)
      theDistance += pow(static_cast<double>(offset[j]), 2.0) / pow(static_cast<double>(radius[j]), 2.0);

    if (theDistance <= 1.0)
    {
      for (j = 0; j < MapType::ImageDimension; ++j)
      {
        int pos             = offset[j] + position[j];
        positionToUpdate[j] = (pos >= 0) ? pos % mapSize[j] : (mapSize[j] - ((-pos) % mapSize[j])) % mapSize[j];
      }

      NeuronType tempNeuron = it.GetPixel(i);
      // NeuronType newNeuron ( tempNeuron.Size() );
      // newNeuron.Fill( 0.0 ); // FIXME
      NeuronType newNeuron(tempNeuron);

      double tempBeta = beta / (1.0 + theDistance);
      for (j = 0; j < newNeuron.Size(); ++j)
      {
        newNeuron[j] += static_cast<typename NeuronType::ValueType>((sample[j] - tempNeuron[j]) * tempBeta);
      }
      map->SetPixel(positionToUpdate, newNeuron);
    }
  }
}

} // end of namespace otb

#endif
