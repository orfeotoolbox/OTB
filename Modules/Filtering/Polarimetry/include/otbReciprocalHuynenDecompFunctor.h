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


#ifndef otbReciprocalHuynenDecompFunctor_h
#define otbReciprocalHuynenDecompFunctor_h

namespace otb
 {

namespace Functor {

/** \class ReciprocalHuynenDecompFunctor
 *
 * \brief Evaluate the Huynen decomposition from the reciprocal Sinclair matrix image.
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalHuynenDecompFunctor
{
public:

  typedef typename TOutput::ValueType   OutputValueType;


  inline void operator()( TOutput & result, const TInput & Covariance ) const
    {
    OutputValueType A0 = static_cast<OutputValueType>(Covariance[0].real() / 2.0);
    OutputValueType B0 = static_cast<OutputValueType>((Covariance[3] + Covariance[5]).real() / 2.0);
    OutputValueType B = static_cast<OutputValueType>(Covariance[3].real() - B0);
    OutputValueType C = static_cast<OutputValueType>(Covariance[1].real());
    OutputValueType D = static_cast<OutputValueType>(-Covariance[1].imag());
    OutputValueType E = static_cast<OutputValueType>(Covariance[4].real());
    OutputValueType F = static_cast<OutputValueType>(Covariance[4].imag());
    OutputValueType G = static_cast<OutputValueType>(Covariance[2].imag());
    OutputValueType H = static_cast<OutputValueType>(Covariance[2].real());

    result[0] = A0;
    result[1] = B0;
    result[2] = B;
    result[3] = C;
    result[4] = D;
    result[5] = E;
    result[6] = F;
    result[7] = G;
    result[8] = H;
    }

   constexpr size_t OutputSize(...) const
  {
    // Size of the result
    return 9;
  }

   /** Constructor */
   ReciprocalHuynenDecompFunctor() : m_Epsilon(1e-6) {}

   /** Destructor */
   virtual ~ReciprocalHuynenDecompFunctor() {}

private:
   const double m_Epsilon;
};
} // end namespace functor
} // end namespace otb

#endif
