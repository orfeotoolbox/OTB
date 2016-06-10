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
#ifndef __otbComplexToIntensityImageFilter_h
#define __otbComplexToIntensityImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class ComplexToIntensityImageFilter
 * \brief Computes pixel-wise the intensity of a complex image.
 *
 * \sa ComplexToPhaseImageFilter ComplexToModulusImageFilter
 * \ingroup IntensityImageFilters  Multithreaded
 *
 * \ingroup OTBCommon
 */

namespace Function {

template< class TInput, class TOutput>
class ComplexToIntensity
{
public:
  ComplexToIntensity() {}
  ~ComplexToIntensity() {}
  bool operator!=( const ComplexToIntensity & ) const
    {
    return false;
    }
  bool operator==( const ComplexToIntensity & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    {
    return static_cast<TOutput>( A.real()*A.real() + A.imag()*A.imag() );
    }
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComplexToIntensityImageFilter :
    public
itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
                        Function::ComplexToIntensity<
  typename TInputImage::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ComplexToIntensityImageFilter  Self;
  typedef itk::UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    Function::ComplexToIntensity< typename TInputImage::PixelType,
                              typename TOutputImage::PixelType> >
                                         Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComplexToIntensityImageFilter,
               UnaryFunctorImageFilter);

  typedef typename TInputImage::PixelType                          InputPixelType;
  typedef typename TOutputImage::PixelType                         OutputPixelType;
  typedef typename itk::NumericTraits< InputPixelType >::ValueType InputPixelValueType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck,
    (itk::Concept::Convertible<InputPixelValueType, OutputPixelType>));
  /** End concept checking */
#endif


protected:
  ComplexToIntensityImageFilter() {}
  ~ComplexToIntensityImageFilter() ITK_OVERRIDE {}

private:
  ComplexToIntensityImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
