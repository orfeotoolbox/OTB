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

#ifndef __MuellerToCircularPolarisationImageFilter_h
#define __MuellerToCircularPolarisationImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMuellerToCircularPolarisationFunctor
 * \brief Evaluate the  Circular Polarisation image
 * (3 channels : LL, RR and LR)  from the Mueller image
 *
 *
 */
template< class TInput, class TOutput>
class MuellerToCircularPolarisationFunctor
{
public:
  typedef double                                    RealType;
  typedef typename TOutput::ValueType               OutputValueType;

  inline TOutput operator()( const TInput & Mueller ) const
    {
    TOutput result;
    result.SetSize(m_NumberOfComponentsPerPixel);

    RealType M11 =  static_cast<RealType>(Mueller[0]);
    RealType M14 =  static_cast<RealType>(Mueller[3]);
    RealType M41 =  static_cast<RealType>(Mueller[12]);
    RealType M44 =  static_cast<RealType>(Mueller[15]);

    result[0] = static_cast<OutputValueType>( M11 + M14 + M41 + M44 ); // LL
    result[1] = static_cast<OutputValueType>( M11 - M14 - M41 + M44 ); // RR
    result[2] = static_cast<OutputValueType>( M11 -M44 );              // LR

    return result;
    }

   unsigned int GetOutputSize()
   {
     return m_NumberOfComponentsPerPixel;
   }

   /** Constructor */
   MuellerToCircularPolarisationFunctor() : m_NumberOfComponentsPerPixel(3)  {}

   /** Destructor */
   ~MuellerToCircularPolarisationFunctor() {}

private:
    unsigned int m_NumberOfComponentsPerPixel;
};
}


/** \class otbMuellerToCircularPolarisationImageFilter
 * \brief Compute the circular polarisation image (3 channels : LL, RR and LR)
 * from the Mueller image (16 real channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MuellerToCircularPolarisationFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MuellerToCircularPolarisationImageFilter :
   public itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MuellerToCircularPolarisationImageFilter  Self;
   typedef itk::UnaryFunctorImageFilter<TInputImage,TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToCircularPolarisationImageFilter,itk::UnaryFunctorImageFilter);


protected:
   MuellerToCircularPolarisationImageFilter() {}
  virtual ~MuellerToCircularPolarisationImageFilter() {}

private:
  MuellerToCircularPolarisationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);                           //purposely not implemented
};
  
} // end namespace otb

#endif
