/*
 * Copyright (C) 2008 Jan Wegner
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

#ifndef otbMRFEnergyFisherClassification_h
#define otbMRFEnergyFisherClassification_h

#include "otbMRFEnergy.h"
#include "otbMath.h"
#include "otb_boost_math_gamma_header.h"

namespace otb
{

/**
 * \class MRFEnergyFisherClassification
 * \brief Fisher model for Markov classification
 *
 * This is the implementation of the Fisher model for Markov classification
 * to be used for the fidelity (likelihood) term for the classification.
 *
 * This class is meant to be used in the MRF framework with the
 * otb::MarkovRandomFieldFilter
 *
 *
 * \ingroup OTBMarkov
 */
template <class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyFisherClassification : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyFisherClassification Self;
  typedef MRFEnergy<TInput1, TInput2> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;
  typedef itk::Array<double>                    ParametersType;

  itkNewMacro(Self);
  itkTypeMacro(MRFEnergyFisherClassification, MRFEnergy);

  void SetNumberOfParameters(const unsigned int nParameters) override
  {
    Superclass::SetNumberOfParameters(nParameters);
    this->m_Parameters.SetSize(nParameters);
    this->Modified();
  }

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) override
  {
    if ((unsigned int)value2 >= this->GetNumberOfParameters() / 3)
    {
      itkExceptionMacro(<< "Number of parameters does not correspond to number of classes");
    }
    double val1 = static_cast<double>(value1);
    double mu   = this->m_Parameters[3 * value2];
    double l    = this->m_Parameters[3 * value2 + 1];
    double m    = this->m_Parameters[3 * value2 + 2];

    double result = -std::log((boost::math::tgamma(l + m) / (boost::math::tgamma(l) * boost::math::tgamma(m))) * (2 / (mu)) * (std::sqrt(l / m)) *
                              ((std::pow((std::sqrt(l / m) * (val1 / mu)), ((2 * l) - 1))) /
                               (std::pow(1 + (std::sqrt(l / m) * (val1 / mu) * std::sqrt(l / m) * (val1 / mu)), (l + m)))));

    return result;
  }

protected:
  // The constructor and destructor.
  MRFEnergyFisherClassification(){};
  ~MRFEnergyFisherClassification() override{};
};
}
#endif
