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

#ifndef otbRealImageToComplexImageFilter_h
#define otbRealImageToComplexImageFilter_h

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
  ~RealImageToComplexImageFilter() override {}

private:
  RealImageToComplexImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

};

} // end namespace otb

#endif
