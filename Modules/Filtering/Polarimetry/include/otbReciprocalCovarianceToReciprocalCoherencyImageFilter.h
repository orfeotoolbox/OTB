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

#ifndef __ReciprocalCovarianceToReciprocalCoherencyImageFilter_h
#define __ReciprocalCovarianceToReciprocalCoherencyImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class ReciprocalCovarianceToReciprocalCoherencyFunctor
 * \brief Evaluate the Coherency matrix from the Covariance image
 *
 * Output value are:
 * - channel #0 : \f$ 0.5 * (S_{hh}+S_{vv}.(S_{hh}+S_{vv})^{*} \f$
 * - channel #1 : \f$ 0.5 * (S_{hh}+S_{vv}.(S_{hh}-S_{vv})^{*} \f$
 * - channel #2 : \f$ (S_{hh}+S_{vv}.(S_{hv})^{*} \f$
 * - channel #3 : \f$ 0.5 * (S_{hh}-S_{vv}.(S_{hh}-S_{vv})^{*} \f$
 * - channel #4 : \f$ (S_{hh}-S_{vv}.(S_{hv})^{*} \f$
 * - channel #5 : \f$ 2.0*S_{hv}.S_{hv}^{*} \f$
 *
 * The output pixel has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa CovarianceToCircularCoherencyDegreeImageFilter
 * \sa ReciprocalCovarianceToReciprocalCoherencyDegreeImageFilter
 *
 */
template< class TInput, class TOutput>
class ReciprocalCovarianceToReciprocalCoherencyFunctor
{
public:
  typedef typename std::complex <double>           ComplexType;
  typedef typename TOutput::ValueType              OutputValueType;

  inline TOutput operator()( const TInput & Covariance ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    /* Using the convention
     * \f$ C_{11} = S_{hh}*S_{hh}^* \f$
     * \f$ C_{12} = S_{hh}*S_{hv}^* \f$
     * \f$ C_{13} = S_{hh}*S_{vv}^* \f$
     * \f$ C_{22} = S_{hv}*S_{hv}^* \f$
     * \f$ C_{23} = S_{hv}*S_{vv}^* \f$
     * \f$ C_{33} = S_{vv}*S_{vv}^* \f$
     */
    const ComplexType C11 =  static_cast<ComplexType>(Covariance[0]);
    const ComplexType C12 =  static_cast<ComplexType>(Covariance[1]);
    const ComplexType C13 =  static_cast<ComplexType>(Covariance[2]);
    const ComplexType C22 =  static_cast<ComplexType>(Covariance[3]);
    const ComplexType C23 =  static_cast<ComplexType>(Covariance[4]);
    const ComplexType C33 =  static_cast<ComplexType>(Covariance[5]);

    //const ComplexType C21 =  vcl_conj(C12);
    const ComplexType C31 =  vcl_conj(C13);
    const ComplexType C32 =  vcl_conj(C23);

    result[0] = static_cast<OutputValueType>( 0.5*(C11 + C13 + C31 + C33) );
    result[1] = static_cast<OutputValueType>( 0.5*(C11 - C13 + C31 - C33) );
    result[2] = static_cast<OutputValueType>( C12 + C32 );
    result[3] = static_cast<OutputValueType>( 0.5*(C11 - C13 - C31 + C33) );
    result[4] = static_cast<OutputValueType>( C12 - C32 );
    result[5] = static_cast<OutputValueType>( 2.0 * C22 );

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalCovarianceToReciprocalCoherencyFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   virtual ~ReciprocalCovarianceToReciprocalCoherencyFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbReciprocalCovarianceToReciprocalCoherencyImageFilter
 * \brief Compute the Coherency image (6 complex channels)
 * from the Covariance image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalCovarianceToReciprocalCoherencyFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa ReciprocalCovarianceToReciprocalCoherencyFunctor
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalCovarianceToReciprocalCoherencyImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalCovarianceToReciprocalCoherencyFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalCovarianceToReciprocalCoherencyImageFilter  Self;
   typedef Functor::ReciprocalCovarianceToReciprocalCoherencyFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctorType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalCovarianceToReciprocalCoherencyImageFilter, UnaryFunctorImageFilter);


protected:
  ReciprocalCovarianceToReciprocalCoherencyImageFilter() {}
  virtual ~ReciprocalCovarianceToReciprocalCoherencyImageFilter() {}

private:
  ReciprocalCovarianceToReciprocalCoherencyImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented


};

} // end namespace otb

#endif
