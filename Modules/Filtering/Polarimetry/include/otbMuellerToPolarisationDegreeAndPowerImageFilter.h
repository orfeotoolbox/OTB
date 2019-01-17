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


#ifndef otbMuellerToPolarisationDegreeAndPowerImageFilter_h
#define otbMuellerToPolarisationDegreeAndPowerImageFilter_h

#include "itkNumericTraits.h"
#include "itkMatrix.h"
#include "itkVector.h"
#include "otbMath.h"

#include "otbFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class MuellerToPolarisationDegreeAndPowerFunctor
 * \brief Evaluate the  min and max polarisation degree and min and max power
 *   from the Mueller image
 *
 * The order of the channels of the input image corresponds to :
 * \f$ \begin{pmatrix}
 * {channel #0 }&{channel #1 }&{channel #2 }&{channel #3 } \\
 * {channel #4 }&{channel #5 }&{channel #6 }&{channel #7 } \\
 * {channel #8 }&{channel #9 }&{channel #10}&{channel #11} \\
 * {channel #12}&{channel #13}&{channel #14}&{channel #15} \\
 * \end{pmatrix} \f$
 *
 * The class process step by step while \f$ \tau <= 45 \f$ and for each \f$ \tau \f$, while \f$ \psi <= 90 \f$ :
 * 1. Define the incident Stokes vector:
 * - \f$ Si_{0} = 1 \f$
 * - \f$ Si_{1} = \cos{\psi * \frac{\pi}{90}} * \cos{\tau *  \frac{\pi}{90}} \f$
 * - \f$ Si_{2} = \sin{\psi * \frac{\pi}{90}} * \cos{\tau *  \frac{\pi}{90}} \f$
 * - \f$ Si_{3} = \sin{\tau * \frac{\pi}{90}} \f$
 *
 * 2. Evaluate the received Stokes vector :
 * - \f$ Sr = Si * MuellerMatrix \f$
 *
 * 3. Evaluate power \f$ P \f$  and polarisation degree \f$ DegP \f$:
 * - \f$ P = \max{0, Sr_0} \f$
 * - \f$ DegP =  \sqrt{Sr_{1}^{2} + Sr_{2}^{2} + Sr_{3}^{2}} / Sr_{0} \f$
 *
 * 4. Keep the smallest and the biggest power (\f$ P_{min}, P_{max} \f$) and polarisation degree (\f$ DegP_{min}, DegP_{max} \f$) ).
 *
 * Output value are:
 * - channel #0 : \f$ P_{min} \f$
 * - channel #1 : \f$ P_{max} \f$
 * - channel #2 : \f$ DegP_{min} \f$
 * - channel #3 : \f$ DegP_{max} \f$
 *
 * Use otb::MuellerToPolarisationDegreeAndPowerImageFilter to apply
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa MuellerToCircularPolarisationImageFilter
 * \sa MuellerToReciprocalCovarianceFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class MuellerToPolarisationDegreeAndPowerFunctor
{
public:
  typedef typename TOutput::ValueType               OutputValueType;
  typedef itk::Matrix<double, 4, 4>                 MuellerMatrixType;
  typedef itk::Vector<double, 4>                   StokesVectorType;

  inline void operator()(TOutput& result, const TInput& Mueller) const
  {
    double P;
    double deg_pol;
    double tau;
    double psi;
    StokesVectorType Si;
    StokesVectorType Sr;

    double l_PowerMin(itk::NumericTraits<double>::max());
    double l_PowerMax(itk::NumericTraits<double>::min());
    double l_PolarisationDegreeMin(itk::NumericTraits<double>::max());
    double l_PolarisationDegreeMax(itk::NumericTraits<double>::min());

    MuellerMatrixType muellerMatrix;
    muellerMatrix[0][0] = Mueller[0];
    muellerMatrix[0][1] = Mueller[1];
    muellerMatrix[0][2] = Mueller[2];
    muellerMatrix[0][3] = Mueller[3];
    muellerMatrix[1][0] = Mueller[4];
    muellerMatrix[1][1] = Mueller[5];
    muellerMatrix[1][2] = Mueller[6];
    muellerMatrix[1][3] = Mueller[7];
    muellerMatrix[2][0] = Mueller[8];
    muellerMatrix[2][1] = Mueller[9];
    muellerMatrix[2][2] = Mueller[10];
    muellerMatrix[2][3] = Mueller[11];
    muellerMatrix[3][0] = Mueller[12];
    muellerMatrix[3][1] = Mueller[13];
    muellerMatrix[3][2] = Mueller[14];
    muellerMatrix[3][3] = Mueller[15];

    tau = -45.0;
    while (tau < 46.0)
      {
        psi = -90.0;
        while (psi < 91.0)
          {
            // Define the incident Stokes vector
            Si[0] = 1.0;
            Si[1] = cos(psi * m_PI_90) * cos(tau * m_PI_90);
            Si[2] = sin(psi * m_PI_90) * cos(tau * m_PI_90);
            Si[3] = sin(tau * m_PI_90);

            // Evaluate the received Stokes vector
            Sr = muellerMatrix * Si;

            //Evaluate Power and Polarisation degree
            P = Sr[0];

            if (P < m_Epsilon)
              {
                deg_pol = 0.;
              }
            else
              {
                deg_pol = std::sqrt(Sr[1] * Sr[1] + Sr[2] * Sr[2] + Sr[3] * Sr[3]) / Sr[0];
              }

            if (P > l_PowerMax)
              {
                l_PowerMax = P;
              }
            else
              {
                l_PowerMin = P;
              }

            if (deg_pol > l_PolarisationDegreeMax)
              {
                l_PolarisationDegreeMax = deg_pol;
              }
            else
              {
                l_PolarisationDegreeMin = deg_pol;
              }
            psi += 5.0;
          }
        tau += 5.0;
      }

    result[0] = l_PowerMin;
    result[1] = l_PowerMax;
    result[2] = l_PolarisationDegreeMin;
    result[3] = l_PolarisationDegreeMax;
    }

    constexpr size_t OutputSize(...) const
    {
      // Size of the result
      return 4;
  }
private:
  static constexpr double m_Epsilon = 1e-6;
  static constexpr double m_PI_90   = 2 * CONST_PI_180;
};
} // namespace Functor

/**
 * \typedef MuellerToPolarisationDegreeAndPowerImageFilter
 * \brief Applies otb::Functor::MuellerToPolarisationDegreeAndPowerFunctor
 * \sa otb::Functor::MuellerToPolarisationDegreeAndPowerFunctor
 *
 * Set inputs with:
 * \code
 * SetInput<0>(inputPtr);
 * \endcode
 *
 * \ingroup OTBPolarimetry
 */
template <typename TInputImage, typename TOutputImage>
using MuellerToPolarisationDegreeAndPowerImageFilter =
    FunctorImageFilter<Functor::MuellerToPolarisationDegreeAndPowerFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;


} // end namespace otb

#endif
