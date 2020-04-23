/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorImageTo3DScalarImageFilter_h
#define otbVectorImageTo3DScalarImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class VectorImageTo3DScalarImageFilter
 *  \brief This filter convert a 2 Dimension VectorImage to a 3 Dimension scalar image.
 *
 *  The third dimension of the output image is the index of the band in the vector image.
 *
 *  No Dimension checking is done by the filter so it is up to the user to template the input
 * and output image dimension correctly.
 *
 * Casting is done through standard casting operators.
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorImageTo3DScalarImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef VectorImageTo3DScalarImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageTo3DScalarImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename InputImageType::IndexType    InputImageIndexType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::IndexType   OutputImageIndexType;
  typedef typename OutputImageType::SizeType    OutputImageSizeType;
  typedef typename OutputImageType::PixelType   OutputPixelType;

protected:
  /** Generate output information */
  void GenerateOutputInformation(void) override;

  /** Generate input requested region */
  void GenerateInputRequestedRegion(void) override;

  /** Threaded Generate data */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  /** Constructor */
  VectorImageTo3DScalarImageFilter();
  /** Destructor */
  ~VectorImageTo3DScalarImageFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  VectorImageTo3DScalarImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageTo3DScalarImageFilter.hxx"
#endif

#endif
