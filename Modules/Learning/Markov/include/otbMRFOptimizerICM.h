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


#ifndef otbMRFOptimizerICM_h
#define otbMRFOptimizerICM_h

#include "otbMRFOptimizer.h"

namespace otb
{
/**
 * \class MRFOptimizerICM
 * \brief This is the optimizer class implementing the ICM algorithm
 *
 * This is one optimizer to be used in the MRF framework. This optimizer
 * follows the ICM algorithm to accept of reject the value proposed by the sampler
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */
class ITK_EXPORT MRFOptimizerICM : public MRFOptimizer
{
public:
  typedef MRFOptimizerICM               Self;
  typedef MRFOptimizer                  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(MRFOptimizerICM, MRFOptimizer);

  inline bool Compute(double deltaEnergy) override
  {
    if (deltaEnergy < 0)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

protected:
  MRFOptimizerICM()
  {
  }
  ~MRFOptimizerICM() override
  {
  }
};
}

#endif
