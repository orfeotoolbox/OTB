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

#include "itkUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMuellerToMLCFunctor
 * \brief Evaluate the  MLC image from the Mueller image
 *
 */
template< class TInput, class TOutput>
class MuellerToMLCFunctor
{
public:
  typedef double                                    RealType;
  typedef std::complex<double>                      ComplexType;
  typedef typename TOutput::ValueType               OutputValueType;
  typedef typename TOutput::ValueType               OutputValueType;


  inline TOutput operator()( const TInput & Mueller ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    RealType M11 =  static_cast<RealType>(Mueller[0]);
    RealType M12 =  static_cast<RealType>(Mueller[1]);
    RealType M13 =  static_cast<RealType>(Mueller[2]);
    RealType M14 =  static_cast<RealType>(Mueller[3]);
    RealType M21 =  static_cast<RealType>(Mueller[4]);
    RealType M22 =  static_cast<RealType>(Mueller[5]);
    RealType M23 =  static_cast<RealType>(Mueller[6]);
    RealType M24 =  static_cast<RealType>(Mueller[7]);
    RealType M31 =  static_cast<RealType>(Mueller[8]);
    RealType M32 =  static_cast<RealType>(Mueller[9]);
    RealType M33 =  static_cast<RealType>(Mueller[10]);
    RealType M34 =  static_cast<RealType>(Mueller[11]);
    RealType M41 =  static_cast<RealType>(Mueller[12]);
    RealType M42 =  static_cast<RealType>(Mueller[13]);
    RealType M43 =  static_cast<RealType>(Mueller[14]);
    RealType M44 =  static_cast<RealType>(Mueller[15]);

    RealType hhhh = M11+M22+2.*M12;   // C1  <hh.hh*>
    RealType hvhv = M11-M22;          // C2  <hv.hv*>
    RealType vvvv = M11+M22-2.*M12;   // C3  <vv.vv*>
    RealType Rehhhv = M13+M23;           // C4  Re<hh.hv*>
    RealType Imhhhv = -1.*(M14+M24);   // C5   Im<hh.hv*>
    RealType Rehhvv = M33-M44;  // C6  Re<hh.vv*>
    RealType Imhhvv = -2.*M34;   // C7  Im<hh.vv*>
    RealType Rehvvv = M13-M23;  // C8  Re<hv.vv>
    RealType Imhvvv = -1.*(M14-M24);  // C9  Im<hv.vv*>


    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MuellerToMLCFunctor() : m_NumberOfComponentsPerPixel(6)  {}

   /** Destructor */
   ~MuellerToMLCFunctor() {}

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
   public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MuellerToMLCImageFilter  Self;
   typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToMLCImageFilter,itk::UnaryFunctorImageFilter);


protected:
   MuellerToMLCImageFilter() {}
  virtual ~MuellerToMLCImageFilter() {}

private:
  MuellerToMLCImageFilter(const Self&); // purposely not implemented
  void operator=(const Self&);          // purposely not implemented
};
  
} // end namespace otb

#endif
