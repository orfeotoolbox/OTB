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

#ifndef otbPeriodicSampler_h
#define otbPeriodicSampler_h

#include "otbSamplerBase.h"

namespace otb
{

/**
 * \class PeriodicSampler
 *
 * \brief Periodic sampler for iteration loops
 *
 * This class allows doing periodic sampling during an iteration loop.
 *
 * \ingroup OTBStatistics
 */
class ITK_EXPORT PeriodicSampler : public SamplerBase
{
public:
  typedef PeriodicSampler               Self;
  typedef SamplerBase                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Internal parameters, only contains an offset to shift the periodic
   * sampling
   */
  typedef struct Parameter
  {
    /** Offset that shifts the whole periodic sampling
     *  (disabled if jitter is used) */
    unsigned long Offset;

    /** Maximum jitter to introduce (0 means no jitter) */
    unsigned long MaxJitter;

    /** Maximum buffer size for internal jitter values */
    unsigned long MaxBufferSize;

    bool operator!=(const struct Parameter& param) const;
  } ParameterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PeriodicSampler, SamplerBase);

  /** Setter for internal parameters */
  void SetParameters(const ParameterType& param)
  {
    if (m_Parameters != param)
    {
      this->Modified();
      m_Parameters = param;
    }
  }

  /** Getter for internal parameters */
  ParameterType GetParameters()
  {
    return m_Parameters;
  }

  /**
   * Method that resets the internal state of the sampler
   */
  void Reset(void) override;

  /**
   * Method to call during iteration, returns true if the sample is selected,
   * and false otherwise.
   */
  bool TakeSample(void);

protected:
  /** Constructor */
  PeriodicSampler();

  /** Destructor */
  ~PeriodicSampler() override
  {
  }

private:
  // Not implemented
  PeriodicSampler(const Self&);
  void operator=(const Self&);

  /** Internal parameters for the sampler */
  ParameterType m_Parameters;

  /** Internal width for jitter */
  double m_JitterSize;

  /** Internal current offset value
   *  (either fixed, or reset each time a sample is taken)*/
  double m_OffsetValue;

  /** jitter offsets computed up to MaxBufferSize */
  std::vector<double> m_JitterValues;
};

} // namespace otb
#endif
