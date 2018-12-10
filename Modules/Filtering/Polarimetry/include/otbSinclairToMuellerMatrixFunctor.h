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

#ifndef otbSinclairToMuellerMatrixFunctor_h
#define otbSinclairToMuellerMatrixFunctor_h

#include <complex>

namespace otb
{
namespace Functor
{
/** \class SinclairToMuellerMatrixFunctor
 *  \brief Construct Mueller matrix with Sinclair matrix information.
 *  Elements of the Mueller matrix are extract from Antennas for radar and communications
 *  Harold Mott p 503
 *
 *  Output value are:
 *  - channel #0  : \f$ 0.5 * \mathcal{Re}( T_{xx}.T_{xx}^{*} + T_{xy}.T_{xy}^{*} + T_{yx}.T_{yx}^{*} + T_{yy}.T_{yy}^{*} ) \f$
 *  - channel #1  : \f$ 0.5 * \mathcal{Re}( T_{xx}.T_{xx}^{*} - T_{xy}.T_{xy}^{*} + T_{yx}.T_{yx}^{*} - T_{yy}.T_{yy}^{*} ) \f$
 *  - channel #2  : \f$ \mathcal{Re}( T_{xx}.T_{xy}^{*} + T_{yx}.T_{yy}^{*} ) \f$
 *  - channel #3  : \f$ \mathcal{Im}( T_{xx}.T_{xy}^{*} + T_{yx}.T_{yy}^{*} ) \f$
 *  - channel #4  : \f$ 0.5 * \mathcal{Re}( T_{xx}.T_{xx}^{*} + T_{xy}.T_{xy}^{*} - T_{yx}.T_{yx}^{*} - T_{yy}.T_{yy}^{*} ) \f$
 *  - channel #5  : \f$ 0.5 * \mathcal{Re}( T_{xx}.T_{xx}^{*} - T_{xy}.T_{xy}^{*} - T_{yx}.T_{yx}^{*} + T_{yy}.T_{yy}^{*} ) \f$
 *  - channel #6  : \f$ \mathcal{Re}( T_{xx}.T_{xy}^{*} - T_{yx}.T_{yy}^{*} ) \f$
 *  - channel #7  : \f$ \mathcal{Im}( T_{xx}.T_{xy}^{*} - T_{yx}.T_{yy}^{*} ) \f$
 *  - channel #8  : \f$ \mathcal{Re}( T_{xx}.T_{yx}^{*} + T_{xy}.T_{yy}^{*} ) \f$
 *  - channel #9  : \f$ \mathcal{Im}( T_{xx}.T_{yx}^{*} - T_{xy}.T_{yy}^{*} ) \f$
 *  - channel #10 : \f$ \mathcal{Re}( T_{xx}.T_{yy}^{*} + T_{xy}.T_{yx}^{*} ) \f$
 *  - channel #11 : \f$ \mathcal{Im}( T_{xx}.T_{yy}^{*} - T_{xy}.T_{yx}^{*} ) \f$
 *  - channel #12 : \f$ \mathcal{Re}( T_{xx}.T_{yx}^{*} + T_{xy}.T_{yy}^{*} ) \f$
 *  - channel #13 : \f$ \mathcal{Im}( T_{xx}.T_{yx}^{*} - T_{xy}.T_{yy}^{*} ) \f$
 *  - channel #14 : \f$ \mathcal{Re}( T_{xx}.T_{yy}^{*} + T_{xy}.T_{yx}^{*} ) \f$
 *  - channel #15 : \f$ \mathcal{Im}( T_{xx}.T_{yy}^{*} - T_{xy}.T_{yx}^{*} ) \f$
 *
 * With :
 * - \f$ T_{xx} = -S_{hh} \f$
 * - \f$ T_{xy} = -S_{hv} \f$
 * - \f$ T_{yx} = S_{vh} \f$
 * - \f$ T_{yy} = S_{vv} \f$
 *
 * Output is a not a complex. The output pixel has 16 channels : each element of the Mueller matrix.
 * The order of the channels corresponds to :
 * \f$  \begin{pmatrix}
 * {channel #0 }&{channel #1 }&{channel #2 }&{channel #3 } \\
 * {channel #4 }&{channel #5 }&{channel #6 }&{channel #7 } \\
 * {channel #8 }&{channel #9 }&{channel #10}&{channel #11} \\
 * {channel #12}&{channel #13}&{channel #14}&{channel #15} \\
 * \end{pmatrix}  \f$
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToMuellerMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  typedef double                                   RealType;

  inline void operator ()(TOutput & result, const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    const ComplexType Txx = static_cast<ComplexType>(-Shh);
    const ComplexType Txy = static_cast<ComplexType>(-Shv);
    const ComplexType Tyx = static_cast<ComplexType>(Svh);
    const ComplexType Tyy = static_cast<ComplexType>(Svv);

    const ComplexType conjTxx = std::conj(Txx);
    const ComplexType conjTxy = std::conj(Txy);
    const ComplexType conjTyx = std::conj(Tyx);
    const ComplexType conjTyy = std::conj(Tyy);

    result[0]  = static_cast<OutputValueType>( 0.5 * ( std::norm(Txx) + std::norm(Txy) + std::norm(Tyx) + std::norm(Tyy) ) );
    result[1]  = static_cast<OutputValueType>( 0.5 * ( std::norm(Txx) - std::norm(Txy) + std::norm(Tyx) - std::norm(Tyy) ) );
    result[2]  = static_cast<OutputValueType>( (Txx*conjTxy + Tyx*conjTyy).real()  );
    result[3]  = static_cast<OutputValueType>( (Txx*conjTxy + Tyx*conjTyy).imag()  );
    result[4]  = static_cast<OutputValueType>( 0.5 * (std::norm(Txx) + std::norm(Txy) - std::norm(Tyx) - std::norm(Tyy) ) );
    result[5]  = static_cast<OutputValueType>( 0.5 * (std::norm(Txx) - std::norm(Txy) - std::norm(Tyx) + std::norm(Tyy) ) );
    result[6]  = static_cast<OutputValueType>( (Txx*conjTxy - Tyx*conjTyy).real()  );
    result[7]  = static_cast<OutputValueType>( (Txx*conjTxy - Tyx*conjTyy).imag()  );
    result[8]  = static_cast<OutputValueType>( (Txx*conjTyx + Txy*conjTyy).real()  );
    result[9]  = static_cast<OutputValueType>( (Txx*conjTyx - Txy*conjTyy).real()  );
    result[10] = static_cast<OutputValueType>( (Txx*conjTyy + Txy*conjTyx).real()  );
    result[11] = static_cast<OutputValueType>( (Txx*conjTyy - Txy*conjTyx).imag()  );
    result[12] = static_cast<OutputValueType>( (conjTxx*Tyx + conjTxy*Tyy).imag()  );
    result[13] = static_cast<OutputValueType>( (conjTxx*Tyx - conjTxy*Tyy).imag()  );
    result[14] = static_cast<OutputValueType>( (conjTxx*Tyy + conjTxy*Tyx).imag()  );
    result[15] = static_cast<OutputValueType>( (Txx*conjTyy - Txy*conjTyx).real()  );
  }

  constexpr size_t OutputSize(...) const
  {
    // Size of mueller matrix
    return 16;
  }

  /** Constructor */
  SinclairToMuellerMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToMuellerMatrixFunctor() {}
};

} // namespace Functor
} // namespace otb

#endif
