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
#include "otbSinclairToCovarianceMatrixFunctor.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCircularCovarianceMatrixFunctor
 *  \brief Construct the fully polarimetric circular covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ S_{ll}.S_{ll}^{*} \f$
 *  - channel #1 : \f$ S_{ll}.S_{lr}^{*} \f$
 *  - channel #2 : \f$ S_{ll}.S_{rl}^{*} \f$
 *  - channel #3 : \f$ S_{ll}.S_{rr}^{*} \f$
 *  - channel #4 : \f$ S_{lr}.S_{lr}^{*} \f$
 *  - channel #5 : \f$ S_{lr}.S_{rl}^{*} \f$
 *  - channel #6 : \f$ S_{lr}.S_{rr}^{*} \f$
 *  - channel #7 : \f$ S_{rl}.S_{rl}^{*} \f$
 *  - channel #8 : \f$ S_{rl}.S_{rr}^{*} \f$
 *  - channel #9 : \f$ S_{rr}.S_{rr}^{*} \f$
 *
 * With:
 * - \f$ S_{ll} = 0.5 * (-S_{hh}-i*S_{hv}-i*S_{vh}+S_{vv}) \f$
 * - \f$ S_{lr} = 0.5 * (-S_{hh}+i*S_{hv}-i*S_{vh}+S_{vv}) \f$
 * - \f$ S_{rl} = 0.5 * (-S_{hh}-i*S_{hv}+i*S_{vh}-S_{vv}) \f$
 * - \f$ S_{rr} = 0.5 * (-S_{hh}+i*S_{hv}+i*S_{vh}+S_{vv}) \f$
 *
 * Extract from Antennas for radar and communications Harold Mott p 317.
 *
 * The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuellerMatrixFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
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
  typedef SinclairToCovarianceMatrixFunctor<ComplexType, ComplexType, ComplexType, ComplexType, TOutput> SinclairToCovarianceFunctorType;
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    TOutput result;

    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vh = static_cast<ComplexType>(Svh);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    result.SetSize(m_NumberOfComponentsPerPixel);
    const ComplexType jS_hv = S_hv * ComplexType(0., 1.);
    const ComplexType jS_vh = S_vh * ComplexType(0., 1.);

    const ComplexType coef(0.5);

    const ComplexType Sll = coef*( -S_hh-jS_hv-jS_vh+S_vv );
    const ComplexType Slr = coef*( -S_hh+jS_hv-jS_vh-S_vv );
    const ComplexType Srl = coef*( -S_hh-jS_hv+jS_vh-S_vv );
    const ComplexType Srr = coef*( -S_hh+jS_hv+jS_vh+S_vv );

    //const ComplexType conjSll = vcl_conj(Sll);
    //const ComplexType conjSlr = vcl_conj(Slr);
    //const ComplexType conjSrl = vcl_conj(Srl);
    //const ComplexType conjSrr = vcl_conj(Srr);

    SinclairToCovarianceFunctorType funct;
    return ( funct( Sll, Slr, Srl, Srr ) );
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
