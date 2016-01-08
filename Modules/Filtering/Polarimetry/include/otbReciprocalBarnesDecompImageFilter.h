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

#ifndef __ReciprocalBarnesDecompImageFilter_h
#define __ReciprocalBarnesDecompImageFilter_h

#include "otbUnaryFunctorImageFilter.h"
#include "otbMath.h"
#include "vnl/algo/vnl_complex_eigensystem.h"
#include <algorithm>

namespace otb
 {

namespace Functor {

/** \class otbBarnesDecompFunctor
 * \brief Evaluate the H-Alpha parameters from the reciprocal covariance matrix image.
 *
 *
 * \ingroup SARPolarimetry
 *
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalBarnesDecompFunctor
{
public:
  typedef typename std::complex<double> ComplexType;
  typedef vnl_matrix<ComplexType>       VNLMatrixType;
  typedef vnl_vector<ComplexType>       VNLVectorType;
  typedef vnl_vector<double>           VNLDoubleVectorType;
  typedef std::vector<double>           VectorType;
  typedef typename TOutput::ValueType   OutputValueType;


  inline TOutput operator()( const TInput & Covariance ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);
    
	VNLMatrixType qi(3,1);
	

    VNLMatrixType cov(3, 3);
    cov[0][0] = ComplexType(Covariance[0]);
    cov[0][1] = ComplexType(Covariance[1]);
    cov[0][2] = ComplexType(Covariance[2]);
    cov[1][0] = std::conj(ComplexType(Covariance[1]));
    cov[1][1] = ComplexType(Covariance[3]);
    cov[1][2] = ComplexType(Covariance[4]);
    cov[2][0] = std::conj(ComplexType(Covariance[2]));
    cov[2][1] = std::conj(ComplexType(Covariance[4]));
    cov[2][2] = ComplexType(Covariance[5]);


	qi[0][0]=ComplexType(1.,0.);
	qi[1][0]=ComplexType(0.,0.);
	qi[2][0]=ComplexType(0.,0.);
	ComplexType norm =  (qi.conjugate_transpose()*cov*qi)[0][0];
	VNLMatrixType ki = cov*qi / std::sqrt(norm);
    result[0] = static_cast<OutputValueType>(ki[0][0]);
    result[1] = static_cast<OutputValueType>(ki[1][0]);
    result[2] = static_cast<OutputValueType>(ki[2][0]);
    
    
	qi[0][0]=ComplexType(0.,0.);
	qi[1][0]=ComplexType(1./std::sqrt(2.),0.);
	qi[2][0]=ComplexType(0.,1./std::sqrt(2.));
	norm =  (qi.conjugate_transpose()*cov*qi)[0][0];
	ki = cov*qi / std::sqrt(norm);
    result[3] = static_cast<OutputValueType>(ki[0][0]);
    result[4] = static_cast<OutputValueType>(ki[1][0]);
    result[5] = static_cast<OutputValueType>(ki[2][0]);
    
    
    qi[0][0]=ComplexType(0.,0.);
	qi[1][0]=ComplexType(0.,1./std::sqrt(2.));
	qi[2][0]=ComplexType(1./std::sqrt(2.),0.);
	norm =  (qi.conjugate_transpose()*cov*qi)[0][0];
	ki = cov*qi / std::sqrt(norm);
    result[6] = static_cast<OutputValueType>(ki[0][0]);
    result[7] = static_cast<OutputValueType>(ki[1][0]);
    result[8] = static_cast<OutputValueType>(ki[2][0]);

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalBarnesDecompFunctor() : m_Epsilon(1e-6) {}

   /** Destructor */
   virtual ~ReciprocalBarnesDecompFunctor() {}

private:
   itkStaticConstMacro(m_NumberOfComponentsPerPixel, unsigned int, 9);
   const double m_Epsilon;
};
}


/** \class otbBarnesDecompImageFilter
 * \brief Compute the H-Alpha image (3 channels)
 * from the Reciprocal Covariance image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalBarnesDecompFunctor.
 *
 * \ingroup SARPOlarimetry
 * \sa ReciprocalBarnesDecompFunctor
 *
 * \ingroup OTBPolarimetry
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalBarnesDecompImageFilter :
   public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalBarnesDecompFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalBarnesDecompImageFilter  Self;
   typedef typename Functor::ReciprocalBarnesDecompFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalBarnesDecompImageFilter, UnaryFunctorImageFilter);

protected:
   ReciprocalBarnesDecompImageFilter() {}
  virtual ~ReciprocalBarnesDecompImageFilter() {}

private:
  ReciprocalBarnesDecompImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};

} // end namespace otb

#endif
