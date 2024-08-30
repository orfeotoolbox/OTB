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

#ifndef otbLineRatioDetectorImageFilter_h
#define otbLineRatioDetectorImageFilter_h

#include "otbLineDetectorImageFilterBase.h"

namespace otb
{

/** \class LineRatioDetectorImageFilter
 * \brief Application of detection of linear features based on the
 * ratio of local means.
 *
 * This class implements the Tupin's detector D1 used to detect
 * two parallel lines. This detector is derived from the coupling of two
 * ratio edge detectors (Touzi detector) on both side of a region.
 *
 * The region is divided in three zones to delimite two parallel lines.
 * The size of one zone is defined by the product of the width
 * of the linear feature by its length.
 *
 * For each vertical line, we calculate the intensity of detection
 * \f$ R_{12}(\theta_{0}) \f$ between zones 1 and 2 and \f$ R_{13}(\theta_{0})\f$ between
 * zones 1 and 3 according to the principle of the Touzi's filter.
 *
 * The response of the edge detector between two zones i and j is:
 *   \f[R_{ij}=1-\min (\frac{\mu_{i}}{\mu_{j}};\frac{\mu_{j}}{\mu_{i}}) \f]
 *
 * The intensity of detection in the three other directions \f$ R(\theta_{i}) \f$
 * is determined by rotation of the pixels of each zone around the
 * central pixel of the region considered. By default, the pixel location after
 * rotation is determined by the Spline interpolator.
 *
 * Finally, the intensity of detection formed by the two parallel lines
 * is determined by the minimum response of a ration edge detector on both sides
 * of the linear structure:
 *  \f[ R = \min (R_{12}; R_{13}) \f]
 * where \f$ R_{12} \f$ and \f$ R_{13}\f$ are the maximum response of the ratio edge
 * detector of \f$ R(\theta_{i}) \f$. The intensity of detection lies in
 * the interval \f$ [0, 1] \f$.
 *
 *
 *
 * \ingroup OTBEdge
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection = TOutputImage,
          class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage>>
class ITK_EXPORT LineRatioDetectorImageFilter : public LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef LineRatioDetectorImageFilter Self;
  typedef LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineRatioDetectorImageFilter, LineDetectorImageFilterBase);

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
  LineRatioDetectorImageFilter();
  ~LineRatioDetectorImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  double ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3) override;

private:
  LineRatioDetectorImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineRatioDetectorImageFilter.hxx"
#endif

#endif
