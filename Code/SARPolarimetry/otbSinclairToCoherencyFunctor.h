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
 *  \brief Construct the reciprocal fully polarimetric coherency matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *   channel #0 : \f$ S_{hh+vv}.S_{hh+vv}^{*} \f$
 *   channel #1 : \f$ S_{hh+vv}.S_{hh-vv}^{*} \f$
 *   channel #2 : \f$ S_{hh+vv}.S_{2*hv}^{*} \f$
 *   channel #3 : \f$ S_{hh-vv}.S_{hh-vv}^{*} \f$
 *   channel #4 : \f$ S_{hh-vv}.S_{2*hv}^{*} \f$
 *   channel #5 : \f$ S_{2*hv}.S_{2*hv}^{*} \f$
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCoherencyFunctor
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

    ComplexType HHPlusVV = static_cast<ComplexType>(Shh + Svv);
    ComplexType VVMinusVV = static_cast<ComplexType>(Shh - Svv);
    ComplexType twoHV     = static_cast<ComplexType>( 2.0 * Shv);

    result[0] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(HHPlusVV) );
    result[1] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(VVMinusVV) );
    result[2] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(twoHV) );
    result[3] = static_cast<OutputValueType>( VVMinusVV * vcl_conj(VVMinusVV) );
    result[4] = static_cast<OutputValueType>( VVMinusVV *vcl_conj(twoHV) );
    result[5] = static_cast<OutputValueType>( twoHV * vcl_conj(twoHV) );

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToCoherencyFunctor() : m_NumberOfComponentsPerPixel(6) {}

  /** Destructor */
  virtual ~SinclairToCoherencyFunctor() {}

protected:


private:
    unsigned int m_NumberOfComponentsPerPixel;
};

} // namespace Functor
} // namespace otb

#endif
