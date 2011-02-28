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
#ifndef __otbSinclairToReciprocalCircularCovarianceMatrixFunctor_h
#define __otbSinclairToReciprocalCircularCovarianceMatrixFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCircularCovarianceMatrixFunctor
 *  \brief Construct the fully reciprocal polarimetric circular covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *   channel #0 : \f$ S_{ll}.S_{ll}^{*} \f$
 *   channel #1 : \f$ S_{ll}.S_{lr}^{*} \f$
 *   channel #2 : \f$ S_{ll}.S_{rr}^{*} \f$
 *   channel #3 : \f$ S_{lr}.S_{lr}^{*} \f$
 *   channel #4 : \f$ S_{lr}.S_{rr}^{*} \f$
 *   channel #5 : \f$ S_{rr}.S_{rr}^{*} \f$
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCoherencyFunctor
 *  \sa SinclairToCovarianceFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyFunctor
 *  \sa SinclairToReciprocalCovarianceFunctor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToReciprocalCircularCovarianceMatrixFunctor
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
    const ComplexType j2Shv = static_cast<ComplexType>(Shv) * vcl_complex<RealType>(0.0, 2.0);

    const ComplexType Sll = static_cast<ComplexType>( 0.5 * (-Shh-j2Shv+Svv) );
    const ComplexType Slr = static_cast<ComplexType>( 0.5 * (-Shh+Svv) );
    const ComplexType Srr = vcl_conj(Sll);

    const ComplexType conjSll = vcl_conj(Sll);
    const ComplexType conjSlr = vcl_conj(Slr);
    const ComplexType conjSrr = vcl_conj(Srr);

    result[0]  = static_cast<OutputValueType>( Sll * conjSll  );
    result[1]  = static_cast<OutputValueType>( Sll * conjSlr  );
    result[2]  = static_cast<OutputValueType>( Sll * conjSrr  );
    result[3]  = static_cast<OutputValueType>( Slr * conjSlr  );
    result[4]  = static_cast<OutputValueType>( Slr * conjSrr  );
    result[5]  = static_cast<OutputValueType>( Srr * conjSrr  );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToReciprocalCircularCovarianceMatrixFunctor() : m_NumberOfComponentsPerPixel(6) {}

  /** Destructor */
  virtual ~SinclairToReciprocalCircularCovarianceMatrixFunctor() {}

protected:


private:
    unsigned int m_NumberOfComponentsPerPixel;
};

} // namespace Functor
} // namespace otb

#endif
