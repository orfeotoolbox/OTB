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
#ifndef otbImaginaryImageToComplexImageFilter_h
#define otbImaginaryImageToComplexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class ImaginaryImageToComplexImageFilter
 * \brief Computes pixel-wise the complex image of a imaginary image.
 *
 * \sa ImaginaryImageToComplexImageFilter
 * \ingroup Multithreaded
 *
 * \ingroup OTBCommon
 */

namespace Function {

template< class TInputImageImaginaryPart, class TOutput>
class ImaginaryToComplex
{
public:
  typedef typename TOutput::value_type OutputValueType;

  ImaginaryToComplex() {}
  ~ImaginaryToComplex() {}
  bool operator!=( const ImaginaryToComplex & ) const
    {
    return false;
    }
  bool operator==( const ImaginaryToComplex & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInputImageImaginaryPart & imaginary) const
    {
    return TOutput(static_cast<OutputValueType>(0.0), static_cast<OutputValueType>(imaginary));
    }
};
}

template <class TInputImageImaginaryPart, class TOutputImage>
class ITK_EXPORT ImaginaryImageToComplexImageFilter :
    public itk::UnaryFunctorImageFilter<TInputImageImaginaryPart, TOutputImage,
                        Function::ImaginaryToComplex<
                                  typename TInputImageImaginaryPart::PixelType,
                                  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ImaginaryImageToComplexImageFilter  Self;
  typedef itk::UnaryFunctorImageFilter<
      TInputImageImaginaryPart, TOutputImage,
      Function::ImaginaryToComplex< typename TInputImageImaginaryPart::PixelType,
                              typename TOutputImage::PixelType> >
                                         Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ImaginaryImageToComplexImageFilter,
               UnaryFunctorImageFilter);

  typedef typename TInputImageImaginaryPart::PixelType              InputImaginaryPartPixelType;
  typedef typename TOutputImage::PixelType                          OutputPixelType;
  typedef typename itk::NumericTraits< OutputPixelType >::ValueType OutputPixelValueType;

protected:
  ImaginaryImageToComplexImageFilter() {}
  ~ImaginaryImageToComplexImageFilter() ITK_OVERRIDE {}

private:
  ImaginaryImageToComplexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
