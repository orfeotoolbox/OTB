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

#ifndef __MuellerToMLCImageFilter_h
#define __MuellerToMLCImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMuellerToMLCFunctor
 * \brief Evaluate the  MLC image from the Mueller image
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa MuellerToCircularPolarisationImageFilter
 * \sa MuellerToPolarisationDegreeAndPowerImageFilter
 *
 */
template< class TInput, class TOutput>
class MuellerToMLCFunctor
{
public:
  typedef double                                    RealType;
  typedef std::complex<double>                      ComplexType;
  typedef typename TOutput::ValueType               OutputValueType;


  inline TOutput operator()( const TInput & Mueller ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    const RealType M11 =  static_cast<RealType>(Mueller[0]);
    const RealType M12 =  static_cast<RealType>(Mueller[1]);
    const RealType M13 =  static_cast<RealType>(Mueller[2]);
    const RealType M14 =  static_cast<RealType>(Mueller[3]);
    const RealType M21 =  static_cast<RealType>(Mueller[4]);
    const RealType M22 =  static_cast<RealType>(Mueller[5]);
    const RealType M23 =  static_cast<RealType>(Mueller[6]);
    const RealType M24 =  static_cast<RealType>(Mueller[7]);
    const RealType M31 =  static_cast<RealType>(Mueller[8]);
    const RealType M32 =  static_cast<RealType>(Mueller[9]);
    const RealType M33 =  static_cast<RealType>(Mueller[10]);
    const RealType M34 =  static_cast<RealType>(Mueller[11]);
    const RealType M41 =  static_cast<RealType>(Mueller[12]);
    const RealType M42 =  static_cast<RealType>(Mueller[13]);
    const RealType M43 =  static_cast<RealType>(Mueller[14]);
    const RealType M44 =  static_cast<RealType>(Mueller[15]);

    const ComplexType hhhh(M11+M22+2.*M12, 0.0);
    const ComplexType hvhv(M11-M22, 0.0);
    const ComplexType vvvv(M11+M22-2.*M12, 0.0);
    const ComplexType hhhv(M13+M23, -1.*(M14+M24));
    const ComplexType hhvv(M33-M44, -2.*M34);
    const ComplexType hvvv(M13-M23, -1.*(M14-M24));

    result[0] = static_cast<OutputValueType>( hhhh );
    result[1] = static_cast<OutputValueType>( 2.* hhhv );
    result[2] = static_cast<OutputValueType>( hhvv );
    result[3] = static_cast<OutputValueType>( 4.* hvhv );
    result[4] = static_cast<OutputValueType>( 2.* hvvv );
    result[5] = static_cast<OutputValueType>( vvvv );

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MuellerToMLCFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   virtual ~MuellerToMLCFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMuellerToMLCImageFilter
 * \brief Compute the MLC  image
 * from the Mueller image (16 real channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MuellerToMLCFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MuellerToMLCImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MuellerToMLCImageFilter  Self;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToMLCImageFilter, UnaryFunctorImageFilter);


protected:
   MuellerToMLCImageFilter() {}
  virtual ~MuellerToMLCImageFilter() {}

private:
  MuellerToMLCImageFilter(const Self&); // purposely not implemented
  void operator=(const Self&);          // purposely not implemented
};
  
} // end namespace otb

#endif
