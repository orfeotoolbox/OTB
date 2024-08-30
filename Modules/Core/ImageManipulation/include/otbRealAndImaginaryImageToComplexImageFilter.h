/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRealAndImaginaryImageToComplexImageFilter_h
#define otbRealAndImaginaryImageToComplexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{

/** \class RealAndImaginaryImageToComplexImageFilter
 * \brief Computes pixel-wise the complex image of a real and imaginary image.
 *
 * \sa RealAndImaginaryImageToComplexImageFilter
 * \ingroup Multithreaded
 *
 * \ingroup OTBImageManipulation
 */

namespace Function
{

template <class TInputImageRealPart, class TInputImageImaginaryPart, class TOutput>
class RealAndImaginaryToComplex
{
public:
  typedef typename TOutput::value_type OutputValueType;

  RealAndImaginaryToComplex()
  {
  }
  ~RealAndImaginaryToComplex()
  {
  }
  bool operator!=(const RealAndImaginaryToComplex&) const
  {
    return false;
  }
  bool operator==(const RealAndImaginaryToComplex& other) const
  {
    return !(*this != other);
  }
  inline TOutput operator()(const TInputImageRealPart& real, const TInputImageImaginaryPart& imaginary) const
  {
    return TOutput(static_cast<OutputValueType>(real), static_cast<OutputValueType>(imaginary));
  }
};
}

template <class TInputImageRealPart, class TInputImageImaginaryPart, class TOutputImage>
class ITK_EXPORT RealAndImaginaryImageToComplexImageFilter
    : public itk::BinaryFunctorImageFilter<TInputImageRealPart, TInputImageImaginaryPart, TOutputImage,
                                           Function::RealAndImaginaryToComplex<typename TInputImageRealPart::PixelType,
                                                                               typename TInputImageImaginaryPart::PixelType, typename TOutputImage::PixelType>>
{
public:
  /** Standard class typedefs. */
  typedef RealAndImaginaryImageToComplexImageFilter Self;
  typedef itk::BinaryFunctorImageFilter<TInputImageRealPart, TInputImageImaginaryPart, TOutputImage,
                                        Function::RealAndImaginaryToComplex<typename TInputImageRealPart::PixelType,
                                                                            typename TInputImageImaginaryPart::PixelType, typename TOutputImage::PixelType>>
                                        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RealAndImaginaryImageToComplexImageFilter, BinaryFunctorImageFilter);

  typedef typename TInputImageRealPart::PixelType                 InputRealPartPixelType;
  typedef typename TInputImageImaginaryPart::PixelType            InputImaginaryPartPixelType;
  typedef typename TOutputImage::PixelType                        OutputPixelType;
  typedef typename itk::NumericTraits<OutputPixelType>::ValueType OutputPixelValueType;

  //#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  //  itkConceptMacro(InputConvertibleToOutputCheck,
  //    (Concept::Convertible<InputRealPartPixelType, OutputPixelValueType>));
  //  itkConceptMacro(InputConvertibleToOutputCheck,
  //    (Concept::Convertible<InputImaginaryPartPixelType, OutputPixelValueType>));
  /** End concept checking */
  //#endif

  void SetInputRealPart(const TInputImageRealPart* imageRealPart)
  {
    this->SetInput1(imageRealPart);
  }

  void SetInputImaginaryPart(const TInputImageImaginaryPart* imageImaginaryPart)
  {
    this->SetInput2(imageImaginaryPart);
  }

protected:
  RealAndImaginaryImageToComplexImageFilter()
  {
  }
  ~RealAndImaginaryImageToComplexImageFilter() override
  {
  }

private:
  RealAndImaginaryImageToComplexImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
