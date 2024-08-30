/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbFunctionToImageFilter_h
#define otbFunctionToImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageFunction.h"
#include "itkImageRegionIterator.h"
#include "itkSize.h"
#include "itkSpatialFunction.h"

namespace otb
{

/** \class FunctionToImageFilter
 * \brief Evaluates a ImageFunction onto a source image
 *
 * The class walks an input image and evaluates
 * the function at every pixel location. The output of the spatial function
 * and the pixel type of the output image must be compatible.
 *
 * Like its parent ImageToImageFilter, this class functions in the filtering
 * pipeline and produces a unique output image.
 *
 * The function has to inherit from itkImageFunction
 *
 * \sa SpatialFunctionImageEvaluatorFilter
 * \sa SpatialFunctionImageFilter
 * \sa ImageFunction
 * \ingroup ImageFilters
 *
 * \ingroup OTBCommon
 */

template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT FunctionToImageFilter : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef FunctionToImageFilter Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FunctionToImageFilter, InPlaceImageFilter);

  /** Some typedefs. */
  /** Image size typedef. */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  /** Type of function. */
  typedef TFunction                         FunctionType;
  typedef typename FunctionType::Pointer    FunctionPointer;
  typedef typename FunctionType::OutputType FunctionValueType;
  typedef typename FunctionType::InputType  FunctionPositionType;

  /** Set the internal spatial function. */
  void SetFunction(FunctionType* PixelFunction)
  {
    m_PixelFunction = PixelFunction;
    this->Modified();
  }
  FunctionType* GetFunction()
  {
    return m_PixelFunction;
  }
  FunctionType const* GetFunction() const
  {
    return m_PixelFunction;
  }


  /** Image dimensions */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

protected:
  FunctionToImageFilter();
  ~FunctionToImageFilter() override
  {
  }

  /** Validate the presence of all three inputs. If one or more inputs
   * are missing, throw an exception. */
  void BeforeThreadedGenerateData() override;

  /** SpatialFunctionImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  FunctionToImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The function that will be evaluated over the image */
  FunctionPointer m_PixelFunction;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFunctionToImageFilter.hxx"
#endif

#endif
