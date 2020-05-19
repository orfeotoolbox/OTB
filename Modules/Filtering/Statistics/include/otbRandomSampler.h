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

#ifndef otbRandomSampler_h
#define otbRandomSampler_h

#include "otbSamplerBase.h"

namespace otb
{

/**
 * \class RandomSampler
 *
 * \brief Random sampler for iteration loops
 *
 * This class allows doing random sampling during an iteration loop.
 * It uses the MersenneTwisterRandomGenerator.
 *
 * \ingroup OTBStatistics
 */
class ITK_EXPORT RandomSampler : public SamplerBase
{
public:
  typedef RandomSampler                 Self;
  typedef SamplerBase                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Internal parameters, only contains an offset to shift the periodic
   * sampling
   */
  typedef struct Parameter
  {
    /** Maximum buffer size to generate permutations */
    unsigned long MaxBufferSize;

    bool operator!=(const struct Parameter& param) const;
  } ParameterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RandomSampler, SamplerBase);

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
   * Reset internal counter (to be called before starting iteration)
   */
  void Reset(void) override;

  /**
   * Method to call during iteration, returns true if the sample is selected,
   * and false otherwise.
   */
  bool TakeSample(void);

protected:
  /** Constructor */
  RandomSampler();

  /** Destructor */
  ~RandomSampler() override
  {
  }

private:
  // Not implemented
  RandomSampler(const Self&);
  void operator=(const Self&);

  /** Internal parameters for the sampler */
  ParameterType m_Parameters;

  /** Internal list of positions to select */
  std::vector<unsigned long> m_Positions;

  /** Internal current position */
  std::vector<unsigned long>::iterator m_CurrentPosition;
};

} // namespace otb
#endif
