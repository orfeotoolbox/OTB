/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbSubsampleImageFilter_h
#define otbSubsampleImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "otbMacro.h" //for OTB_DISABLE_DYNAMIC_MT;

//Just to get the forward/inverse definitions
#include "otbWaveletOperatorBase.h"

namespace otb {

/** \class SubsampleImageFilter
 * \brief Performs a down sampling of an image
 *
 * This class performs a simple resampling image filtering.
 *
 * The third template is a Wavelet::FORWARD/Wavelet::INVERSE Wavelet::WaveletDirection
 * enumeration type. With Wavelet::FORWARD, the output image is of the same size of the one of
 * the input image. But the output image contains only co-located subsample, the rest being 0.
 * When Wavelet::INVERSE is used, the input image is considered as the sub-sampled. Then, the output
 * image is larger. Initial pixel values are preserved but the output image is interleaved with
 * 0.
 *
 * \sa ResampleImageFilter
 * \sa SubsampleImageRegionConstIterator
 * \sa DecimateImageFilter
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage,
    Wavelet::WaveletDirection TDirectionOfTransformation>
class ITK_EXPORT SubsampleImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SubsampleImageFilter                               Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SubsampleImageFilter, ImageToImageFilter);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Direction definition */
  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, TDirectionOfTransformation);

  /** Image typedef support. */
  typedef TInputImage                                  InputImageType;
  typedef typename InputImageType::RegionType          InputImageRegionType;
  typedef typename InputImageType::IndexType           InputImageIndexType;
  typedef typename InputImageIndexType::IndexValueType InputImageIndexValueType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType   OutputImageSizeType;
  typedef typename OutputImageType::IndexType  OutputImageIndexType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  /** Set/Get the SubsampleFactor */
  itkGetMacro(SubsampleFactor, const InputImageIndexType &);
  itkSetMacro(SubsampleFactor, InputImageIndexType &);
  void SetSubSampleFactor(InputImageIndexValueType factor)
  {
    InputImageIndexType indexFactor;
    indexFactor.Fill(factor);
    SetSubSampleFactor(indexFactor);
  }

protected:
  SubsampleImageFilter ()
    {
    OTB_DISABLE_DYNAMIC_MT;
    m_SubsampleFactor.Fill(1);
    }
  ~SubsampleImageFilter() override {}

  /** Internal test function to check if there is any direction to subsample */
  bool IsSubsampleFactorOne() const;

  /** Since input and output image are very likely to be of different size.
   * Region estimation functions has to be reimplemented
   */
  void CallCopyOutputRegionToInputRegion
    (InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion) override;
  void CallCopyInputRegionToOutputRegion
    (OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion) override;

  /** Output image region size is not of the same dimension as the input.
   * That is why GenerateOutputInformation has to be redefined.
   */
  void GenerateOutputInformation() override;

  /** Set output image to 0 before processing */
  void BeforeThreadedGenerateData() override;

  /** Allows multithreading */
  void ThreadedGenerateData
    (const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SubsampleImageFilter (const Self &) = delete;
  void operator =(const Self&) = delete;

  InputImageIndexType m_SubsampleFactor;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSubsampleImageFilter.hxx"
#endif

#endif
