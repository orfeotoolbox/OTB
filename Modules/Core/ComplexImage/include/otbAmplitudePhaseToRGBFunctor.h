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


#ifndef otbAmplitudePhaseToRGBFunctor_h
#define otbAmplitudePhaseToRGBFunctor_h

#include "otbScalarToRainbowRGBPixelFunctor.h"
#include "otbMath.h"

namespace otb
{

namespace Functor
{
/**
 * \class AmplitudePhaseToRGBFunctor
 * \brief Function object to compute a color representation of a radar image.
 *
 * This class is useful for visualizing radar images with a combination of
 * the phase and the amplitude at the same time.
 *
 * The amplitude is used to represent the intensity of the pixel and the phase
 * to represent it's hue.
 *
 * Amplitude and phase can be obtained using the itk::ComplexToModulusImageFilter
 * and itk::ComplexToPhaseImageFilter for example.
 *
 *
 * \ingroup OTBComplexImage
 */
template<class TInput1, class TInput2 = TInput1, class TInput3 = TInput1, class TOutput = TInput1>
class ITK_EXPORT AmplitudePhaseToRGBFunctor
{
public:
  typedef TOutput                          RGBPixelType;
  typedef typename RGBPixelType::ValueType RGBComponentType;
  typedef HSVToRGBFunctor<RGBPixelType>    HSVToRGBFunctorType;
  typedef TInput1                          ScalarType;

  AmplitudePhaseToRGBFunctor()
    {
    m_Minimum = 0;
    m_Maximum = itk::NumericTraits<ScalarType>::max();
    };
  ~AmplitudePhaseToRGBFunctor(){}

  void SetMaximum(ScalarType max)
  {
    this->m_Maximum = max;
  }

  void SetMinimum(ScalarType min)
  {
    this->m_Minimum = min;
  }

  inline TOutput operator ()(const TInput1& amplitude, const TInput2& coherence, const TInput3& phase) const
  {
//           std::cout << amplitude << " - " << phase << std::endl;
    double hinc;
    hinc = 0.6 / (CONST_2PI);

    double hue, sat, val;

    hue = 0.6 - (phase + CONST_PI) * hinc;
    sat = 0.6 * coherence + 0.3;
    val = itk::NumericTraits<RGBComponentType>::max() / 2
          * ((amplitude - m_Minimum) / (m_Maximum - m_Minimum) + 1.0);

    if (amplitude < m_Minimum)
      {
      val = 0;
      }
    if (amplitude > m_Maximum)
      {
      val = itk::NumericTraits<RGBComponentType>::max();
      }

    return m_HSVToRGBFunctor(hue, sat, val);

  }
private:
  ScalarType          m_Maximum;
  ScalarType          m_Minimum;
  HSVToRGBFunctorType m_HSVToRGBFunctor;
};
}

}
#endif
