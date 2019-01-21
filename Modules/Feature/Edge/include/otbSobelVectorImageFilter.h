/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
                    std::sqrt( std::pow( hori, 2. ) + std::pow( vert, 2. ) ) );
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
  ~SobelVectorImageFilter() override { }

private:
  SobelVectorImageFilter( const Self & ); // Not implemented
  void operator=( const Self & ); // Not implemented
}; // end of class

} // end of namespace otb


#endif // otbSobelVectorImageFilter_h
