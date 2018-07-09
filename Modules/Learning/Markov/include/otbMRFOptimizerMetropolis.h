/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMRFOptimizerMetropolis_h
#define otbMRFOptimizerMetropolis_h

#include "otbMRFOptimizer.h"
#include "otbMath.h"
#include "itkNumericTraits.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{
/**
 * \class MRFOptimizerMetropolis
 * \brief This is the optimizer class implementing the Metropolis algorithm
 *
 * This is one optimizer to be used in the MRF framework. This optimizer
 * follows the metropolis algorithm to accept of reject the value proposed by the sampler.
 *
 * The MRFOptimizerMetropolis has one parameter corresponding to the temperature T used
 * to accept or reject proposed values. The proposed value is accepted with a probability:
 *
 *  \f[ e^{\frac{-\Delta E}{T}} \f]
 *
 *
 * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
 *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

class ITK_EXPORT MRFOptimizerMetropolis : public MRFOptimizer
{
public:

  typedef MRFOptimizerMetropolis        Self;
  typedef MRFOptimizer                  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef Superclass::ParametersType    ParametersType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

  itkNewMacro(Self);

  itkTypeMacro(MRFOptimizerMetropolis, MRFOptimizer);

  /** Set parameter to a one array filled with paramVal.*/
  void SetSingleParameter(double parameterVal)
  {
    this->m_Parameters.SetSize(1);
    this->m_Parameters.Fill(parameterVal);
    this->Modified();
  }

  inline bool Compute(double deltaEnergy) override
  {
    if (deltaEnergy < 0)
      {
      return true;
      }
    if (deltaEnergy == 0)
      {
      return false;
      }
    else
      {
      double proba = std::exp(-(deltaEnergy) / this->m_Parameters[0]);
      if ((m_Generator->GetIntegerVariate() % 10000) < proba * 10000)
        {
        return true;
        }
      }
    return false;
  }

  /** Methods to cancel random effects.*/
  void InitializeSeed(int seed)
  {
    m_Generator->SetSeed(seed);
  }
  void InitializeSeed()
  {
    m_Generator->SetSeed();
  }

protected:
  MRFOptimizerMetropolis()
    {
    this->m_NumberOfParameters = 1;
    this->m_Parameters.SetSize(1);
    this->m_Parameters[0] = 1.0;
    m_Generator = RandomGeneratorType::GetInstance();
    m_Generator->SetSeed();
    }
  ~MRFOptimizerMetropolis() override {}
  RandomGeneratorType::Pointer m_Generator;
};

}

#endif
