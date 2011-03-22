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
#ifndef __otbSinclairToCoherencyMatrixFunctor_h
#define __otbSinclairToCoherencyMatrixFunctor_h

#include "vcl_complex.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToCoherencyMatrixFunctor
 *  \brief Construct the fully polarimetric coherency matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ (S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 *  - channel #1 : \f$ (S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #2 : \f$ (S_{hh}+S_{vv}).(S_{hv}+S_{vh})^{*} \f$
 *  - channel #3 : \f$ (S_{hh}+S_{vv}).(j(S_{hv}-S_{vh}))^{*} \f$
 *  - channel #4 : \f$ (S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #5 : \f$ (S_{hh}-S_{vv}).(S_{hv}+S_{vh})^{*} \f$
 *  - channel #6 : \f$ (S_{hh}-S_{vv}).(j(S_{hv}-S_{vh}))^{*} \f$
 *  - channel #7 : \f$ (S_{hv}+S_{vh}).(S_{hv}+S_{vh})^{*} \f$
 *  - channel #8 : \f$ (S_{hv}+S_{vh}).(j(S_{hv}-S_{vh}))^{*} \f$
 *  - channel #9 : \f$ j(S_{hv}-S_{vh}).(j(S_{hv}-S_{vh}))^{*} \f$
 *
 * The output pixel has 10 channels : the diagonal and the upper element of the matrix.
 * Element are stored from left to right, line by line.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCovarianceFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceFunctor
 *
 */
template <class TInput1, class TInput2, class TInput3,
          class TInput4, class TOutput>
class SinclairToCoherencyMatrixFunctor
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

    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vh = static_cast<ComplexType>(Svh);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    const ComplexType HHPlusVV   = S_hh + S_vv;
    const ComplexType HHMinusVV  = S_hh - S_vv;
    const ComplexType HVPlusVH   =  S_hv + S_vh;
    const ComplexType jHVMinusVH = (S_hv - S_vh) * ComplexType(0., 1.);

    result[0] = static_cast<OutputValueType>( std::norm(HHPlusVV) );
    result[1] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(HHMinusVV) );
    result[2] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(HVPlusVH) );
    result[3] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(jHVMinusVH) );
    result[4] = static_cast<OutputValueType>( std::norm(HHMinusVV) );
    result[5] = static_cast<OutputValueType>( HHMinusVV * vcl_conj(HVPlusVH) );
    result[6] = static_cast<OutputValueType>( HHMinusVV * vcl_conj(jHVMinusVH) );
    result[7] = static_cast<OutputValueType>( std::norm(HVPlusVH) );
    result[8] = static_cast<OutputValueType>( HVPlusVH * vcl_conj(jHVMinusVH) );
    result[9] = static_cast<OutputValueType>( std::norm(jHVMinusVH) );

    result /= 2.0;

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return m_NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToCoherencyMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToCoherencyMatrixFunctor() {}

protected:


private:
  //itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 10);
  static const  unsigned int m_NumberOfComponentsPerPixel = 10;
};

} // namespace Functor
} // namespace otb

#endif
