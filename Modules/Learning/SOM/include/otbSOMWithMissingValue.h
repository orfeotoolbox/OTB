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

#ifndef otbSOMWithMissingValue_h
#define otbSOMWithMissingValue_h

#include "otbPeriodicSOM.h"

namespace otb
{
/**
 *  \class SOMWithMissingValue
 *  \brief This class implements SOM training with missing value
 *
 *  The distance between vectors that may have missing components is holded
 *  by the EuclideanDistanceMetricWithMissingValue class in the SOMMap distance
 *  template. Nevertheless, this class re-implements the UpdateMap method to
 *  adapt the evaluation of each component of the 'newNeuron' when dealing
 *  with missing values.
 *
 *  TMap has to be templeted with EuclideanDistanceMetricWithMissingValuePow2
 *
 *  \sa SOMMap
 *  \sa PeriodicSOM
 *  \sa EuclideanDistanceMetricWithMissingValue
 *
 * \ingroup OTBSOM
 */
template <class TListSample, class TMap, class TSOMLearningBehaviorFunctor = Functor::CzihoSOMLearningBehaviorFunctor,
          class TSOMNeighborhoodBehaviorFunctor = Functor::CzihoSOMNeighborhoodBehaviorFunctor>
class ITK_EXPORT SOMWithMissingValue : public PeriodicSOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor>
{
public:
  /** Standard typedefs */
  typedef SOMWithMissingValue Self;
  typedef PeriodicSOM<TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TListSample                      ListSampleType;
  typedef typename ListSampleType::Pointer ListSamplePointerType;

  typedef TMap                                  MapType;
  typedef typename MapType::PixelType           NeuronType;
  typedef typename NeuronType::ValueType        ValueType;
  typedef typename MapType::IndexType           IndexType;
  typedef typename MapType::SizeType            SizeType;
  typedef typename MapType::RegionType          RegionType;
  typedef typename MapType::Pointer             MapPointerType;
  typedef typename MapType::DistanceType        DistanceType;
  typedef typename MapType::DistancePointerType DistancePointerType;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(SOMWithMissingValue, PeriodicSOM);

protected:
  /** Constructor */
  SOMWithMissingValue();
  /** Destructor */
  ~SOMWithMissingValue() override;
  /** Output information redefinition */
  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation();
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
   * Update the output map with a new sample, depending on the availability of the data
   */
  void UpdateMap(const NeuronType& sample, double beta, SizeType& radius) override;

  /** Step one iteration. */
  void Step(unsigned int currentIteration) override
  {
    Superclass::Step(currentIteration);
  }
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  SOMWithMissingValue(const Self&) = delete;
  void operator=(const Self&) = delete;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMWithMissingValue.hxx"
#endif

#endif
