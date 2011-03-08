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
#ifndef __otbSinclairToCovarianceFunctor_h
#define __otbSinclairToCovarianceFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCovarianceFunctor
 *  \brief Construct the fully polarimetric covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *   channel #0 : \f$ S_{hh}.S_{hh}^{*} \f$
 *   channel #1 : \f$ S_{hh}.S_{hv}^{*} \f$
 *   channel #2 : \f$ S_{hh}.S_{vh}^{*} \f$
 *   channel #3 : \f$ S_{hh}.S_{vv}^{*} \f$
 *   channel #4 : \f$ S_{hv}.S_{hv}^{*} \f$
 *   channel #5 : \f$ S_{hv}.S_{vh}^{*} \f$
 *   channel #6 : \f$ S_{hv}.S_{vv}^{*} \f$
 *   channel #7 : \f$ S_{vh}.S_{vh}^{*} \f$
 *   channel #8 : \f$ S_{vh}.S_{vv}^{*} \f$
 *   channel #9 : \f$ S_{vv}.S_{vv}^{*} \f$
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyFunctor
 *  \sa SinclairToReciprocalCovarianceFunctor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCovarianceFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv,
                             const TInput3& Svh, const TInput4& Svv)
  {
    TOutput result;

    result.SetSize(m_NumberOfComponentsPerPixel);

    result[0] = static_cast<OutputValueType>( std::norm(Shh) );
    result[1] = static_cast<OutputValueType>( static_cast<ComplexType>(Shh)*vcl_conj(static_cast<ComplexType>(Shv)) );
    result[2] = static_cast<OutputValueType>( static_cast<ComplexType>(Shh)*vcl_conj(static_cast<ComplexType>(Svh)) );
    result[3] = static_cast<OutputValueType>( static_cast<ComplexType>(Shh)*vcl_conj(static_cast<ComplexType>(Svv)) );
    result[4] = static_cast<OutputValueType>( std::norm(Shv) );
    result[5] = static_cast<OutputValueType>( static_cast<ComplexType>(Shv)*vcl_conj(static_cast<ComplexType>(Svh)) );
    result[6] = static_cast<OutputValueType>( static_cast<ComplexType>(Shv)*vcl_conj(static_cast<ComplexType>(Svv)) );
    result[7] = static_cast<OutputValueType>( std::norm(Svh) );
    result[8] = static_cast<OutputValueType>( static_cast<ComplexType>(Svh)*vcl_conj(static_cast<ComplexType>(Svv)) );
    result[9] = static_cast<OutputValueType>( std::norm(Svv) );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToCovarianceFunctor() {}

  /** Destructor */
  virtual ~SinclairToCovarianceFunctor() {}

protected:


private:
    itkStaticConstMacro(m_NumberOfComponentsPerPixel, unsigned int, 10);

};

} // namespace Functor
} // namespace otb

#endif
