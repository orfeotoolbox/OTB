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


#ifndef otbMRFEnergyPotts_h
#define otbMRFEnergyPotts_h

#include "otbMRFEnergy.h"

namespace otb
{
/**
 * \class MRFEnergyPotts
 * \brief This is the implementation of the Potts model for Markov classification.
 *
 * This is the implementation of the Potts model for Markov classification, to be used for
 * regularization. Energy is:
 * \f[  U(x_s, x_t) = -\beta \textrm{ if } x_s = x_t \f]
 * \f[  U(x_s, x_t) = +\beta \textrm{ if } x_s \neq x_t \f]
 * with
 * - \f$ x_s \f$ the label on site s
 * - \f$ y_s \f$ the value on the reference image
 * - \f$ \sigma^2_{x_s} \f$ the noise variance
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

template <class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyPotts : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyPotts Self;
  typedef MRFEnergy<TInput1, TInput2> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;

  typedef itk::Array<double> ParametersType;

  itkTypeMacro(MRFEnergyPotts, MRFEnergy);

  itkNewMacro(Self);

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) override
  {
    if (value1 != value2)
    {
      return this->m_Parameters[0];
    }
    else
    {
      return -this->m_Parameters[0];
    }
  }

protected:
  // The constructor and destructor.
  MRFEnergyPotts()
  {
    this->m_NumberOfParameters = 1;
    this->m_Parameters.SetSize(this->m_NumberOfParameters);
    this->m_Parameters[0] = 1.0;
  };
  ~MRFEnergyPotts() override
  {
  }
};
}

#endif
