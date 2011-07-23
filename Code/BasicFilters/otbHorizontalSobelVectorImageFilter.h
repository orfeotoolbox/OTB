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
#ifndef __otbHorizontalSobelVectorImageFilter_h
#define __otbHorizontalSobelVectorImageFilter_h

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"

namespace otb
{

namespace Functor
{
/** \class HorizontalSobelOperator
 * \brief Performs the calculation of horizontal Sobel derivation
 */
template < class TInput, class TOutput >
class HorizontalSobelOperator
{
public:
  HorizontalSobelOperator() { }
  virtual ~HorizontalSobelOperator() { }

  TOutput operator() ( const TInput & input )
  {
    unsigned int length = input.GetPixel(0).Size();
    TOutput output ( length );
    for ( unsigned int i = 0; i < length; ++i )
    {
      output[i] = static_cast<typename TOutput::ValueType>(
                 ( input.GetPixel(0)[i] - input.GetPixel(2)[i] )
                  + 2 * ( input.GetPixel(3)[i] - input.GetPixel(5)[i] )
                  + ( input.GetPixel(6)[i] - input.GetPixel(8)[i] ) );
    }
    return output;
  }
}; // end of functor class

} // end of namespace Functor

/** \class HorizontalSobelVectorImageFilter
 * \brief Implements the Horizontal Sobel Gradient to be processed on a vector image
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT HorizontalSobelVectorImageFilter
  : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::HorizontalSobelOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  /** Standart class typedefs */
  typedef HorizontalSobelVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::HorizontalSobelOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(HorizontalSobelVectorImageFilter, ImageToImageFilter);

protected:
  HorizontalSobelVectorImageFilter()
  {
    typename Superclass::RadiusType radius = {{1, 1}};
    this->SetRadius( radius );
  }
  virtual ~HorizontalSobelVectorImageFilter() { }

private:
  HorizontalSobelVectorImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class

} // end of namespace otb


#endif // __otbHorizontalSobelVectorImageFilter_h
