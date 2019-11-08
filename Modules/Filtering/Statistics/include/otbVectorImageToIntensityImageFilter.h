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

#ifndef otbVectorImageToIntensityImageFilter_h
#define otbVectorImageToIntensityImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class VectorImageToIntensityImageFilter
 *  \brief This filter implements the computation of the mean of the spectral values of each pixel.
 *
 * The spectral mean
 * is defined as:
 *
 * \f[ SM = \frac{1}{n_{b}} \times \sum_{b=1}^{n_{b}}p(b) \f]
 * with \f$b\f$ being the spectral band and \f$p\f$
 * the current pixel.
 *
 * Since the spectral mean deals with multi-bands image, the InputImage pixels are supposed to
 * support the [] operator, and the input image to support the GetNumberOfComponentsPerPixel() method.
 *
 * \sa VectorImage
 *
 * This filter is implemented as a multithreaded filter.
 *
 * \ingroup IntensityImageFilters
 * \ingroup Threading
 * \ingroup Streamed
 *
 * \ingroup OTBStatistics
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorImageToIntensityImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef VectorImageToIntensityImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToIntensityImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputPixelType;

protected:
  /** Constructor */
  VectorImageToIntensityImageFilter();
  /** Destructor */
  ~VectorImageToIntensityImageFilter() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** VectorImageToIntensityImageFilter can be implemented as a multithreaded filter.
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
  VectorImageToIntensityImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageToIntensityImageFilter.hxx"
#endif

#endif
