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


#ifndef otbReciprocalHAlphaImageFilter_h
#define otbReciprocalHAlphaImageFilter_h

#include "otbUnaryFunctorImageFilter.h"
#include "otbMath.h"
#include "vnl/algo/vnl_complex_eigensystem.h"
#include <algorithm>

namespace otb
 {

namespace Functor {

/** \class otbHAlphaFunctor
 * \brief Evaluate the H-Alpha parameters from the reciprocal coherency matrix image.
 *
 * To process, we diagonalise the complex coherency matrix (size 3*3). We call \f$ SortedEigenValues \f$ the list that contains the
 * eigen values of the matrix sorted in decrease order. \f$ SortedEigenVector \f$ the corresponding list
 * of eigen vector.
 *
 * Output value are:
 * - channel #0 : \f$ entropy = -\sum_{i=0}^{2}{p[i].\log{p[i]}} / \log{3} \f$
 * - channel #1 : \f$ \alpha = \sum_{i=0}^{2}{p[i].\alpha_{i} \f$
 * - channel #2 : \f$ anisotropy = \frac {SortedEigenValues[1] - SortedEigenValues[2]}{SortedEigenValues[1] + SortedEigenValues[2]} \f$
 *
 * Where:
 * - \f$ p[i] = max(SortedEigenValues[i], 0) / \sum_{i=0}^{2, SortedEigenValues[i]>0}{SortedEigenValues[i]} \f$
 * - \f$ \alpha_{i} = \left| SortedEigenVector[i] \right|* \frac{180}{\pi}\f$
 *
 * With,
 * - \f$ if p[i] < 0, p[i]=0 \f$
 * - \f$ if p[i] > 1, p[i]=1 \f$
 * - \f$ if \alpha_{i} > 90, \alpha_{i}=90 \f$
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalHAlphaFunctor
{
public:
  typedef typename std::complex<double> ComplexType;
  typedef vnl_matrix<ComplexType>       VNLMatrixType;
  typedef vnl_vector<ComplexType>       VNLVectorType;
  typedef vnl_vector<double>           VNLDoubleVectorType;
  typedef std::vector<double>           VectorType;
  typedef typename TOutput::ValueType   OutputValueType;


  inline TOutput operator()( const TInput & Coherency ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    const double T0 = static_cast<double>(Coherency[0].real());
    const double T1 = static_cast<double>(Coherency[3].real());
    const double T2 = static_cast<double>(Coherency[5].real());

    VNLMatrixType vnlMat(3, 3, 0.);
    vnlMat[0][0] = ComplexType(T0,  0.);
    vnlMat[0][1] = ComplexType(Coherency[1]);
    vnlMat[0][2] = ComplexType(Coherency[2]);
    vnlMat[1][0] = std::conj(ComplexType(Coherency[1]));
    vnlMat[1][1] = ComplexType(T1,  0.);
    vnlMat[1][2] = ComplexType(Coherency[4]);
    vnlMat[2][0] = std::conj(ComplexType(Coherency[2]));
    vnlMat[2][1] = std::conj(ComplexType(Coherency[4]));
    vnlMat[2][2] = ComplexType(T2,  0.);

    // Only compute the left symmetry to respect the previous Hermitian Analisys code
    vnl_complex_eigensystem syst(vnlMat, false, true);
    const VNLMatrixType eigenVectors( syst.L );
    const VNLVectorType eigenValues(syst.W);

    // Entropy estimation
    double totalEigenValues(0.0);
    double p[3];
    double plog[3];
    double entropy;
    double alpha;
    double anisotropy;

    // Sort eigen values in decreasing order
    VectorType sortedRealEigenValues(3,  eigenValues[0].real());
    sortedRealEigenValues[1] = eigenValues[1].real();
    sortedRealEigenValues[2] = eigenValues[2].real();
    std::sort( sortedRealEigenValues.begin(), sortedRealEigenValues.end() );
    std::reverse( sortedRealEigenValues.begin(), sortedRealEigenValues.end() );

    // Extract the first component of each the eigen vector sorted by eigen value decrease order
    VNLVectorType sortedGreaterEigenVector(3, eigenVectors[0][0]);
    for(unsigned int i=0; i<3; ++i)
      {
        if( std::abs( eigenValues[1].real()-sortedRealEigenValues[i] ) < m_Epsilon )
          {
            sortedGreaterEigenVector[i] = eigenVectors[1][0];
          }
        else if( std::abs( eigenValues[2].real()-sortedRealEigenValues[i] ) < m_Epsilon )
          {
            sortedGreaterEigenVector[i] = eigenVectors[2][0];
          }
      }

    totalEigenValues = 0.0;
    for (unsigned int k = 0; k < 3; ++k)
      {
        sortedRealEigenValues[k] = std::max(sortedRealEigenValues[k], 0.);
        totalEigenValues += sortedRealEigenValues[k];
      }

      
    for (unsigned int k = 0; k < 3; ++k)
      {
        p[k] = sortedRealEigenValues[k] / totalEigenValues;
        
        if (p[k]<m_Epsilon) //n=log(n)-->0 when n-->0
			plog[k]=0.0;
		else
			plog[k]=-p[k]*log(p[k])/log(3.0);

      }

	entropy = 0.0;
	for (unsigned int k = 0; k < 3; ++k)
			entropy += plog[k];

    // alpha estimation
    double val0, val1, val2;
    double a0, a1, a2;

    val0 = std::abs(sortedGreaterEigenVector[0]);
    a0=acos(std::abs(val0)) * CONST_180_PI;

    val1 = std::abs(sortedGreaterEigenVector[1]);
    a1=acos(std::abs(val1)) * CONST_180_PI;

    val2= std::abs(sortedGreaterEigenVector[2]);
    a2=acos(std::abs(val2)) * CONST_180_PI;

    alpha=p[0]*a0 + p[1]*a1 + p[2]*a2;

    // Anisotropy estimation
    anisotropy=(sortedRealEigenValues[1] - sortedRealEigenValues[2])/(sortedRealEigenValues[1] + sortedRealEigenValues[2] + m_Epsilon);


    result[0] = static_cast<OutputValueType>(entropy);
    result[1] = static_cast<OutputValueType>(alpha);
    result[2] = static_cast<OutputValueType>(anisotropy);

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   ReciprocalHAlphaFunctor() : m_Epsilon(1e-6) {}

   /** Destructor */
   virtual ~ReciprocalHAlphaFunctor() {}

private:
   itkStaticConstMacro(m_NumberOfComponentsPerPixel, unsigned int, 3);
   const double m_Epsilon;
};
}


/** \class otbHAlphaImageFilter
 * \brief Compute the H-Alpha image (3 channels)
 * from the Reciprocal coherency image (6 complex channels)
 *
 * For more details, please refer to the class ReciprocalHAlphaFunctor.
 *
 * \ingroup OTBPolarimetry
 * \sa ReciprocalHAlphaFunctor
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReciprocalHAlphaImageFilter :
   public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::ReciprocalHAlphaFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef ReciprocalHAlphaImageFilter  Self;
   typedef typename Functor::ReciprocalHAlphaFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef otb::UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(ReciprocalHAlphaImageFilter, UnaryFunctorImageFilter);

protected:
   ReciprocalHAlphaImageFilter() {}
  ~ReciprocalHAlphaImageFilter() override {}

private:
  ReciprocalHAlphaImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

};

} // end namespace otb

#endif
