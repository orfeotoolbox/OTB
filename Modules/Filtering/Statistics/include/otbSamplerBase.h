/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSamplerBase_h
#define otbSamplerBase_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

/**
 * \class SamplerBase
 *
 * \brief This is a base class for sampling methods
 *
 * \ingroup OTBStatistics
 */
class ITK_EXPORT SamplerBase : public itk::Object
{
public:
  typedef SamplerBase                   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Runtime information support. */
  itkTypeMacro(SamplerBase, itk::Object);

  /**
   * Reset internal counter (to be called before starting iteration)
   */
  virtual void Reset(void);

  /** method to set the number of needed and total elements
   *  the sampling rate is automatically updated */
  void SetNumberOfElements(unsigned long needed, unsigned long total);

  /** method to set the sampling rate, if the total number of elements
   *  is given, the number of needed elements is updated */
  void SetRate(double rate, unsigned long total = 0UL);

  /** Get macro for sampling rate */
  itkGetMacro(Rate, double);

  /** Get macro for the total number of elements */
  itkGetMacro(TotalElements, unsigned long);

  /** Get macro for the number of needed elements */
  itkGetMacro(NeededElements, unsigned long);

  /** Get macro for the current number of chosen elements */
  itkGetMacro(ChosenElements, unsigned long);

  /** Get macro for the current number of processed elements */
  itkGetMacro(ProcessedElements, unsigned long);

protected:
  /** Constructor */
  SamplerBase();

  /** Destructor */
  ~SamplerBase() override
  {
  }

  /** Current count of selected elements */
  unsigned long m_ChosenElements;

  /** Current count of encountered elements during iteration */
  unsigned long m_ProcessedElements;

private:
  // Not implemented
  SamplerBase(const Self&);
  void operator=(const Self&);

  /** Total number of elements during the iteration */
  unsigned long m_TotalElements;

  /** Number of elements to select during the iteration */
  unsigned long m_NeededElements;

  /** Sampling rate */
  double m_Rate;
};

} // namespace otb
#endif
