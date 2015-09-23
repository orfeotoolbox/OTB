/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All right reserved.
See IMTCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSOMMap_txx
#define __otbSOMMap_txx

#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
/**
 * Constructor
 */
template <class TNeuron, class TDistance, unsigned int VMapDimension>
SOMMap<TNeuron, TDistance, VMapDimension>
::SOMMap()
{}
/**
 * Destructor
 */
template <class TNeuron, class TDistance, unsigned int VMapDimension>
SOMMap<TNeuron, TDistance, VMapDimension>
::~SOMMap()
{}
/**
 * Get The index of the winning neuron for a sample.
 * \param sample The sample
 * \return The index of the winning neuron.
 */
template <class TNeuron, class TDistance, unsigned int VMapDimension>
typename SOMMap<TNeuron, TDistance, VMapDimension>
::IndexType
SOMMap<TNeuron, TDistance, VMapDimension>
::GetWinner(const NeuronType& sample)
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
      minPos = it.GetIndex();
      }
    }
  // Return the index of the winner
  return minPos;
}
template <class TNeuron, class TDistance, unsigned int VMapDimension>
void
SOMMap<TNeuron, TDistance, VMapDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
