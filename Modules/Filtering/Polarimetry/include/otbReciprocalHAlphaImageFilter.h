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

#ifndef __ReciprocalHAlphaImageFilter_h
#define __ReciprocalHAlphaImageFilter_h

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
 * \ingroup SARPolarimetry
 *
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
    vnlMat[0][1] = std::conj(ComplexType(Coherency[1]));
    vnlMat[0][2] = std::conj(ComplexType(Coherency[2]));
    vnlMat[1][0] = ComplexType(Coherency[1]);
    vnlMat[1][1] = ComplexType(T1,  0.);
    vnlMat[1][2] = std::conj(ComplexType(Coherency[4]));
    vnlMat[2][0] = ComplexType(Coherency[2]);
    vnlMat[2][1] = ComplexType(Coherency[4]);
    vnlMat[2][2] = ComplexType(T2,  0.);

    // Only compute the left symetry to respect the previous Hermitian Analisys code
    vnl_complex_eigensystem syst(vnlMat, false, true);
    const VNLMatrixType eigenVectors( syst.L );
    const VNLVectorType eigenValues(syst.W);

    // Entropy estimation
    double totalEigenValues(0.0);
    double p[3];
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
        if( vcl_abs( eigenValues[1].real()-sortedRealEigenValues[i] ) < m_Epsilon )
          {
            sortedGreaterEigenVector[i] = eigenVectors[1][0];
          }
        else if( vcl_abs( eigenValues[2].real()-sortedRealEigenValues[i] ) < m_Epsilon )
          {
            sortedGreaterEigenVector[i] = eigenVectors[2][0];
          }
      }

    totalEigenValues = sortedRealEigenValues[0] + sortedRealEigenValues[1] + sortedRealEigenValues[2];
    if (totalEigenValues <m_Epsilon)
      {
        totalEigenValues = m_Epsilon;
      }

    for (unsigned int k = 0; k < 3; ++k)
      {
        p[k] = std::max(sortedRealEigenValues[k], 0.) / totalEigenValues;
      }

    if ( (p[0] < m_Epsilon) || (p[1] < m_Epsilon) || (p[2] < m_Epsilon) )
      {
        entropy =0.0;
      }
    else
      {
        entropy = p[0]*log(p[0]) + p[1]*log(p[1]) + p[2]*log(p[2]);
        entropy /= -log(3.);
      }

    // alpha estimation
    double val0, val1, val2;
    double a0, a1, a2;

    for(unsigned int k = 0; k < 3; ++k)
      {
         p[k] = sortedRealEigenValues[k] / totalEigenValues;

         if (p[k] < 0.)
           {
             p[k] = 0.;
           }

         if (p[k] > 1.)
           {
             p[k] = 1.;
           }
      }

    val0 = std::abs(sortedGreaterEigenVector[0]);
    a0=acos(vcl_abs(val0)) * CONST_180_PI;

    val1 = std::abs(sortedGreaterEigenVector[1]);
    a1=acos(vcl_abs(val1)) * CONST_180_PI;

    val2= std::abs(sortedGreaterEigenVector[2]);
    a2=acos(vcl_abs(val2)) * CONST_180_PI;

    alpha=p[0]*a0 + p[1]*a1 + p[2]*a2;

    if (alpha>90)
      {
        alpha=0.;
      }

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
 * \ingroup SARPOlarimetry
 * \sa ReciprocalHAlphaFunctor
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
  virtual ~ReciprocalHAlphaImageFilter() {}

private:
  ReciprocalHAlphaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);            //purposely not implemented

};

} // end namespace otb

#endif
