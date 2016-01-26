/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbPeriodicSOM_h
#define __otbPeriodicSOM_h

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
 * generated following a normal law. The seed for the random intialization can be modified.
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
  virtual ~PeriodicSOM() {}
  /** Output information redefinition */
  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation ();
  }
  /** Output allocation redefinition */
  virtual void AllocateOutputs()
  {
    Superclass::AllocateOutputs();
  }
  /** Main computation method */
  virtual void GenerateData(void)
  {
    Superclass::GenerateData();
  }
  /**
  * Update the output map with a new sample.
  * \param sample The new sample to learn,
  * \param beta The learning coefficient,
  * \param radius The radius of the nieghbourhood.
  */
  virtual void UpdateMap(const NeuronType& sample, double beta, SizeType& radius);
  /**
  * Step one iteration.
  */
  virtual void Step(unsigned int currentIteration)
  {
    Superclass::Step(currentIteration);
  }
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  PeriodicSOM(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPeriodicSOM.txx"
#endif

#endif
