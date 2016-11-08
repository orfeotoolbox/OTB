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
#ifndef otbSobelVectorImageFilter_h
#define otbSobelVectorImageFilter_h

#include "otbUnaryFunctorNeighborhoodVectorImageFilter.h"

#include <vnl/vnl_math.h>

namespace otb
{

namespace Functor
{
/** \class SobelOperator
 * \brief Performs the calculation of Sobel derivation
 *
 * \ingroup OTBEdge
 */
template < class TInput, class TOutput >
class SobelOperator
{
public:
  SobelOperator() { }
  virtual ~SobelOperator() { }

  TOutput operator() ( const TInput & input )
  {
    unsigned int length = input.GetPixel(0).Size();
    TOutput output ( length );
    for ( unsigned int i = 0; i < length; ++i )
    {
      double hori = static_cast<double>(
            ( input.GetPixel(0)[i] - input.GetPixel(2)[i] )
                + 2 * ( input.GetPixel(3)[i] - input.GetPixel(5)[i] )
                + ( input.GetPixel(6)[i] - input.GetPixel(8)[i] ) );
      double vert = static_cast<double>(
            ( input.GetPixel(0)[i] - input.GetPixel(6)[i] )
                  + 2 * ( input.GetPixel(1)[i] - input.GetPixel(7)[i] )
                  + ( input.GetPixel(2)[i] - input.GetPixel(8)[i] ) );

      output[i] = static_cast<typename TOutput::ValueType>(
                    vcl_sqrt( vcl_pow( hori, 2. ) + vcl_pow( vert, 2. ) ) );
    }
    return output;
  }
}; // end of functor class

} // end of namespace Functor

/** \class SobelVectorImageFilter
 * \brief Implements the  Sobel Gradient to be processed on a vector image
 *
 * \ingroup OTBEdge
 */
template < class TInputImage, class TOutputImage >
class ITK_EXPORT SobelVectorImageFilter
  : public UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::SobelOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs */
  typedef SobelVectorImageFilter Self;
  typedef UnaryFunctorNeighborhoodVectorImageFilter< TInputImage, TOutputImage,
            Functor::SobelOperator<
              typename itk::ConstNeighborhoodIterator<TInputImage>,
              typename TOutputImage::PixelType > >  Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SobelVectorImageFilter, ImageToImageFilter);

protected:
  SobelVectorImageFilter()
  {
    typename Superclass::RadiusType radius = {{1, 1}};
    this->SetRadius( radius );
  }
  ~SobelVectorImageFilter() ITK_OVERRIDE { }

private:
  SobelVectorImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class

} // end of namespace otb


#endif // otbSobelVectorImageFilter_h
