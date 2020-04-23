/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSamplingRateCalculatorList_h
#define otbSamplingRateCalculatorList_h

#include "otbSamplingRateCalculator.h"
#include "otbObjectList.h"
#include "OTBSamplingExport.h"

namespace otb
{
/** \class SamplingRateCalculatorList
 *  \brief This class is a list of sampling rate calculators.
 *
 * It computes sampling rates for each image and each class, given input
 * statistics on available samples, and a sampling strategy. The sampling
 * strategies are made from the combination of :
 *   - the sampling strategies of otb::SamplingRateCalculator  (mono-image)
 *   - a multi-image behaviour : proportional / constant / custom
 *
 * \ingroup OTBSampling
 */
class OTBSampling_EXPORT SamplingRateCalculatorList : public ObjectList<SamplingRateCalculator>
{
public:
  /** Standard typedefs */
  typedef SamplingRateCalculatorList         Self;
  typedef ObjectList<SamplingRateCalculator> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  typedef SamplingRateCalculator::ClassCountMapType ClassCountMapType;
  typedef SamplingRateCalculator::MapRateType       MapRateType;

  enum PartitionType
  {
    PROPORTIONAL,
    EQUAL,
    CUSTOM
  };

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculatorList, ObjectList);

  /** Set the class counts for input 'index' */
  void SetNthClassCount(unsigned int index, const ClassCountMapType& map);

  /** Get the sampling rates computed for input 'index' */
  const MapRateType& GetRatesByClass(unsigned int index);

  /** Clear internal data */
  void ClearRates(void);

  /** Method to select all available samples */
  void SetAllSamples(PartitionType t);

  /** Method to choose a sampling strategy based on the smallest class.
   * The number of samples in each class is set to this minimum size*/
  void SetMinimumNbOfSamplesByClass(PartitionType t);

  /** Method to set the same number of required samples in each class */
  void SetNbOfSamplesAllClasses(std::vector<unsigned long>& nb, PartitionType t);

  /** Method to manually set the number of samples required in each class */
  void SetNbOfSamplesByClass(const std::vector<ClassCountMapType>& required, PartitionType t);

  /** Method to use a percentage of the samples available in each
    * class */
  void SetPercentageOfSamples(std::vector<double>& p, PartitionType t);

  /** Method to set the total number of samples and use classes proportions
   */
  void SetTotalNumberOfSamples(std::vector<unsigned long>& tot, PartitionType t);

protected:
  /** Constructor */
  SamplingRateCalculatorList()
  {
  }

  /** Destructor */
  ~SamplingRateCalculatorList() override
  {
  }

private:
  SamplingRateCalculatorList(const Self&) = delete;
  void operator=(const Self&) = delete;

  void UpdateGlobalCounts();

  ClassCountMapType m_GlobalCountMap;
};
} // end namespace otb

#endif
