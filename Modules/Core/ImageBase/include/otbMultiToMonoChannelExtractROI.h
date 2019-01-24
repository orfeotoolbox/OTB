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

#ifndef otbMultiToMonoChannelExtractROI_h
#define otbMultiToMonoChannelExtractROI_h

#include "otbExtractROIBase.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#include "itkMacro.h"

namespace otb
{

/** \class MultiToMonoChannelExtractROI
 * \brief Extract a mono channel part of a multi-channel image.
 *
 * This filter extracts either all channels or only those specified by the user.
 * The SetChannel method allows selecting the channels to process.
 * \note If nothing is specified, only the first channel is processed.
 * \note This class is templated over the pixel types of the input and output images.
 * The input image has to be an otb::VectorImage, whereas the output image has to be an otb::Image.
 *
 * \ingroup OTBImageBase
 */
template <class TInputPixelType, class TOutputPixelType>
class ITK_EXPORT MultiToMonoChannelExtractROI :
//    public ExtractROIBase< itk::VectorImage<TInputPixelType, 2> , itk::Image<TOutputPixelType, 2> >
  public ExtractROIBase<VectorImage<TInputPixelType, 2>, Image<TOutputPixelType, 2> >
{
public:
  /** Standard class typedefs. */
  typedef MultiToMonoChannelExtractROI Self;
//  typedef ExtractROIBase< itk::VectorImage<TInputPixelType, 2> , itk::Image<TOutputPixelType, 2> > Superclass;
  typedef ExtractROIBase<VectorImage<TInputPixelType, 2>, Image<TOutputPixelType, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiToMonoChannelExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Pixel type information */
  typedef typename OutputImageType::ValueType OutputValueType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType  InputImageIndexType;
  typedef typename OutputImageType::SizeType  OutputImageSizeType;
  typedef typename InputImageType::SizeType   InputImageSizeType;

  /** Select the channel to process */
  itkSetMacro(Channel, unsigned int);
  itkGetConstMacro(Channel, unsigned int);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiToMonoChannelExtractROI();
  ~MultiToMonoChannelExtractROI() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** ExtractImageFilter can produce an image which is a different
   * resolution than its input image.  As such, ExtractImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  void GenerateOutputInformation() override;

  /** ExtractImageFilter can be implemented as a multithreaded filter.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) override;

private:
  MultiToMonoChannelExtractROI(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Channel to process [1...] */
  unsigned int m_Channel;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiToMonoChannelExtractROI.hxx"
#endif

#endif
