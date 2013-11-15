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

#ifndef __MuellerToReciprocalCovarianceImageFilter_h
#define __MuellerToReciprocalCovarianceImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class MuellerToReciprocalCovarianceFunctor
 * \brief Evaluate the  MLC image from the Mueller image
 *
 * Output value are:
 * - channel #0 : \f$ 0.5*(M_{11} + M_{22} + 2*M_{12}) \f$
 * - channel #1 : \f$ 0.5*(M_{11} - M_{22}) \f$
 * - channel #2 : \f$ 0.5*(M_{11} + M_{22} - 2*M_{12}) \f$
 * - channel #3 : \f$ 0.5*(M_{13} + M_{23} + i*(M_{14}+M_{24})) \f$
 * - channel #4 : \f$ 0.5*(M_{33} - M_{44} - 2*i*M_{34}) \f$
 * - channel #5 : \f$ 0.5*(M_{13} - M_{23} - i*(M_{14}-M_{24})) \f$
 *
 * Where \f$M_{ij}\f$ are the coefficients of the input Mueller matrix.
 *
 * Input pixel must have 10 channels (one for each Mueller matrix coeffcients).
 * The order of the channels corresponds to :
 * \f$ \begin{pmatrix}
 * {channel #0 }&{channel #1 }&{channel #2 }&{channel #3 } \\
 * {channel #4 }&{channel #5 }&{channel #6 }&{channel #7 } \\
 * {channel #8 }&{channel #9 }&{channel #10}&{channel #11} \\
 * {channel #12}&{channel #13}&{channel #14}&{channel #15} \\
 * \end{pmatrix} \f$
 *
 * The output image has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa MuellerToCircularPolarisationImageFilter
 * \sa MuellerToPolarisationDegreeAndPowerImageFilter
 *
 */

template< class TInput, class TOutput>
class MuellerToReciprocalCovarianceFunctor
{
public:
  typedef std::complex<double>                      ComplexType;
  typedef typename TOutput::ValueType               OutputValueType;


  inline TOutput operator()( const TInput & Mueller ) const
  {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    // Keep the upper diagonal of the matrix
    const double M11 =  static_cast<double>(Mueller[0]);
    const double M12 =  static_cast<double>(Mueller[1]);
    const double M13 =  static_cast<double>(Mueller[2]);
    const double M14 =  static_cast<double>(Mueller[3]);
    const double M21 =  static_cast<double>(Mueller[4]);
    const double M22 =  static_cast<double>(Mueller[5]);
    const double M23 =  static_cast<double>(Mueller[6]);
    const double M24 =  static_cast<double>(Mueller[7]);
    const double M31 =  static_cast<double>(Mueller[8]);
    const double M32 =  static_cast<double>(Mueller[9]);
    const double M33 =  static_cast<double>(Mueller[10]);
    const double M34 =  static_cast<double>(Mueller[11]);
    const double M41 =  static_cast<double>(Mueller[12]);
    const double M42 =  static_cast<double>(Mueller[13]);
    const double M43 =  static_cast<double>(Mueller[14]);
    const double M44 =  static_cast<double>(Mueller[15]);

    const ComplexType hhhh(M11+M22+M12+M21, 0.0);
    const ComplexType hvhv(M11+M12-M21-M22, 0.0);
    const ComplexType vvvv(M11+M22-M12-M21, 0.0);
    const ComplexType hhhv(M13+M23, M14+M24);
    const ComplexType hhvv(-M33-M44, M43-M34);
    const ComplexType hvvv(M32-M31, M41-M42);

    result[0] = static_cast<OutputValueType>( hhhh );
    result[1] = static_cast<OutputValueType>( 2.* hhhv );
    result[2] = static_cast<OutputValueType>( hhvv );
    result[3] = static_cast<OutputValueType>( 4.* hvhv );
    result[4] = static_cast<OutputValueType>( 2.* hvvv );
    result[5] = static_cast<OutputValueType>( vvvv );

    return 0.5*result;
  }

  unsigned int GetOutputSize()
    {
     return m_NumberOfComponentsPerPixel;
    }

  /** Constructor */
  MuellerToReciprocalCovarianceFunctor() : m_NumberOfComponentsPerPixel(6)  {}

  /** Destructor */
  virtual ~MuellerToReciprocalCovarianceFunctor() {}

 private:
  unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMuellerToReciprocalCovarianceImageFilter
 * \brief Compute the MLC  image
 * from the Mueller image (16 real channels)
 * For more details, lease refer to the class MuellerToReciprocalCovarianceFunctor.
 *
 * \ingroup SARPolarimetry
 * \sa MuellerToReciprocalCovarianceFunctor
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MuellerToReciprocalCovarianceImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::MuellerToReciprocalCovarianceFunctor<
    typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
   /** Standard class typedefs. */
   typedef MuellerToReciprocalCovarianceImageFilter  Self;
   typedef Functor::MuellerToReciprocalCovarianceFunctor<
     typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctionType;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, FunctionType> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToReciprocalCovarianceImageFilter, UnaryFunctorImageFilter);


protected:
   MuellerToReciprocalCovarianceImageFilter() {}
  virtual ~MuellerToReciprocalCovarianceImageFilter() {}

private:
  MuellerToReciprocalCovarianceImageFilter(const Self&); // purposely not implemented
  void operator=(const Self&);          // purposely not implemented
};

} // end namespace otb

#endif
