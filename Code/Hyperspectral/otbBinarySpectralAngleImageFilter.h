/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinarySpectralAngleImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:05 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbBinarySpectralAngleImageFilter_h
#define __otbBinarySpectralAngleImageFilter_h

#include "otbBinarySpectralAngleFunctor.h"
#include "otbBinaryFunctorImageFilter.h"
#include "itkConceptChecking.h"

namespace otb
{
  
/** \class BinarySpectralAngleImageFilter
 * \brief Implements pixel-wise the computation of spectral angle.
 *
 * This filter is parametrized over the types of the two 
 * input images and the type of the output image. 
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following: 
 *
 * - cast the input 1 pixel value to \c double 
 * - cast the input 2 pixel value to \c double 
 * - compute the spectral angle of the two pixel values
 * - cast the \c double value resulting from \c the value to the pixel type of the output image
 * - store the casted value into the output image.
 * 
 * The filter expect all images to have the same dimension 
 * (e.g. all 2D, or all 3D, or all ND)
 * 
 * \ingroup IntensityImageFilters Multithreaded
 */

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT BinarySpectralAngleImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage,
  Functor::BinarySpectralAngleFunctor<
    typename TInputImage1::PixelType,
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef BinarySpectralAngleImageFilter<TInputImage1,TInputImage2,TOutputImage>  Self;
  typedef BinaryFunctorImageFilter<TInputImage1,TInputImage2,TOutputImage, 
                                   Functor::BinarySpectralAngleFunctor<
    typename TInputImage1::PixelType, 
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType>   
  >  Superclass;

  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(BinarySpectralAngleImageFilter,
               BinaryFunctorImageFilter);

#if 0
#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(Input1CovertibleToDoubleCheck,
    (Concept::Convertible<typename TInputImage1::PixelType, double>));
  itkConceptMacro(Input2ConvertibleToDoubleCheck,
    (Concept::Convertible<typename TInputImage2::PixelType, double>));
  itkConceptMacro(DoubleCovertibleToOutputCheck,
    (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif
#endif

protected:
  BinarySpectralAngleImageFilter() {}
  virtual ~BinarySpectralAngleImageFilter() {}

private:
  BinarySpectralAngleImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb


#endif
