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

namespace otb
 {

namespace Functor {

/** \class ReciprocalLinearCovarianceToReciprocalCircularCovarianceFunctor
 * \brief Compute the reciprocal Covariance circular matrix from the reciprocal Covariance linear matrix.
 *  Extract from Antennas for radar and communications Harold Mott p 317.
 *
 *  Output value are:
 *  - channel #0 : \f$ 0.25 * (C_{1}+C_{3}+4*C_{2}-2*C_{6}-4*C_{5}-4*C_{9}) \f$
 *  - channel #1 : \f$ 0.25 * (C_{1}-C_{3}+2*C_{5}+2*C_{9} - 0.5\mathcal{i}.(C_{4}+C_{7}+C_{8}) \f$
 *  - channel #2 : \f$ 0.25 * (C_{1}+C_{3}-4*C_{2}-2*C_{6} - \mathcal{i}.(C_{4}+C_{8}) \f$
 *  - channel #3 : \f$ 0.25 * (C_{1}+C_{3}+2*C_{6} \f$
 *  - channel #4 : \f$ 0.25 * (C_{1}-C_{3}+2*C_{5}-2*C_{9} - 0.5\mathcal{i}.(C_{4}-C_{7}+C_{8}) \f$
 *  - channel #5 : \f$ 0.25 * (C_{1}+C_{3}+4*C_{2}-2*C_{6}+4*C_{5}+4*C_{9}) \f$
 *
 *  Where:
 *  - \f$ C_{1} = S_{hh}*S_{hh}}^{*} = \mathcal{Re}(input[0]) \f$
 *  - \f$ C_{2} = S_{hv}*S_{hv}}^{*} = \mathcal{Re}(input[3]) \f$
 *  - \f$ C_{3} = S_{vv}*S_{vv}}^{*} = \mathcal{Re}(input[5]) \f$
 *  - \f$ C_{4} = \mathcal{Re}(S_{hh}*S_{hv}}^{*}) = \mathcal{Re}(input[1]) \f$
 *  - \f$ C_{5} = \mathcal{Im}(S_{hh}*S_{hv}}^{*}) = \mathcal{Im}(input[1]) \f$
 *  - \f$ C_{6} = \mathcal{Re}(S_{hh}*S_{vv}}^{*}) = \mathcal{Re}(input[2]) \f$
 *  - \f$ C_{7} = \mathcal{Im}(S_{hh}*S_{vv}}^{*}) = \mathcal{Im}(input[2]) \f$
 *  - \f$ C_{8} = \mathcal{Re}(S_{hv}*S_{vv}}^{*}) = \mathcal{Re}(input[4]) \f$
 *  - \f$ C_{9} = \mathcal{Im}(S_{hv}*S_{vv}}^{*} = \mathcal{Im}(input[4])) \f$
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

    const RealType C1 =  static_cast<RealType>(Covariance[0].real()); // C1  <hh.hh*>
    const RealType C2 =  static_cast<RealType>(Covariance[3].real()); // C2  <hv.hv*>
    const RealType C3 =  static_cast<RealType>(Covariance[5].real()); // C3  <vv.vv*>
    const RealType C4 =  static_cast<RealType>(Covariance[1].real()); // C4  Re<hh.hv*>
    const RealType C5 =  static_cast<RealType>(Covariance[1].imag()); // C5  Im<hh.hv*>
    const RealType C6 =  static_cast<RealType>(Covariance[2].real()); // C6  Re<hh.vv*>
    const RealType C7 =  static_cast<RealType>(Covariance[2].imag()); // C7  Im<hh.vv*>
    const RealType C8 =  static_cast<RealType>(Covariance[4].real()); // C8  Re<hv.vv*>
    const RealType C9 =  static_cast<RealType>(Covariance[4].imag()); // C9  Im<hv.vv*>

    const RealType llrrReal = 0.25 * ( C1 + C3 - 4*C2 -2*C6);
    const RealType llrrImag = -(C4 + C8);

    const RealType lllrReal = 0.25 * ( C1 - C3 - 2*C5 + 2*C9);
    const RealType lllrImag = -0.5*(C7+C4+C8);

    const RealType rrlrReal = 0.25 * ( C1 -C3 + 2*C5 - 2*C9);
    const RealType rrlrImag = 0.5 * (C4+C8-C7);

    result[0] = ComplexType(0.25 * ( C1 + C3 + 4*C2 - 2*C6 - 4*C5 - 4*C9));
    result[1] = ComplexType(lllrReal, lllrImag);
    result[2] = ComplexType(llrrReal, llrrImag);
    result[3] = ComplexType(0.25 * ( C1 + C3 + 2*C6));
    result[4] = ComplexType(rrlrReal, -rrlrImag);
    result[5] = ComplexType(0.25 * ( C1 + C3 + 4*C2 - 2*C6 + 4*C5 + 4*C9));

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
  virtual ~ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter() {}

private:
  ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                          //purposely not implemented

};

} // end namespace otb

#endif
