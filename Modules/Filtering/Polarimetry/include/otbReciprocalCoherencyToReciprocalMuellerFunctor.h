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


#ifndef otbReciprocalCoherencyToReciprocalMuellerFunctor_h
#define otbReciprocalCoherencyToReciprocalMuellerFunctor_h

namespace otb
 {

namespace Functor {

/** \class CoherencyToReciprocalMuellerFunctor
 * \brief Evaluate the reciprocal Mueller matrix from the reciprocal coherency matrix image
 *
 * Outpus are:
 * - channel #0 : \f$ 0.5*( C_{11}+C_{22}+C_{33} ) \f$
 * - channel #1 : \f$ Re(C_{12}) + Im(C_{22}) \f$
 * - channel #2 : \f$ Re(C_{13}) \f$
 * - channel #3 : \f$ Im(C_{23}) \f$
 * - channel #4 : \f$ Re(C_{12}) \f$
 * - channel #5 : \f$ 0.5*( C_{11}+C_{22}-C_{33} ) \f$
 * - channel #6 : \f$ Re(C_{23}) \f$
 * - channel #7 : \f$ Im(C_{13}) \f$
 * - channel #8 : \f$ -Re(C_{13}) \f$
 * - channel #9 : \f$ -Re(C_{23}) \f$
 * - channel #10 : \f$ 0.5.Re(VAL1) \f$
 * - channel #11 : \f$ 0.5.Im(VAL0) \f$
 * - channel #12 : \f$ Im(C_{23}) \f$
 * - channel #13 : \f$ Im(C_{13}) \f$
 * - channel #14 : \f$ 0.5.Im(VAL1^{*}) \f$
 * - channel #15 : \f$ 0.5.Re(VAL0) \f$
 *
 * With:
 * VAL0 = C_{33}+C_{12}-C_{11}-(C_{12}-C_{22})^{*}
 * VAL1 = -C_{33}+C_{12}-C_{11}-(C_{12}-C_{22})^{*}
 *
 * Where Coherency is the input pixel and contains:
 * - channel #0 : \f$ 0.5*(S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 * - channel #1 : \f$ 0.5*(S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 * - channel #2 : \f$ 0.5*(S_{hh}+S_{vv}).(2*S_{hv})^{*} \f$
 * - channel #3 : \f$ 0.5*(S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 * - channel #4 : \f$ 0.5*(S_{hh}-S_{vv}).(2*S_{hv})^{*} \f$
 * - channel #5 : \f$ 0.5*(2*S_{hv}).(2*S_{hv})^{*} \f$
 *
 * The output pixel has 16 channels
 * Element are stored from left to right, line by line.
 *
 * \ingroup SARPolarimetry
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalCoherencyToReciprocalMuellerFunctor
{
public:
  typedef typename std::complex <double>         ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline void operator()( TOutput & result, const TInput & Coherency ) const
  {
    const double T1 = static_cast<double>(Coherency[0].real());
    const double T2 = static_cast<double>(Coherency[3].real());
    const double T3 = static_cast<double>(Coherency[5].real());

    ComplexType VAL4 = static_cast<ComplexType>( (Coherency[1] - Coherency[3]) );
    ComplexType VAL5 = static_cast<ComplexType>( (Coherency[1] - Coherency[0]) );
    ComplexType VAL0 = static_cast<ComplexType>( Coherency[5] ) + VAL5 - std::conj(VAL4);
    ComplexType VAL1 = static_cast<ComplexType>( -Coherency[5] ) + VAL5 - std::conj(VAL4);

    result[0] = 0.5*(T1+T2+T3);
    result[1] = static_cast<double>( Coherency[1].real()+Coherency[3].imag() );
    result[2] = static_cast<double>( Coherency[2].real() );
    result[3] = static_cast<double>( Coherency[4].imag() );
    result[4] = static_cast<double>( Coherency[1].real() );
    result[5] = 0.5*(T1+T2-T3);
    result[6] = static_cast<double>( Coherency[4].real() );
    result[7] = static_cast<double>( Coherency[2].imag() );
    result[8] = static_cast<double>( -Coherency[2].real() );
    result[9] = static_cast<double>( -Coherency[4].real() );
    result[10] = static_cast<double>( 0.5*VAL1.real() );
    result[11] = static_cast<double>( 0.5*VAL0.imag() );
    result[12] = static_cast<double>( Coherency[4].imag() );
    result[13] = static_cast<double>( Coherency[2].imag() );
    result[14] = static_cast<double>( 0.5*std::conj(VAL1).imag() );
    result[15] = static_cast<double>( 0.5*VAL0.real() );
  }

  constexpr size_t OutputSize(...) const
  {
    // Size of the reciprocal mueller matrix
    return 16;
  }

   /** Constructor */
   ReciprocalCoherencyToReciprocalMuellerFunctor() {}

   /** Destructor */
   virtual ~ReciprocalCoherencyToReciprocalMuellerFunctor() {}
};
} // end namespace functor
} // end namespace otb

#endif
