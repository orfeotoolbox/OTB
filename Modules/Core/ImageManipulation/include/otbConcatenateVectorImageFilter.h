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

#ifndef otbConcatenateVectorImageFilter_h
#define otbConcatenateVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbVectorImage.h"

namespace otb
{
/** \class ConcatenateVectorImageFilter
 * \brief This filter concatenates the vector pixel of the first
 * image with the vector pixel of the second image.
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT ConcatenateVectorImageFilter : public itk::ImageToImageFilter<TInputImage1, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ConcatenateVectorImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage1, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImageToImagePixelAccessor, ImageToImageFilter);

  /** Template related typedefs */
  typedef TInputImage1 InputImage1Type;
  typedef TInputImage2 InputImage2Type;
  typedef TOutputImage OutputImageType;

  typedef typename InputImage1Type::Pointer InputImage1PointerType;
  typedef typename InputImage2Type::Pointer InputImage2PointerType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  typedef typename InputImage1Type::PixelType InputPixel1Type;
  typedef typename InputImage2Type::PixelType InputPixel2Type;

  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  /**
   * Set The first input image.
   * \param image The first input image.
   */
  void SetInput1(const TInputImage1* image);
  /**
   * Get the first input image.
   * \return The first input image.
   */
  InputImage1Type* GetInput1(void);
  /**
   * Set The second input image.
   * \param image The second input image.
   */
  void SetInput2(const TInputImage2* image);
  /**
   * Get the second input image.
   * \return The second input image.
   */
  InputImage2Type* GetInput2(void);

  /** ImageDimension constant */
  itkStaticConstMacro(InputImage1Dimension, unsigned int, TInputImage1::ImageDimension);
  itkStaticConstMacro(InputImage2Dimension, unsigned int, TInputImage2::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

protected:
  /** Constructor. */
  ConcatenateVectorImageFilter();
  /** Destructor. */
  ~ConcatenateVectorImageFilter() override;
  void GenerateOutputInformation() override;
  void BeforeThreadedGenerateData() override;
  /** Main computation method. */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ConcatenateVectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateVectorImageFilter.hxx"
#endif
#endif
