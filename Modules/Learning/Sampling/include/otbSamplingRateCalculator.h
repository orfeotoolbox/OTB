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

#ifndef otbSamplingRateCalculator_h
#define otbSamplingRateCalculator_h

#include "itkImageRegion.h"
#include "itkVectorContainer.h"
#include <set>
#include "OTBSamplingExport.h"

namespace otb
{
/** \class SamplingRateCalculator
 *  \brief This class is a sampling rate calculator.
 *
 * It computes sampling rates for each class, given input statistics on
 * available samples, and a sampling strategy.
 *
 * \ingroup OTBSampling
 */

class OTBSampling_EXPORT SamplingRateCalculator : public itk::Object
{
public:
  /** Standard typedefs */
  typedef SamplingRateCalculator        Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** typdefs **/
  typedef std::map<std::string, unsigned long> ClassCountMapType;
  typedef ClassCountMapType::const_iterator constItMapType;
  typedef struct OTBSampling_EXPORT         Triplet
  {
    unsigned long Required;
    unsigned long Tot;
    double        Rate;
    bool operator==(const struct Triplet& triplet) const;
  } TripletType;

  typedef std::map<std::string, TripletType> MapRateType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SamplingRateCalculator, itk::Object);

  /** Method to manually set the number of samples required in each class */
  void SetNbOfSamplesByClass(const ClassCountMapType& required);

  /** Method to set the same number of required samples in each class */
  void SetNbOfSamplesAllClasses(unsigned long);

  /** Method to set a percentage of samples for each class */
  void SetPercentageOfSamples(double percent);

  /** Method to set the total number of samples to generate */
  void SetTotalNumberOfSamples(unsigned long value);

  /** Method to choose a sampling strategy based on the smallest class.
   * The number of samples in each class is set to this minimum size*/
  void SetMinimumNbOfSamplesByClass(void);

  /** Method to select all available samples */
  void SetAllSamples(void);

  /** Method to export sampling rates in a CSV file.
   * Columns are : class name, required samples, total samples, rate.*/
  void Write(std::string filename);

  /** Method to import sampling rates from a CSV file.
   * Columns are : class name, required samples, total samples, rate.*/
  void Read(std::string filename);

  /** Get macro to the computed sampling rates.*/
  itkGetConstReferenceMacro(RatesByClass, MapRateType);

  /** Set method to input the total number of samples in each class.
   *  This method should be called before the other Set... methods
   *  that apply a sampling strategy */
  void SetClassCount(const ClassCountMapType& map);

  /** Clear internal data */
  void ClearRates(void);

  static ClassCountMapType ReadRequiredSamples(const std::string& filename);

protected:
  /** Constructor */
  SamplingRateCalculator();

  /** Destructor */
  ~SamplingRateCalculator() override
  {
  }

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SamplingRateCalculator(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Update the computed rate for a given class.*/
  void UpdateRate(const std::string& name);

  /** Internal sampling rates.*/
  MapRateType m_RatesByClass;
};
} // End namespace otb

#endif
