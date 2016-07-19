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

#ifndef __ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter_h
#define __ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vcl_complex.h"

namespace otb
 {

namespace Functor {

/** \class ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
 * \brief Compute the reciprocal Covariance circular matrix from the reciprocal Covariance linear matrix.
 *  Extract from Antennas for radar and communications Harold Mott p 317.
 *
 *  Output value are:
 *  - channel #0 : \f$ 0.25 * (C33-i.\sqrt{2}.C23-C13+i.\sqrt{2}.C23^{*}-C13^{*}+2.C22-i.\sqrt{2}.C12+i.\sqrt{2}.C12^{*}+C11) \f$
 *  - channel #1 : \f$ 0.25 * (i.\sqrt{2}.C33+2.C23-i.\sqrt{2}.C13+i.\sqrt{2}.C13^{*}+2.C12^{*}-i.\sqrt{2}.C11) \f$
 *  - channel #2 : \f$ 0.25 * (-C33+i.\sqrt{2}.C23+C13+i.\sqrt{2}.C23^{*}+C13^{*}+2.C22-i.\sqrt{2}.C12-i.\sqrt{2}.C12^{*}-C11 ) \f$
 *  - channel #3 : \f$ 0.25 * (2.C33+2.C13+2.C13^{*}+2.C11)\f$
 *  - channel #4 : \f$ 0.25 * (i.\sqrt{2}.C33+i.\sqrt{2}.C13+2.C23^{*}-i.\sqrt{2}.C13^{*}+2.C12-i.\sqrt{2}.C11) \f$
 *  - channel #5 : \f$ 0.25 * (C33+i.\sqrt{2}.C23-C13-i.\sqrt{2}.C23^{*}-C13^{*}+2.C22+i.\sqrt{2}.C12-i.\sqrt{2}.C12^{*}+C11) \f$
 *
 *  Where Cij are related to the elements of the reciprocal linear covariance matrix.
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa CovarianceToCoherencyDegreeImageFilter
 * \sa CovarianceToCoherencyImageFilter
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
{
public:
  typedef double                                   RealType;
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline TOutput operator()( const TInput & Covariance ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);
    result.Fill(0.0);
    
    
    const ComplexType C11 =  static_cast<ComplexType>(  Covariance[0]    );     //   <hh.hh*>
    const ComplexType C12 =  static_cast<ComplexType>(  Covariance[1]    );     //   <sqrt(2).hh.hv*>
    const ComplexType C13 =  static_cast<ComplexType>(  Covariance[2]    );     //   <hh.vv*>
    const ComplexType C22 =  static_cast<ComplexType>(  Covariance[3]    );     //   <2.hv.hv*>
    const ComplexType C23 =  static_cast<ComplexType>(  Covariance[4]    );     //   <sqrt(2).hv.vv*>
    const ComplexType C33 =  static_cast<ComplexType>(  Covariance[5]    );     //   <vv.vv*>
    
    
    const ComplexType cst1 = ComplexType(0.0, vcl_sqrt(2.0));
    const ComplexType two = ComplexType(2.0, 0 );

    result[0] = static_cast<ComplexType>( C33-cst1*C23-C13+cst1*vcl_conj(C23)-vcl_conj(C13)+two*C22-cst1*C12+cst1*vcl_conj(C12)+C11  ) ;
    result[1] = static_cast<ComplexType>( cst1*C33+two*C23-cst1*C13+cst1*vcl_conj(C13)+two*vcl_conj(C12)-cst1*C11 );
    result[2] = static_cast<ComplexType>( -C33+cst1*C23+C13+cst1*vcl_conj(C23)+vcl_conj(C13)+two*C22-cst1*C12-cst1*vcl_conj(C12)-C11  ) ;
    result[3] = static_cast<ComplexType>( two*C33+two*C13+two*vcl_conj(C13)+two*C11 ) ;
    result[4] = static_cast<ComplexType>( cst1*C33+cst1*C13+two*vcl_conj(C23)-cst1*vcl_conj(C13)+two*C12-cst1*C11 ) ;
    result[5] = static_cast<ComplexType>( C33+cst1*C23-C13-cst1*vcl_conj(C23)-vcl_conj(C13)+two*C22+cst1*C12-cst1*vcl_conj(C12)+C11 ) ;

	result /= 4.0;

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   virtual ~ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
 * \brief Compute the reciprocal Covariance circular matrix image from the reciprocal Covariance linear matrix image.
 * For more details, please refer to the class ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
 *
 * \ingroup OTBPolarimetry
 */
 template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter :
   public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter  Self;
   typedef typename Functor::ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter, itk::UnaryFunctorImageFilter);


protected:
  ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter() {}
  ~ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter() ITK_OVERRIDE {}

private:
  ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                          //purposely not implemented

};

} // end namespace otb

#endif
