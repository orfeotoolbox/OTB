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

#ifndef otbHistogramOfOrientedGradientCovariantImageFunction_h
#define otbHistogramOfOrientedGradientCovariantImageFunction_h

#include "itkImageFunction.h"
#include "itkFixedArray.h"

namespace otb
{

/**
 * \class HistogramOfOrientedGradientCovariantImageFunction
 * \brief Calculate the centered HOG features
 *
 * This filter implements the centered histogram of gradient
 * feature. It expects a gradient covariant image as input, such as
 * the output of the itk::GradientImageFilter. Steps of the algorithm
 * are as follows.
 *
 * In order to make the C-HOG descriptor rotation-invariant, a
 * principal gradient orientation is first looked for. Within the
 * m_NeighborhoodRadius ($r$), an histogram of the local orientation is
 * computed with m_NumberOfOrientationBins bins. Values cumulated in
 * this histogram are the gradient magnitude weighted by a gaussian
 * kernel of $\sigma = 0.5 * r$.
 *
 * From this orientation histogram, the principal orientation is
 * computed by looking for the maximum valued bin.
 *
 * Once principal orientation is computed,
 * gradient magnitude values weighted by the gaussian kernel are
 * cumulated in five different histograms corresponding to five
 * distinct spatial areas : the center area (radius of the center area
 * is computed using $r_{c}=log_{2}(r)$), and the upper-left,
 * upper-right, lower-left and lower-right radial areas. Orientation
 * of these radial areas is shifted to match the principal
 * orientation, and gradient orientations to determine histogram bins
 * are also compensated with the principal orientation. Last, each
 * histogram is normalized by its $L_2$ norm, ensuring that they
 * all lie in the range $[0, 1]$.
 *
 * This class is templated over the input image type, the output
 * precision (e.g. float or double) and the
 * coordinate representation type (e.g. float or double).

 * \ingroup ImageFunctions
 *
 * \ingroup OTBDescriptors
 */

template <class TInputImage, class TOutputPrecision = double, class TCoordRep = double>
class ITK_EXPORT HistogramOfOrientedGradientCovariantImageFunction
    : public itk::ImageFunction<TInputImage, std::vector<std::vector<TOutputPrecision>>, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef HistogramOfOrientedGradientCovariantImageFunction Self;
  typedef itk::ImageFunction<TInputImage, std::vector<std::vector<TOutputPrecision>>, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HistogramOfOrientedGradientCovariantImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef TOutputPrecision                OutputPrecisionType;
  typedef typename Superclass::OutputType OutputType;

  typedef TCoordRep CoordRepType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int, InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  OutputType EvaluateAtIndex(const IndexType& index) const override;

  /** Evaluate the function at non-integer positions */
  OutputType Evaluate(const PointType& point) const override
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& cindex) const override
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the radius of the neighborhood over which the
   *  statistics are evaluated
   */
  itkSetMacro(NeighborhoodRadius, unsigned int);
  itkGetConstReferenceMacro(NeighborhoodRadius, unsigned int);

  /** Get/Set the number of bins of the orientation histograms
   */
  itkSetMacro(NumberOfOrientationBins, unsigned int);
  itkGetConstReferenceMacro(NumberOfOrientationBins, unsigned int);


protected:
  HistogramOfOrientedGradientCovariantImageFunction();
  ~HistogramOfOrientedGradientCovariantImageFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  HistogramOfOrientedGradientCovariantImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Radius over which the principal orientation will be computed
  unsigned int m_NeighborhoodRadius;

  // Number of bins in the orientation
  unsigned int m_NumberOfOrientationBins;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramOfOrientedGradientCovariantImageFunction.hxx"
#endif

#endif
