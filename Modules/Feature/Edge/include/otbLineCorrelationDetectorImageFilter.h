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

#ifndef otbLineCorrelationDetectorImageFilter_h
#define otbLineCorrelationDetectorImageFilter_h

#include "otbLineDetectorImageFilterBase.h"

namespace otb
{
/**
 *\class LineCorrelationDetectorImageFilter
 * \brief To be documented
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage, class TOutputImageDirection = TOutputImage,
          class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage>>
class ITK_EXPORT LineCorrelationDetectorImageFilter : public LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef LineCorrelationDetectorImageFilter Self;
  typedef LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineCorrelationDetectorImageFilter, LineDetectorImageFilterBase);

  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;
  typedef typename Superclass::InterpolatorType         InterpolatorType;

  /** Typedefs to describe and access Interpolator */
  typedef typename InterpolatorType::Pointer      InterpolatorPointer;
  typedef typename InterpolatorType::CoordRepType CoordRepType;

  typedef typename InputImageType::PointType TPoint;

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

protected:
  LineCorrelationDetectorImageFilter();
  ~LineCorrelationDetectorImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Compute the measure */
  double ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3) override;

private:
  LineCorrelationDetectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineCorrelationDetectorImageFilter.hxx"
#endif

#endif
