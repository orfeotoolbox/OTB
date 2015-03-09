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
#ifndef __otbRealImageToComplexImageFilter_h
#define __otbRealImageToComplexImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class RealImageToComplexImageFilter
 * \brief Computes pixel-wise the complex image of a real image.
 *
 * \sa RealImageToComplexImageFilter
 * \ingroup Multithreaded
 *
 * \ingroup OTBImageManipulation
 */

namespace Function {

template< class TInputImageRealPart, class TOutput>
class RealToComplex
{
public:
  typedef typename TOutput::value_type OutputValueType;

  RealToComplex() {}
  ~RealToComplex() {}
  bool operator!=( const RealToComplex & ) const
    {
    return false;
    }
  bool operator==( const RealToComplex & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInputImageRealPart & real) const
    {
    return TOutput(static_cast<OutputValueType>(real), static_cast<OutputValueType>(0.0));
    }
};
}

template <class TInputImageRealPart, class TOutputImage>
class ITK_EXPORT RealImageToComplexImageFilter :
    public itk::UnaryFunctorImageFilter<TInputImageRealPart, TOutputImage,
                        Function::RealToComplex<
                                  typename TInputImageRealPart::PixelType,
                                  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef RealImageToComplexImageFilter  Self;
  typedef itk::UnaryFunctorImageFilter<
      TInputImageRealPart, TOutputImage,
      Function::RealToComplex< typename TInputImageRealPart::PixelType,
                              typename TOutputImage::PixelType> >
                                         Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RealImageToComplexImageFilter,
               UnaryFunctorImageFilter);

  typedef typename TInputImageRealPart::PixelType                   InputRealPartPixelType;
  typedef typename TOutputImage::PixelType                          OutputPixelType;
  typedef typename itk::NumericTraits< OutputPixelType >::ValueType OutputPixelValueType;

protected:
  RealImageToComplexImageFilter() {}
  virtual ~RealImageToComplexImageFilter() {}

private:
  RealImageToComplexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
