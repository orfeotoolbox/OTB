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

#ifndef __MLCToCoherencyDegreeImageFilter_h
#define __MLCToCoherencyDegreeImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMLCToCoherencyDegreeFunctor
 * \brief Evaluate the Coherency Degree coefficient from from the MLC image
 *
 * *  Output value are:
 *   channel #0 : \f$ abs(S_{hh}*S_{vv}}^{*}) / sqrt(S_{hh}*S_{hh}}^{*}) / sqrt(S_{vv}*S_{vv}}^{*})\f$
 *   channel #1 : \f$ abs(S_{hv}*S_{vv}}^{*}) / sqrt(S_{hv}*S_{hv}}^{*}) / sqrt(S_{vv}*S_{vv}}^{*}) \f$
 *   channel #2 : \f$ abs(S_{hh}*S_{hv}}^{*}) / sqrt(S_{hh}*S_{hh}}^{*}) / sqrt(S_{hv}*S_{hv}}^{*}) \f$
 *
 */
template< class TInput, class TOutput>
class MLCToCoherencyDegreeFunctor
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

    RealType C11 =  static_cast<RealType>(Covariance[0].real());
    ComplexType C12 =  static_cast<ComplexType>(Covariance[1]);
    ComplexType C13 =  static_cast<ComplexType>(Covariance[2]);
    RealType C22 =  static_cast<RealType>(Covariance[3].real());
    ComplexType C23 =  static_cast<ComplexType>(Covariance[4]);
    RealType C33 =  static_cast<RealType>(Covariance[5].real());

    if ((C11 >0.00001) && (C33 > 0.0001))
      {
      result[0] = vcl_abs(C13) / vcl_sqrt(C11 *C33); // |<hh.vv*|/sqrt(<hh.hh*><vv.vv*>)
      }

    if ((C22 > 0.00001) && (C33 > 0.00001))
      {
      result[1] = vcl_abs(C23) / vcl_sqrt(C22 * C33);  // |<hv.vv*|/sqrt(<hv.hv*><vv.vv*>)
      }

    if ((C11 > 0.00001) && (C22 > 0.00001) )
      {
      result[2] = vcl_abs(C12) / vcl_sqrt(C11 * C22);  // |<hh.hv*|/sqrt(<hh.hh*><hv.hv*>)
      }

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MLCToCoherencyDegreeFunctor() : m_NumberOfComponentsPerPixel(3)  {}

   /** Destructor */
   ~MLCToCoherencyDegreeFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMLCToCoherencyDegreeImageFilter
 * \brief Compute the Coherency Degree coefficient
 * from the MLC image (6 complex channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MLCToCoherencyDegreeFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MLCToCoherencyDegreeImageFilter :
   public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MLCToCoherencyDegreeImageFilter  Self;
   typedef itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MLCToCoherencyDegreeImageFilter, itk::UnaryFunctorImageFilter);


protected:
  MLCToCoherencyDegreeImageFilter() {}
  virtual ~MLCToCoherencyDegreeImageFilter() {}

private:
  MLCToCoherencyDegreeImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                  //purposely not implemented

 
 

};
  
} // end namespace otb

#endif
