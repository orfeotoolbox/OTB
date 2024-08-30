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

#ifndef otbMRFEnergyGaussianClassification_h
#define otbMRFEnergyGaussianClassification_h

#include "otbMRFEnergy.h"
#include "otbMath.h"

namespace otb
{
/**
 * \class MRFEnergyGaussianClassification
 * \brief This is the implementation of the Gaussian model for Markov classification.
 *
 * This is the implementation of the Gaussian Energy model for Markov classification, to be used for
 * the fidelity term for classification. Energy is:
 * \f[   U(x_s / y_s) = \frac{(y_s+\mu_{x_s})^2}{2\sigma^2_{x_s}}+\log{\sqrt{2\pi}\sigma_{x_s}} \f]
 * with
 * - \f$ x_s \f$ the label on site s
 * - \f$ y_s \f$ the value on the reference image
 * - \f$ \mu_{x_s} \f$ and \f$ \sigma^2_{x_s} \f$ the mean and variance of label \f$ x_s \f$
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

template <class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyGaussianClassification : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyGaussianClassification Self;
  typedef MRFEnergy<TInput1, TInput2> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;
  typedef itk::Array<double>                    ParametersType;

  itkNewMacro(Self);

  itkTypeMacro(MRFEnergyGaussianClassification, MRFEnergy);

  void SetNumberOfParameters(const unsigned int nParameters) override
  {
    Superclass::SetNumberOfParameters(nParameters);
    this->m_Parameters.SetSize(nParameters);
    this->Modified();
  }

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) override
  {
    if ((unsigned int)value2 >= this->GetNumberOfParameters() / 2)
    {
      itkExceptionMacro(<< "Number of parameters does not correspond to number of classes");
    }
    double val1 = static_cast<double>(value1);

    double result =
        vnl_math_sqr(val1 - this->m_Parameters[2 * static_cast<int>(value2)]) / (2 * vnl_math_sqr(this->m_Parameters[2 * static_cast<int>(value2) + 1])) +
        std::log(std::sqrt(CONST_2PI) * this->m_Parameters[2 * static_cast<int>(value2) + 1]);

    return static_cast<double>(result);
  }

protected:
  // The constructor and destructor.
  MRFEnergyGaussianClassification(){};
  ~MRFEnergyGaussianClassification() override
  {
  }
};
}

#endif
