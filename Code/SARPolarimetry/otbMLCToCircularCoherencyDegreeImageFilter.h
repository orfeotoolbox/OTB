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

#ifndef __MLCToCircularCoherencyDegreeImageFilter_h
#define __MLCToCircularCoherencyDegreeImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMLCToCircularCoherencyDegreeFunctor
 * \brief Evaluate the Circular Coherency Degree coefficient from from the MLC image
 *
 *  Output value are:
 *   channel #0 : \f$ abs(S_{ll}*S_{rr}}^{*}) / sqrt(S_{ll}*S_{ll}}^{*}) / sqrt(S_{rr}*S_{rr}}^{*})\f$
 *   channel #1 : \f$ abs(S_{lr}*S_{rr}}^{*}) / sqrt(S_{lr}*S_{lr}}^{*}) / sqrt(S_{rr}*S_{rr}}^{*}) \f$
 *   channel #2 : \f$ abs(S_{ll}*S_{lr}}^{*}) / sqrt(S_{ll}*S_{ll}}^{*}) / sqrt(S_{lr}*S_{lr}}^{*}) \f$
 *
 * In general case :
 *   \f$ S_{ll} = 1/2 * ( -S_{xx} - j*S_{xy} - j*S_{yx} + S_{yy})  \f$
 *   \f$ S_{lr} = 1/2 * ( -S_{xx} + j*S_{xy} - j*S_{yx} - S_{yy})  \f$
 *   \f$ S_{rl} = 1/2 * ( -S_{xx} - j*S_{xy} + j*S_{yx} - S_{yy})  \f$
 *   \f$ S_{rl} = 1/2 * ( -S_{xx} + j*S_{xy} + j*S_{yx} + S_{yy})  \f$
 *
 * For backscattering, the equiations simplify to:
 *
 *   \f$ S_{ll} = S_{ll}^{*} = 1/2 * ( -S_{xx} - j*2*S_{xy} + S_{yy})  \f$
 *   \f$ S_{lr} = S_{rl}     = 1/2 * ( -S_{xx} - S_{yy})  \f$
 *
 */
template< class TInput, class TOutput>
class MLCToCircularCoherencyDegreeFunctor
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

    RealType C1 =  static_cast<RealType>(Covariance[0].real()); // C1  <hh.hh*>
    RealType C2 =  static_cast<RealType>(Covariance[3].real()); // C2  <hv.hv*>
    RealType C3 =  static_cast<RealType>(Covariance[5].real()); // C3  <vv.vv*>
    RealType C4 =  static_cast<RealType>(Covariance[1].real()); // C4  Re<hh.hv*>
    RealType C5 =  static_cast<RealType>(Covariance[1].imag()); // C5  Im<hh.hv*>
    RealType C6 =  static_cast<RealType>(Covariance[2].real()); // C6  Re<hh.vv*>
    RealType C7 =  static_cast<RealType>(Covariance[2].imag()); // C7  Im<hh.vv*>
    RealType C8 =  static_cast<RealType>(Covariance[4].real()); // C8  Re<hv.vv*>
    RealType C9 =  static_cast<RealType>(Covariance[4].imag()); // C9  Im<hv.vv*>

    RealType llrrReal = 0.25 * ( C1 + C3 -4*C2 -2*C6);
    RealType llrrImag = -(C4 + C8);

    RealType lllrReal = 0.25 * ( C1 - C3 - 2*C5 + 2*C9);
    RealType lllrImag = -0.5*(C7+C4+C8);

    RealType rrlrReal = 0.25 * ( C1 -C3 + 2*C5 - 2*C9);
    RealType rrlrImag = 0.5 * (C4+C8-C7);

    RealType ll2    = 0.25 * ( C1 + C3 + 4*C2 - 2*C6 - 4*C5 - 4*C9);
    RealType rr2    = 0.25 * ( C1 + C3 + 4*C2 - 2*C6 + 4*C5 + 4*C9);
    RealType lr2    = 0.25 * ( C1 + C3 + 2*C6);

    if (ll2 <= 0.01)
      {
        ll2=0.01;
      }

    if (rr2 <= 0.01)
      {
        rr2=0.01;
      }

    if (lr2 <= 0.01)
      {
        lr2=0.01;
      }

    result[0]= sqrt( llrrReal*llrrReal + llrrImag*llrrImag ) / sqrt (ll2*rr2); // |<ll.rr*>|/sqrt(<ll.ll*><rr.rr*>)
    result[1]= sqrt( lllrReal*lllrReal + lllrImag*lllrImag ) / sqrt (ll2*lr2); // |<ll.lr*>|/sqrt(<ll.ll*><lr.lr*>)
    result[2]= sqrt( rrlrReal*rrlrReal + rrlrImag*rrlrImag ) / sqrt (rr2*lr2); // |<rr.lr*>|/sqrt(<rr.rr*><lr.lr*>)

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MLCToCircularCoherencyDegreeFunctor() : m_NumberOfComponentsPerPixel(3)  {}

   /** Destructor */
   ~MLCToCircularCoherencyDegreeFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMLCToCircularCoherencyDegreeImageFilter
 * \brief Compute the Circular Coherency Degree coefficient
 * from the MLC image (6 complex channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MLCToCircularCoherencyDegreeFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MLCToCircularCoherencyDegreeImageFilter :
   public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MLCToCircularCoherencyDegreeImageFilter  Self;
   typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MLCToCircularCoherencyDegreeImageFilter,itk::UnaryFunctorImageFilter);


protected:
  MLCToCircularCoherencyDegreeImageFilter() {}
  virtual ~MLCToCircularCoherencyDegreeImageFilter() {}

private:
  MLCToCircularCoherencyDegreeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                          //purposely not implemented

};
  
} // end namespace otb

#endif
