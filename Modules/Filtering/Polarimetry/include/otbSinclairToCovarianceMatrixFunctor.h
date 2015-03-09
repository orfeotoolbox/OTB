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
#ifndef __otbSinclairToCovarianceMatrixFunctor_h
#define __otbSinclairToCovarianceMatrixFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCovarianceMatrixFunctor
 *  \brief Construct the fully polarimetric covariance matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ S_{hh}.S_{hh}^{*} \f$
 *  - channel #1 : \f$ S_{hh}.S_{hv}^{*} \f$
 *  - channel #2 : \f$ S_{hh}.S_{vh}^{*} \f$
 *  - channel #3 : \f$ S_{hh}.S_{vv}^{*} \f$
 *  - channel #4 : \f$ S_{hv}.S_{hv}^{*} \f$
 *  - channel #5 : \f$ S_{hv}.S_{vh}^{*} \f$
 *  - channel #6 : \f$ S_{hv}.S_{vv}^{*} \f$
 *  - channel #7 : \f$ S_{vh}.S_{vh}^{*} \f$
 *  - channel #8 : \f$ S_{vh}.S_{vv}^{*} \f$
 *  - channel #9 : \f$ S_{vv}.S_{vv}^{*} \f$
 *
 * The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
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
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCovarianceMatrixFunctor
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

    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vh = static_cast<ComplexType>(Svh);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    result[0] = static_cast<OutputValueType>( std::norm(S_hh) );
    result[1] = static_cast<OutputValueType>( S_hh*vcl_conj(S_hv) );
    result[2] = static_cast<OutputValueType>( S_hh*vcl_conj(S_vh) );
    result[3] = static_cast<OutputValueType>( S_hh*vcl_conj(S_vv) );
    result[4] = static_cast<OutputValueType>( std::norm(S_hv) );
    result[5] = static_cast<OutputValueType>( S_hv*vcl_conj(S_vh) );
    result[6] = static_cast<OutputValueType>( S_hv*vcl_conj(S_vv) );
    result[7] = static_cast<OutputValueType>( std::norm(S_vh) );
    result[8] = static_cast<OutputValueType>( S_vh*vcl_conj(S_vv) );
    result[9] = static_cast<OutputValueType>( std::norm(S_vv) );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToCovarianceMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToCovarianceMatrixFunctor() {}

protected:


private:
  //itkStaticConstMacro(m_NumberOfComponentsPerPixel, unsigned int, 10);
  static const  unsigned int m_NumberOfComponentsPerPixel = 10;

};

} // namespace Functor
} // namespace otb

#endif
