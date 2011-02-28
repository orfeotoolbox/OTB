/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSinclairToCoherencyFunctor_h
#define __otbSinclairToCoherencyFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCoherencyFunctor
 *  \brief Construct the fully polarimetric coherency matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *   channel #0 : \f$ (S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 *   channel #1 : \f$ (S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *   channel #2 : \f$ (S_{hh}+S_{vv}).(S_{hv}+S_{vh})^{*} \f$
 *   channel #3 : \f$ (S_{hh}+S_{vv}).(j(S_{hv}-S_{vh}))^{*} \f$
 *   channel #4 : \f$ (S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *   channel #5 : \f$ (S_{hh}-S_{vv}).(S_{hv}+S_{vh})^{*} \f$
 *   channel #6 : \f$ (S_{hh}-S_{vv}).(j(S_{hv}-S_{vh}))^{*} \f$
 *   channel #7 : \f$ (S_{hv}+S_{vh}).(S_{hv}+S_{vh})^{*} \f$
 *   channel #8 : \f$ (S_{hv}+S_{vh}).(j(S_{hv}-S_{vh}))^{*} \f$
 *   channel #9 : \f$ j(S_{hv}-S_{vh}).(j(S_{hv}-S_{vh}))^{*} \f$
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCovarianceFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyFunctor
 *  \sa SinclairToReciprocalCovarianceFunctor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCoherencyFunctor
{
public:
  /** Some typedefs. */
  typedef double                                   RealType;
  typedef std::complex <RealType>                  ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    TOutput result;

    result.SetSize(NumberOfComponentsPerPixel);

    const ComplexType HHPlusVV = static_cast<ComplexType>(Shh + Svv);
    const ComplexType VVMinusVV = static_cast<ComplexType>(Shh - Svv);
    const ComplexType HVPlusHV     = static_cast<ComplexType>( Shv + Svh);
    const ComplexType jHVMinusHV     = static_cast<ComplexType>( Shv - Svh) * vcl_complex<RealType>(0.0, 1.0);

    result[0] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(HHPlusVV) );
    result[1] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(VVMinusVV) );
    result[2] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(HVPlusHV) );
    result[3] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(jHVMinusHV) );
    result[4] = static_cast<OutputValueType>( VVMinusVV * vcl_conj(VVMinusVV) );
    result[5] = static_cast<OutputValueType>( VVMinusVV * vcl_conj(HVPlusHV) );
    result[6] = static_cast<OutputValueType>( VVMinusVV * vcl_conj(jHVMinusHV) );
    result[7] = static_cast<OutputValueType>( HVPlusHV * vcl_conj(HVPlusHV) );
    result[8] = static_cast<OutputValueType>( HVPlusHV * vcl_conj(jHVMinusHV) );
    result[9] = static_cast<OutputValueType>( jHVMinusHV * vcl_conj(jHVMinusHV) );

    result /= 2.0;

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToCoherencyFunctor() {}

  /** Destructor */
  virtual ~SinclairToCoherencyFunctor() {}

protected:


private:
    itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 10);
};

} // namespace Functor
} // namespace otb

#endif
