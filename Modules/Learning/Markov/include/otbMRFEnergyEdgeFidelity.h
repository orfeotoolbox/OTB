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


#ifndef otbMRFEnergyEdgeFidelity_h
#define otbMRFEnergyEdgeFidelity_h

#include "otbMRFEnergy.h"

namespace otb
{
/**
 * \class MRFEnergyEdgeFidelity
 * \brief This is the implementation of an edge preserving model for Markov denoising.
 *
 * This is the implementation of an edge fidelity model for Markov denoising, to be used for
 * regularization. Energy is:
 * \f[  \sum_{t \in \mathcal{V}_s} U(x_s, x_t) = \Phi(x_s-x_t) \f]
 * with
 * - \f$ x_s \f$ the label on site s
 * - \f$ x_t \f$ the label on site t, a neighbor of s
 * - \f$ \Phi \f$ an edge preserving function:
 *     \f[ \Phi(u) = \frac{u^2}{1+u^2} \f]
 *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
 *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
*/

template <class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyEdgeFidelity : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyEdgeFidelity Self;
  typedef MRFEnergy<TInput1, TInput2> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ConstNeighborhoodIterator<TInput1> NeighborhoodIterator;
  typedef typename TInput1::PixelType             InputImagePixelType;
  typedef typename TInput2::PixelType             LabelledImagePixelType;

  itkNewMacro(Self);

  itkTypeMacro(MRFEnergyEdgeFidelity, MRFEnergy);

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) override
  {
    double val1 = static_cast<double>(value1);
    double val2 = static_cast<double>(value2);

    return vnl_math_sqr((val1 - val2)) / (1 + vnl_math_sqr(val1 - val2));
  }

protected:
  // The constructor and destructor.
  MRFEnergyEdgeFidelity(){};
  ~MRFEnergyEdgeFidelity() override
  {
  }
};
}

#endif
