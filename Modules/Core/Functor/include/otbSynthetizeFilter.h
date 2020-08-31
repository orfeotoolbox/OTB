/*
 * Copyright(C) 2005-2020 Centre National d'Etudes Spatiales(CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https: //www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbSynthetizeFilter_h
#define otbSynthetizeFilter_h

#include "otbZipIterator.h"
#include "itkImageToImageFilter.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageScanlineIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Filter that reduces/synthetize multiple input into a single output.
 *
 * This filter makes sure to avoid VectorImages. Instead it works on a
 * collection of scalar images.
 * \tparam TInputImage   Type of the input images
 * \tparam TOutputImage  Type of the output image
 * \tparam TFunctor      Type of the functor, meant to be auto-deduced by
 * `MakeSynthetizeFilter()`
 *
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \see for instance `otb::Wrapper::Synthetize`
 */
template <typename TInputImage, typename TOutputImage, typename TFunctor>
class SynthetizeFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:

  /**\name Convenient typedefs for simplifying declarations */
  //@{
  using InputImageType  = TInputImage;
  using OutputImageType = TOutputImage;
  using FunctorType     = TFunctor;
  //@}

  /**\name Extract dimension from input and output images */
  //@{
  itkStaticConstMacro(InputImageDimension,  unsigned int, InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);
  //@}
  /**\name Standard class typedefs */
  //@{
  using Self            = SynthetizeFilter;
  using Superclass      = itk::ImageToImageFilter<InputImageType, OutputImageType>;
  using Pointer         = itk::SmartPointer<Self>;
  using ConstPointer    = itk::SmartPointer<const Self>;
  //@}

  /** Method for creation through the object factory. */
  static Pointer New(FunctorType functor)
  {
    Pointer smartPtr = new Self(std::move(functor));
    smartPtr->UnRegister();
    return smartPtr;
  }

  /** Run-time type information (and related methods). */
  itkTypeMacro(SynthetizeFilter, ImageToImageFilter);

  /**\name Image typedef support */
  //@{
  using InputPixelType        = typename InputImageType::PixelType;
  using OutputPixelType       = typename OutputImageType::PixelType;
  using InputRealType         = typename itk::NumericTraits<InputPixelType>::RealType;
  using InputImageRegionType  = typename InputImageType::RegionType;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using InputSizeType         = typename InputImageType::SizeType;
  using OutputIndexType       = typename OutputImageType::IndexType;
  using OutputSizeType        = typename OutputImageType::SizeType;

  static_assert(InputImageDimension == OutputImageDimension, "Images have the same number of components");

  using DataObjectPointerArraySizeType = itk::ProcessObject::DataObjectPointerArraySizeType;
   //@}

  /**\name Access to N-th Input
   *
   * Set/Get the nth filter input with or without a specified associated
   * variable name.
   */
  //@{
  using Superclass::SetNthInput;
  using Superclass::GetInput;

  /** Return a pointer on the nth filter input */
  InputImageType* GetNthInput(DataObjectPointerArraySizeType idx)
  {
    return const_cast<InputImageType*>(this->GetInput(idx));
  }

  /** Returns a vector of input images. */
  std::vector<InputImageType const*> GetInputs() const
  {
    std::vector<InputImageType const*> res;
    auto const  nbInputImages = this->GetNumberOfInputs();
    res.reserve(nbInputImages);
    for (std::size_t i = 0 ; i != nbInputImages ; ++i)
      res.push_back(this->GetInput(i));
    return res;
  }
  //@}

protected:
  /** Init constructor. */
  explicit SynthetizeFilter(FunctorType functor)
    : m_functor(functor){}
  ~SynthetizeFilter() = default;

  /** Overrides `GenerateOutputInformation` to check images consistency. */
  void GenerateOutputInformation() override
  {
    Superclass::GenerateOutputInformation();
    CheckInputImageDimensions();
  }

  // void GenerateInputRequestedRegion() override;
  // +-> TODO: detect neighborhood to apply pad radius

  /**
   * Main computation function called by each thread.
   * \param[in] outputRegionForThread  Specified output region to compute
   * \param[in] threadId               Id of the computing threads
   */
  void ThreadedGenerateData(
      OutputImageRegionType const& outputRegionForThread,
      itk::ThreadIdType            threadId) override
  {
    using ImageScanlineConstIteratorType = itk::ImageScanlineConstIterator<InputImageType const>;
    using OutImageScanlineConstIteratorType = itk::ImageScanlineIterator<OutputImageType>;
    using OutputIterator  = itk::ImageScanlineIterator<OutputImageType>;
    using InputIterator   = ZipConstIterator<ImageScanlineConstIteratorType>;

    auto const regSizeY = outputRegionForThread.GetSize()[1];
    itk::ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() / regSizeY );

    InputIterator  inputIterator(this->GetInputs(), outputRegionForThread);
    OutputIterator outputIterator(this->GetOutput(), outputRegionForThread);

    inputIterator.GoToBegin();
    outputIterator.GoToBegin();
    for (
        ; !inputIterator.IsAtEnd()
        ; inputIterator.NextLine(), outputIterator.NextLine())
    {
      assert(! outputIterator.IsAtEnd());
      // inputIterator.GoToBeginOfLine();
      // outputIterator.GoToBeginOfLine();
      for (
          ; !inputIterator.IsAtEndOfLine()
          ; ++inputIterator, ++outputIterator)
      {
        assert(!outputIterator.IsAtEndOfLine());

        outputIterator.Set(m_functor(inputIterator.Get()));
      }
      progress.CompletedPixel(); // Completed...Line()
    }
  }

private:

  void CheckInputImageDimensions()
  {
    // Check if input image dimensions match
    auto const  nbInputImages = this->GetNumberOfInputs();
    auto const& inputSize = this->GetInput(0)->GetLargestPossibleRegion().GetSize();

    for (auto p = 1U; p < nbInputImages; ++p)
    {
      auto const& regionSize = this->GetInput(p)->GetLargestPossibleRegion().GetSize();
      if (inputSize != regionSize)
      {
        itkExceptionMacro(<< "Input images must have the same dimensions.\n"
            << "band #1 is [" << inputSize[0] << ";" << inputSize[1] << "]\n"
            << "band #" << p + 1 << " is [" << this->GetInput(p)->GetLargestPossibleRegion().GetSize(0) << ";"
            << regionSize << "]");
      }
    }
  }

  /** Internal functor.
   * Can only be set at filter creation through call to `New`.
   */
  FunctorType m_functor;

};

/**
 * Factory function for `SynthetizeFilter`.
 * `SynthetizeFilter` objects are best made thanks to this factory function
 * that'll automatically deduce the type of the functor parameter.
 *
 * Actually, there is no other to create a `SynthetizeFilter<>` that'll call a
 * lambda on each pixel.
 * \tparam TInputImage  Type of the input images
 * \tparam TOutputImage Type of the output image
 * \tparam TFunctor     Type of the functor
 * \param[in] functor   The functor
 *
 * \return a new `SynthetizeFilter` object.
 * \sa `SynthetizeFilter`
 */
template <typename TInputImage, typename TOutputImage, typename TFunctor>
auto MakeSynthetizeFilter(TFunctor functor)
{
  auto filter = SynthetizeFilter<TInputImage, TOutputImage, TFunctor>::New(std::move(functor));
  return filter;
}

} // otb namespace

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbSynthetizeFilter.hxx"
#endif

#endif  // otbSynthetizeFilter_h
