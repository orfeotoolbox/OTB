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

#ifndef otbExtractROI_h
#define otbExtractROI_h

#include "otbExtractROIBase.h"
#include "otbImage.h"
#include "itkMacro.h"
#include "OTBImageBaseExport.h"

namespace otb
{

/** \class ExtractROI
 * \brief Extract a subset of a mono-channel image.
 *
 * Pixel can be of a simple type or an itk::RGBPixel, etc.
 * \note Parameter to this class are the input pixel type and the output
 * pixel type.
 *
 *
 * \ingroup OTBImageBase
 */
template <class TInputPixel, class TOutputPixel>
class OTBImageBase_EXPORT_TEMPLATE ExtractROI : public ExtractROIBase<Image<TInputPixel, 2>, Image<TOutputPixel, 2>>
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI Self;
  typedef ExtractROIBase<Image<TInputPixel, 2>, Image<TOutputPixel, 2>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

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

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int, InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

protected:
  ExtractROI();
  ~ExtractROI() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** ExtractROI
   *
   * \sa ExtractROIBase::GenerateOutputInformation()  */
  void GenerateOutputInformation() override;

private:
  ExtractROI(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractROI.hxx"
#endif

#endif
