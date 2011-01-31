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
#ifndef __otbSinclairToCircularCovarianceMatrixFunctor_h
#define __otbSinclairToCircularCovarianceMatrixFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCircularCovarianceMatrixFunctor
 *  \brief Construct the fully polarimetric circular covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *   channel #0 : \f$ S_{ll}.S_{ll}^{*} \f$
 *   channel #1 : \f$ S_{ll}.S_{lr}^{*} \f$
 *   channel #2 : \f$ S_{ll}.S_{rl}^{*} \f$
 *   channel #3 : \f$ S_{ll}.S_{rr}^{*} \f$
 *   channel #4 : \f$ S_{lr}.S_{lr}^{*} \f$
 *   channel #5 : \f$ S_{lr}.S_{rl}^{*} \f$
 *   channel #6 : \f$ S_{lr}.S_{rr}^{*} \f$
 *   channel #7 : \f$ S_{rl}.S_{rl}^{*} \f$
 *   channel #8 : \f$ S_{rl}.S_{rr}^{*} \f$
 *   channel #9 : \f$ S_{rr}.S_{rr}^{*} \f$
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCircularCovarianceMatrixFunctor
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

    result.SetSize(m_NumberOfComponentsPerPixel);
    ComplexType jShv = static_cast<ComplexType>(Shv) * vcl_complex<RealType>(0.0,1.0);
    ComplexType jSvh = static_cast<ComplexType>(Svh) * vcl_complex<RealType>(0.0,1.0);

    ComplexType Sll = static_cast<ComplexType>( 0.5 * (-Shh-jShv-jSvh+Svv) );
    ComplexType Slr = static_cast<ComplexType>( 0.5 * (-Shh+jShv-jSvh+Svv) );
    ComplexType Srl = static_cast<ComplexType>( 0.5 * (-Shh-jShv+jSvh-Svv) );
    ComplexType Srr = static_cast<ComplexType>( 0.5 * (-Shh+jShv+jSvh+Svv) );

    ComplexType conjSll = vcl_conj(Sll);
    ComplexType conjSlr = vcl_conj(Slr);
    ComplexType conjSrl = vcl_conj(Srl);
    ComplexType conjSrr = vcl_conj(Srr);

    result[0]  = static_cast<OutputValueType>( Sll * conjSll  );
    result[1]  = static_cast<OutputValueType>( Sll * conjSlr  );
    result[2]  = static_cast<OutputValueType>( Sll * conjSrl  );
    result[3]  = static_cast<OutputValueType>( Sll * conjSrr  );
    result[4]  = static_cast<OutputValueType>( Slr * conjSlr  );
    result[5]  = static_cast<OutputValueType>( Slr * conjSrl  );
    result[6]  = static_cast<OutputValueType>( Slr * conjSrr  );
    result[7]  = static_cast<OutputValueType>( Srl * conjSrl  );
    result[8]  = static_cast<OutputValueType>( Srl * conjSrr  );
    result[9]  = static_cast<OutputValueType>( Srr * conjSrr  );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToCircularCovarianceMatrixFunctor() : m_NumberOfComponentsPerPixel(10) {}

  /** Destructor */
  virtual ~SinclairToCircularCovarianceMatrixFunctor() {}

protected:


private:
    unsigned int m_NumberOfComponentsPerPixel;
};

} // namespace Functor
} // namespace otb

#endif
