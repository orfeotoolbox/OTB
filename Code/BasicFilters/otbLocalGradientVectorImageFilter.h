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
#ifndef __otbLocalGradientVectorImageFilter_h
#define __otbLocalGradientVectorImageFilter_h

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"

#include <itkNumericTraits.h>

namespace otb
{

namespace Functor
{
/** \class LocalGradientOperator
 * \brief Performs the calculation of LocalGradient derivation
 */
template < class TInput, class TOutput >
class LocalGradientOperator
{
public:
  LocalGradientOperator() { }
  virtual ~LocalGradientOperator() { }

  TOutput operator() ( const TInput & input )
  {
    /*
     * it is assumed that input and output have the same size
     */
    unsigned int length = input.GetPixel(0).Size();
    TOutput output ( length );
    for ( unsigned int i = 0; i < length; ++i )
    {
      output[i] = static_cast<typename TOutput::ValueType>(
                  input.GetCenterPixel()[i]
                  - input.GetPixel(5)[i] / 2.
                  - input.GetPixel(7)[i] / 2. );
    }
    return output;
  }
}; // end of functor class

} // end of namespace Functor

/** \class LocalGradientVectorImageFilter
 * \brief Implements the 3x3 Local Gradient to be processed on a vector image
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT LocalGradientVectorImageFilter
  : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::LocalGradientOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  /** Standart class typedefs */
  typedef LocalGradientVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::LocalGradientOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LocalGradientVectorImageFilter, ImageToImageFilter);

protected:
  LocalGradientVectorImageFilter()
  {
    typename Superclass::RadiusType radius = {{1, 1}};
    this->SetRadius( radius );
  }
  virtual ~LocalGradientVectorImageFilter() { }

private:
  LocalGradientVectorImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class

} // end of namespace otb


#endif // __otbLocalGradientVectorImageFilter_h
