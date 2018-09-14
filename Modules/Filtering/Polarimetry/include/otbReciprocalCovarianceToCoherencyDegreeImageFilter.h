/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbReciprocalCovarianceToCoherencyDegreeImageFilter_h
#define otbReciprocalCovarianceToCoherencyDegreeImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class ReciprocalCovarianceToCoherencyDegreeFunctor
 * \brief Evaluate the Coherency Degree coefficient from from the MLC image
 *
 * Output value are:
 * - channel #0 : \f$ abs(S_{hh}*S_{vv}}^{*}) / sqrt(S_{hh}*S_{hh}}^{*}) / sqrt(S_{vv}*S_{vv}}^{*}) \f$
 * - channel #1 : \f$ abs(S_{hv}*S_{vv}}^{*}) / sqrt(S_{hv}*S_{hv}}^{*}) / sqrt(S_{vv}*S_{vv}}^{*}) \f$
 * - channel #2 : \f$ abs(S_{hh}*S_{hv}}^{*}) / sqrt(S_{hh}*S_{hh}}^{*}) / sqrt(S_{hv}*S_{hv}}^{*}) \f$
 *
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa MLCToCircularCoherencyDegreeImageFilter
 * \sa ReciprocalCovarianceToCoherencyImageFilter
 *
 *
 * \ingroup OTBPolarimetry
 */

template< class TInput, class TOutput>
class ReciprocalCovarianceToCoherencyDegreeFunctor
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

    /* Using the convention
     * \f$ C_{11} = S_{hh}*S_{hh}^* \f$
     * \f$ C_{12} = S_{hh}*S_{hv}^* \f$
     * \f$ C_{13} = S_{hh}*S_{vv}^* \f$
     * \f$ C_{22} = S_{hv}*S_{hv}^* \f$
     * \f$ C_{23} = S_{hv}*S_{vv}^* \f$
     * \f$ C_{33} = S_{vv}*S_{vv}^* \f$
     */
    const RealType    C11 =  static_cast<RealType>(Covariance[0].real());
    const ComplexType C12 =  static_cast<ComplexType>(Covariance[1]);
    const ComplexType C13 =  static_cast<ComplexType>(Covariance[2]);
    const RealType    C22 =  static_cast<RealType>(Covariance[3].real());
    const ComplexType C23 =  static_cast<ComplexType>(Covariance[4]);
    const RealType    C33 =  static_cast<RealType>(Covariance[5].real());

    if ((C11 >m_Epsilon) && (C33 > m_Epsilon))
      {
      result[0] = std::abs(C13) / std::sqrt(C11 * C33); // |<hh.vv*|/sqrt(<hh.hh*><vv.vv*>)
      }

    if ((C22 > m_Epsilon) && (C33 > m_Epsilon))
      {
      result[1] = std::abs(C23) / std::sqrt(C22 * C33);  // |<hv.vv*|/sqrt(<hv.hv*><vv.vv*>)
      }

    if ((C11 > m_Epsilon) && (C22 > m_Epsilon) )
      {
      result[2] = std::abs(C12) / std::sqrt(C11 * C22);  // |<hh.hv*|/sqrt(<hh.hh*><hv.hv*>)
      }

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalCovarianceToCoherencyDegreeFunctor() : m_NumberOfComponentsPerPixel(3), m_Epsilon(1e-6) {}

   /** Destructor */
   virtual ~ReciprocalCovarianceToCoherencyDegreeFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
    const double m_Epsilon;
};
}


/** \class otbReciprocalCovarianceToCoherencyDegreeImageFilter
 * \brief Compute the Coherency Degree coefficient
 * from the MLC image (6 complex channels)
 * For more details, lease refer to the class ReciprocalCovarianceToCoherencyDegreeFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa ReciprocalCovarianceToCoherencyDegreeFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */
template <class TInputImage, class TOutputImage>
class ReciprocalCovarianceToCoherencyDegreeImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalCovarianceToCoherencyDegreeFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalCovarianceToCoherencyDegreeImageFilter  Self;
   typedef typename Functor::ReciprocalCovarianceToCoherencyDegreeFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalCovarianceToCoherencyDegreeImageFilter, UnaryFunctorImageFilter);


protected:
  ReciprocalCovarianceToCoherencyDegreeImageFilter() {}
  ~ReciprocalCovarianceToCoherencyDegreeImageFilter() override {}

private:
  ReciprocalCovarianceToCoherencyDegreeImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;


};

} // end namespace otb

#endif
