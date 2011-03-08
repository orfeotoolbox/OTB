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
#ifndef __otbSinclairToReciprocalCovarianceFunctor_h
#define __otbSinclairToReciprocalCovarianceFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToReciprocalCovarianceFunctor
 *  \brief Construct the reciprocal fully polarimetric covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *   channel #0 : \f$ S_{hh}.S_{hh}^{*} \f$
 *   channel #1 : \f$ S_{hh}.S_{hv}^{*} \f$
 *   channel #2 : \f$ S_{hh}.S_{vv}^{*} \f$
 *   channel #3 : \f$ S_{hv}.S_{hv}^{*} \f$
 *   channel #4 : \f$ S_{hv}.S_{vv}^{*} \f$
 *   channel #5 : \f$ S_{vv}.S_{vv}^{*} \f$
 *
 * Output is a complex.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyFunctor
 *  \sa SinclairToCovarianceFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyFunctor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToReciprocalCovarianceFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    TOutput result;

    result.SetSize(NumberOfComponentsPerPixel);

    result[0] = static_cast<OutputValueType>( std::norm(Shh) );
    result[1] = static_cast<OutputValueType>( static_cast<ComplexType>(Shh)*vcl_conj(static_cast<ComplexType>(Shv)) );
    result[2] = static_cast<OutputValueType>( static_cast<ComplexType>(Shh)*vcl_conj(static_cast<ComplexType>(Svv)) );
    result[3] = static_cast<OutputValueType>( std::norm(Shv) );
    result[4] = static_cast<OutputValueType>( static_cast<ComplexType>(Shv)*vcl_conj(static_cast<ComplexType>(Svv)) );
    result[5] = static_cast<OutputValueType>( std::norm(Svv) );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToReciprocalCovarianceFunctor() {}

  /** Destructor */
  virtual ~SinclairToReciprocalCovarianceFunctor() {}

protected:


private:
    itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 6 );

};

} // namespace Functor
} // namespace otb

#endif
