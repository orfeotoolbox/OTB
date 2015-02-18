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
#ifndef __otbSinclairToReciprocalCoherencyMatrixFunctor_h
#define __otbSinclairToReciprocalCoherencyMatrixFunctor_h

#include "vcl_complex.h"
#include "itkMacro.h"

namespace otb
{
namespace Functor
{
/** \class SinclairToReciprocalCoherencyMatrixFunctor
 *  \brief Construct the reciprocal fully polarimetric coherency matrix
 *  with Sinclair matrix information.
 *
 *  Output value are:
 *  - channel #0 : \f$ (S_{hh}+S_{vv}).(S_{hh}+S_{vv})^{*} \f$
 *  - channel #1 : \f$ (S_{hh}+S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #2 : \f$ (S_{hh}+S_{vv}).(2*S_{hv})^{*} \f$
 *  - channel #3 : \f$ (S_{hh}-S_{vv}).(S_{hh}-S_{vv})^{*} \f$
 *  - channel #4 : \f$ (S_{hh}-S_{vv}).(2*S_{hv})^{*} \f$
 *  - channel #5 : \f$ (2*S_{hv}).(2*S_{hv})^{*} \f$
 *
 * This is a adaptation of the SinclairToCoherencyFunctor, where \f$ S_{hv}=S_{vh} \f$.
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 *  \ingroup Functor
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyMatrixFunctor
 *  \sa SinclairToCovarianceMatrixFunctor
 *  \sa SinclairToMuellerMatrixFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class SinclairToReciprocalCoherencyMatrixFunctor
{
public:
  /** Some typedefs. */
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;
  
  itkStaticConstMacro(NumberOfComponentsPerPixel, unsigned int, 6);
  
  inline TOutput operator ()(const TInput1& Shh, const TInput2& Shv, const TInput3& Svv)
  {
    TOutput result;

    result.SetSize(NumberOfComponentsPerPixel);

    const ComplexType S_hh = static_cast<ComplexType>(Shh);
    const ComplexType S_hv = static_cast<ComplexType>(Shv);
    const ComplexType S_vv = static_cast<ComplexType>(Svv);

    const ComplexType HHPlusVV  = S_hh + S_vv;
    const ComplexType HHMinusVV = S_hh - S_vv;
    const ComplexType twoHV     = ComplexType( 2.0 ) * S_hv;

    result[0] = static_cast<OutputValueType>( std::norm(HHPlusVV) );
    result[1] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(HHMinusVV) );
    result[2] = static_cast<OutputValueType>( HHPlusVV * vcl_conj(twoHV) );
    result[3] = static_cast<OutputValueType>( std::norm(HHMinusVV) );
    result[4] = static_cast<OutputValueType>( HHMinusVV *vcl_conj(twoHV) );
    result[5] = static_cast<OutputValueType>( std::norm(twoHV) );

    result /= 2.0;

    return (result);
  }

  unsigned int GetNumberOfComponentsPerPixel()
  {
    return NumberOfComponentsPerPixel;
  }

  /** Constructor */
  SinclairToReciprocalCoherencyMatrixFunctor() {}

  /** Destructor */
  virtual ~SinclairToReciprocalCoherencyMatrixFunctor() {}

protected:

private:

};

} // namespace Functor
} // namespace otb

#endif
