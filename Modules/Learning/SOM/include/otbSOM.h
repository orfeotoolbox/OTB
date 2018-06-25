/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSOM_h
#define otbSOM_h

#include "itkImageToImageFilter.h"
#include "itkEuclideanDistanceMetric.h"

#include "otbCzihoSOMLearningBehaviorFunctor.h"
#include "otbCzihoSOMNeighborhoodBehaviorFunctor.h"

namespace otb
{
/**
 * \class SOM
 * \brief This class is responsible for the learning of a self organizing map from a
 * set of vector represented by the input image (each vector is a pixel of the image).
 *
 * The learning process iteratively select the best-response neuron for each input vector,
 * enhancing its response and the response of its neighbors with respect to a certain radius,
 * computed from an initial radius, and to a certain learning factor, decreasing at each iteration.
 *
 * The behavior of the neighborhood is given by a functor (templated) which parameter is the current
 * iteration. It returns a neighborhood of type \code SizeType \endcode.
 *
 * The behavior of the learning factor (hold by a beta variable) is given by an other functor
 * which parameter is the current iteration. It returns a beta value of type double.
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
class ITK_EXPORT SOM
  : public itk::ImageSource<TMap>
{
public:
  /** Standard typedefs */
  typedef SOM                           Self;
  typedef itk::ImageSource<TMap>        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(SOM, ImageSource);

  typedef TListSample                      ListSampleType;
  typedef typename ListSampleType::Pointer ListSamplePointerType;
  typedef TMap                             MapType;
  typedef typename MapType::PixelType      NeuronType;
  typedef typename NeuronType::ValueType   ValueType;
  typedef typename MapType::IndexType      IndexType;
  typedef typename MapType::SizeType       SizeType;
  typedef typename MapType::RegionType     RegionType;
  typedef typename MapType::Pointer        MapPointerType;

  typedef TSOMLearningBehaviorFunctor     SOMLearningBehaviorFunctorType;
  typedef TSOMNeighborhoodBehaviorFunctor SOMNeighborhoodBehaviorFunctorType;

  /** Map dimension */
  itkStaticConstMacro(MapDimension, unsigned int, MapType::ImageDimension);

  /** Accessors */
  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetMacro(NumberOfIterations, unsigned int);
  itkSetMacro(BetaInit, double);
  itkGetMacro(BetaInit, double);
  itkSetMacro(BetaEnd, double);
  itkGetMacro(BetaEnd, double);
  itkSetMacro(MinWeight, ValueType);
  itkGetMacro(MinWeight, ValueType);
  itkSetMacro(MaxWeight, ValueType);
  itkGetMacro(MaxWeight, ValueType);
  itkSetMacro(MapSize, SizeType);
  itkGetMacro(MapSize, SizeType);
  itkSetMacro(NeighborhoodSizeInit, SizeType);
  itkGetMacro(NeighborhoodSizeInit, SizeType);
  itkSetMacro(RandomInit, bool);
  itkGetMacro(RandomInit, bool);
  itkSetMacro(Seed, unsigned int);
  itkGetMacro(Seed, unsigned int);
  itkGetObjectMacro(ListSample, ListSampleType);
  itkSetObjectMacro(ListSample, ListSampleType);

  void SetBetaFunctor(const SOMLearningBehaviorFunctorType& functor)
  {
    m_BetaFunctor = functor;
  }

  void SetNeighborhoodSizeFunctor(const SOMNeighborhoodBehaviorFunctorType& functor)
  {
    m_NeighborhoodSizeFunctor = functor;
  }

protected:
  /** Constructor */
  SOM();
  /** Destructor */
  ~SOM() override;
  /** Output information redefinition */
  void GenerateOutputInformation() override;
  /** Output allocation redefinition */
  void AllocateOutputs() override;
  /** Main computation method */
  void GenerateData(void) override;
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
  virtual void Step(unsigned int currentIteration);
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SOM(const Self &) = delete;
  void operator =(const Self&) = delete;
  /** Size of the neurons map */
  SizeType m_MapSize;
  /** Number of iterations */
  unsigned int m_NumberOfIterations;
  /** Initial learning coefficient */
  double m_BetaInit;
  /** Final learning coefficient */
  double m_BetaEnd;
  /** Initial neighborhood size */
  SizeType m_NeighborhoodSizeInit;
  /** Minimum initial neuron weights */
  ValueType m_MinWeight;
  /** Maximum initial neuron weights */
  ValueType m_MaxWeight;
  /** Random initialization bool */
  bool m_RandomInit;
  /** Seed for random initialization */
  unsigned int m_Seed;
  /** The input list sample */
  ListSamplePointerType m_ListSample;
  /** Behavior of the Learning weightening (link to the beta coefficient) */
  SOMLearningBehaviorFunctorType m_BetaFunctor;
  /** Behavior of the Neighborhood extent */
  SOMNeighborhoodBehaviorFunctorType m_NeighborhoodSizeFunctor;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOM.hxx"
#endif

#endif
