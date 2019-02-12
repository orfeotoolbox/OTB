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


#ifndef otbPeriodicSOM_h
#define otbPeriodicSOM_h

#include "otbSOM.h"

namespace otb
{

/**
 * \class PeriodicSOM
 * \brief This class is responsible for the learning of a self organizing
 * map when considered as a torus.
 *
 * This class extends the SOM object which implements the Self
 * Organizing Map (or Kohonen map) learning.
 *
 * The learning process iteratively select the best-response neuron for each input vector,
 * enhancing its response and the response of its neighbors with respect to a certain radius,
 * computed from an initial radius, and to a certain learning factor, decreasing at each iteration.
 *
 * The behavior of the neighborhood is given by a functor (templated) which parameter is the current
 * iteration. It returns a neighborhood of type \code SizeType \endcode.
 *
 * The behavior of the learning factor (hold by a beta variable) is given by an other functor
 * which parameter is the current iteration. It returns a beta value of type \code double \endcode.
 *
 * The SOMMap produced as output can be either initialized with a constant custom value or randomly
 * generated following a normal law. The seed for the random initialization can be modified.
 *
 * \sa SOMMap
 * \sa SOMActivationBuilder
 * \sa CzihoSOMLearningBehaviorFunctor
 * \sa CzihoSOMNeighborhoodBehaviorFunctor
 *
 * \ingroup OTBSOM
 */
template <class TListSample, class TMap,
    class TSOMLearningBehaviorFunctor = Functor::CzihoSOMLearningBehaviorFunctor,
    class TSOMNeighborhoodBehaviorFunctor = Functor::CzihoSOMNeighborhoodBehaviorFunctor>
class ITK_EXPORT PeriodicSOM
  : public SOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
{
public:
  /** Standard typedefs */
  typedef PeriodicSOM Self;
  typedef SOM<TListSample, TMap,
      TSOMLearningBehaviorFunctor,
      TSOMNeighborhoodBehaviorFunctor> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Runtime information macro */
  itkTypeMacro(PeriodicSOM, SOM);

  typedef TListSample                      ListSampleType;
  typedef typename ListSampleType::Pointer ListSamplePointerType;
  typedef TMap                             MapType;
  typedef typename MapType::PixelType      NeuronType;
  typedef typename NeuronType::ValueType   ValueType;
  typedef typename MapType::IndexType      IndexType;
  typedef typename MapType::SizeType       SizeType;
  typedef typename MapType::RegionType     RegionType;
  typedef typename MapType::Pointer        MapPointerType;

protected:
  /** Constructor */
  PeriodicSOM() {}
  /** Destructor */
  ~PeriodicSOM() override {}
  /** Output information redefinition */
  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation ();
  }
  /** Output allocation redefinition */
  void AllocateOutputs() override
  {
    Superclass::AllocateOutputs();
  }
  /** Main computation method */
  void GenerateData(void) override
  {
    Superclass::GenerateData();
  }
  /**
  * Update the output map with a new sample.
  * \param sample The new sample to learn,
  * \param beta The learning coefficient,
  * \param radius The radius of the nieghbourhood.
  */
  void UpdateMap(const NeuronType& sample, double beta, SizeType& radius) override;
  /**
  * Step one iteration.
  */
  void Step(unsigned int currentIteration) override
  {
    Superclass::Step(currentIteration);
  }
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  PeriodicSOM(const Self &) = delete;
  void operator =(const Self&) = delete;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPeriodicSOM.hxx"
#endif

#endif
