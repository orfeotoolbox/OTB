/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSOM_txx
#define __otbSOM_txx

#include "otbSOM.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkFixedArray.h"
#include "otbMacro.h"
#include "itkImageRegionIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::SOM()
{
  this->SetNumberOfRequiredInputs(0);
  this->SetNumberOfRequiredOutputs(1);

  m_MapSize.Fill(10);
  m_NumberOfIterations = 10;
  m_BetaInit = 1.0;
  m_BetaEnd = 0.2;
  m_NeighborhoodSizeInit.Fill(3);
  m_MinWeight = static_cast<ValueType>(0.0);
  m_MaxWeight = static_cast<ValueType>(128.0);
  m_RandomInit = false;
  m_Seed = 123574651;
}
/**
 * Destructor
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::~SOM()
{
}
/**
 * Update the output map with a new sample.
 * \param sample The new sample to learn,
 * \param beta The learning coefficient,
 * \param radius The radius of the nieghbourhood.
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::UpdateMap(const NeuronType& sample, double beta, SizeType& radius)
{
  // output map pointer
  MapPointerType map = this->GetOutput(0);

  // typedefs
  typedef itk::ImageRegionIteratorWithIndex<MapType>              IteratorType;
  typedef itk::FixedArray<double, MapType::ImageDimension>        FixedArrayIndexType;
  typedef itk::Statistics::EuclideanDistanceMetric<FixedArrayIndexType> DistanceType;
  typename DistanceType::Pointer distance = DistanceType::New();

  // winner index in the map
  IndexType  position = map->GetWinner(sample);
  NeuronType winner = map->GetPixel(position);

  // Local neighborhood definition
  RegionType localRegion;
  IndexType  localIndex = position - radius;
  SizeType   localSize;
  for (unsigned int i = 0; i < MapType::ImageDimension; ++i)
    {
    localSize[i] = 2 * radius[i] + 1;
    }
  localRegion.SetIndex(localIndex);
  localRegion.SetSize(localSize);
  localRegion.Crop(map->GetLargestPossibleRegion());
  IteratorType it(map, localRegion);

  // Walk through the map, and evolve each neuron depending on its
  // distance to the winner.
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    NeuronType tempNeuron = it.Get();
    NeuronType newNeuron(tempNeuron.Size());
    
    FixedArrayIndexType positionFA,indexFA;
    positionFA[0] = position[0];
    positionFA[1] = position[1];

    indexFA[0] = it.GetIndex()[0];
    indexFA[1] = it.GetIndex()[1];
    
    double tempBeta = beta
                      / (1 +
                         distance->Evaluate(positionFA, indexFA));

    for (unsigned int i = 0; i < newNeuron.Size(); ++i)
      {
      newNeuron[i] = tempNeuron[i]
                     + static_cast<typename NeuronType::ValueType>(
        (sample[i] - tempNeuron[i]) * tempBeta);
      }

    it.Set(newNeuron);
    }
}
/**
 * Step one iteration.
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::Step(unsigned int currentIteration)
{
  // Compute the new learning coefficient
  double newBeta = m_BetaFunctor(
    currentIteration, m_NumberOfIterations, m_BetaInit, m_BetaEnd);

  // Compute the new neighborhood size
  SizeType newSize = m_NeighborhoodSizeFunctor(
    currentIteration, m_NumberOfIterations, m_NeighborhoodSizeInit);

  // update the neurons map with each example of the training set.
  otbMsgDebugMacro(<< "Beta: " << newBeta << ", radius: " << newSize);
  for (typename ListSampleType::Iterator it = m_ListSample->Begin();
       it != m_ListSample->End();
       ++it)
    {
    UpdateMap(it.GetMeasurementVector(), newBeta, newSize);
    }
}
/**
 *  Output information redefinition
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(
    m_ListSample->GetMeasurementVectorSize());

  IndexType index;
  index.Fill(0);

  RegionType region;
  region.SetIndex(index);
  region.SetSize(this->GetMapSize());
  this->GetOutput()->SetRegions(region);

}
/**
 *  Output redefinition
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::AllocateOutputs()
{
  if (this->GetNumberOfOutputs() != 1) itkExceptionMacro(<< "Number of output image should be 1");

  // output neuron map fill
  MapPointerType map = this->GetOutput(0);
  map->Allocate();
}

/**
 * Main computation method
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::GenerateData(void)
{
  this->AllocateOutputs();
  this->BeforeThreadedGenerateData();

  MapPointerType map = this->GetOutput(0);

  if (m_RandomInit)
    {
    typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
    typedef itk::ImageRegionIterator<MapType>                      IteratorType;
    GeneratorType::Pointer generator = GeneratorType::New();
    generator->Initialize(m_Seed);

    NeuronType neuronInit(m_ListSample->GetMeasurementVectorSize());
    IteratorType it(map, map->GetLargestPossibleRegion());

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
      {
      for (unsigned int i = 0; i < neuronInit.Size(); ++i)
        {
        neuronInit[i] = static_cast<typename NeuronType::ValueType>(
          generator->GetUniformVariate(static_cast<double>(m_MinWeight),
                                       static_cast<double>(m_MaxWeight)));
        }
      it.Set(neuronInit);
      }
    }
  else
    {
    NeuronType neuronInit(m_ListSample->GetMeasurementVectorSize());
    neuronInit.Fill(m_MaxWeight);
    map->FillBuffer(neuronInit);
    }

  // Step through the iterations
  for (unsigned int i = 0; i < m_NumberOfIterations; ++i)
    {
    //otbMsgDebugMacro(<<"Step "<<i+1<<" / "<<m_NumberOfIterations);
    std::cerr << "Step " << i + 1 << " / " << m_NumberOfIterations << "                         \r";
    Step(i);
    }

  this->AfterThreadedGenerateData();
}
/**
 *PrintSelf method
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor>
void
SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
