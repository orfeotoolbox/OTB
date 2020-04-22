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

#ifndef otbLocalHistogramImageFunction_h
#define otbLocalHistogramImageFunction_h

#include "itkImageFunction.h"
#include "itkHistogram.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * \class LocalHistogramImageFunction
 * \brief Calculate a local histogram over a specified circular neighborhood
 *
 * This image function computes the local histogram of the pixels lying
 * in a circular neighborhood around the given position.
 *
 * Pixel count can be optionally weighted by a gaussian kernel with
 * \f$ \sigma = 0.5 radius \f$ so that pixel far from the center position
 * account less in the histogram than pixel near the center position.
 * This option can be deactivated using the GaussianSmoothing flag.
 *
 * Histogram mininimum value, maximum value and number of bins can be
 * set using the Setters/Getters.
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBStatistics
 */

template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT LocalHistogramImageFunction
    : public itk::ImageFunction<TInputImage, typename itk::Statistics::Histogram<typename TInputImage::PixelType>::Pointer, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef LocalHistogramImageFunction Self;
  typedef itk::ImageFunction<TInputImage, typename itk::Statistics::Histogram<typename TInputImage::PixelType>::Pointer, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LocalHistogramImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef typename Superclass::OutputType                             OutputType;
  typedef itk::Statistics::Histogram<typename TInputImage::PixelType> HistogramType;
  typedef typename HistogramType::Pointer                             HistogramPointer;

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

  /** Set/Get the number of histogram bins. Default is 128. */
  itkSetClampMacro(NumberOfHistogramBins, unsigned long, 1, itk::NumericTraits<unsigned long>::max());
  itkGetConstMacro(NumberOfHistogramBins, unsigned long);

  itkSetMacro(HistogramMin, double);
  itkGetConstReferenceMacro(HistogramMin, double);

  itkSetMacro(HistogramMax, double);
  itkGetConstReferenceMacro(HistogramMax, double);

  itkSetMacro(GaussianSmoothing, bool);
  itkGetConstReferenceMacro(GaussianSmoothing, bool);
  itkBooleanMacro(GaussianSmoothing);

protected:
  LocalHistogramImageFunction();
  ~LocalHistogramImageFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LocalHistogramImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int  m_NeighborhoodRadius;
  unsigned long m_NumberOfHistogramBins;
  double        m_HistogramMin;
  double        m_HistogramMax;
  bool          m_GaussianSmoothing;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLocalHistogramImageFunction.hxx"
#endif

#endif
