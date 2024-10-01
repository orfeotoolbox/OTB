/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbMRFEnergyGaussian_h
#define otbMRFEnergyGaussian_h

#include "otbMRFEnergy.h"

#include "otbMath.h"

namespace otb
{
/**
 * \class MRFEnergyGaussian
 * \brief This is the implementation of the Gaussian model for Markov classification.
 *
 * This is the implementation of the Gaussian model for Markov classification,
 * to be used for data fidelity. Energy is:
 *
 * \f[  U(x_s, x_t) = (x_s-x_t)^2 \f]
 *
 * with
 * - \f$ x_s \f$ the label on site s
 * - \f$ y_s \f$ the value on the reference image
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

template <class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyGaussian : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyGaussian Self;
  typedef MRFEnergy<TInput1, TInput2> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;

  typedef itk::Array<double> ParametersType;

  itkTypeMacro(MRFEnergyGaussian, MRFEnergy);

  itkNewMacro(Self);

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) override
  {
    return vnl_math_sqr((static_cast<double>(value1)) - (static_cast<double>(value2)));
  }

protected:
  // The constructor and destructor.
  MRFEnergyGaussian()
  {
    this->m_NumberOfParameters = 0;
    this->m_Parameters.SetSize(this->m_NumberOfParameters);
  };
  ~MRFEnergyGaussian() override
  {
  }
};
}

#endif
