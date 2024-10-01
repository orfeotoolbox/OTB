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

#ifndef otbBCOInterpolateImageFunction_h
#define otbBCOInterpolateImageFunction_h

#include <boost/version.hpp>
#include <boost/container/small_vector.hpp>

#include "itkInterpolateImageFunction.h"
#include "otbMath.h"

#include "otbVectorImage.h"

namespace otb
{
/** \class BCOInterpolateImageFunction
 *  \brief Interpolate an image at specified positions using bicubic interpolation.
 *
 * BCOInterpolateImageFunction interpolates image intensity at
 * a non-integer pixel position. This class is templated
 * over the input image type and the coordinate representation type
 * (e.g. float or double).
 *
 * This function works for 2-dimensional images.
 *
 * This function works with both Images and VectorImages.
 *
 * Parameters are the interpolation window radius and the bicubic
 * optimisation coefficient alpha.
 * Alpha is usually set to -0.5, -0.75 or -1 (-0.5 by default).
 * The case alpha = -0.5 (which corresponds to the cubic Hermite
 * spline) is known to produce the best approximation of the original
 * function.
 *
 * \ingroup ImageFunctions ImageInterpolators
 *
 * \ingroup OTBInterpolation
 */
template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT BCOInterpolateImageFunctionBase : public itk::InterpolateImageFunction<TInputImage, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef BCOInterpolateImageFunctionBase                       Self;
  typedef itk::InterpolateImageFunction<TInputImage, TCoordRep> Superclass;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BCOInterpolateImageFunctionBase, InterpolateImageFunction);

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;

  /** InputPixelType typedef support. */
  typedef typename Superclass::InputPixelType InputPixelType;

  /** RealType typedef support. */
  typedef typename Superclass::RealType RealType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType      IndexType;
  typedef typename Superclass::IndexValueType IndexValueType;

  /** Point typedef support. */
  typedef typename Superclass::PointType PointType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef TCoordRep                                ContinuousIndexValueType;

  /** Coefficients container type. */
  typedef boost::container::small_vector<double, 7> CoefContainerType;

  /** Set/Get the window radius */
  virtual void         SetRadius(unsigned int radius);
  virtual unsigned int GetRadius() const;

  /** Set/Get the optimisation coefficient (Common values are -0.5, -0.75 or -1.0) */
  virtual void   SetAlpha(double alpha);
  virtual double GetAlpha() const;

  /** Evaluate the function at a ContinuousIndex position
   *
   * Returns the linearly interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& index) const override = 0;

protected:
  BCOInterpolateImageFunctionBase() : m_Radius(2), m_WinSize(5), m_Alpha(-0.5){};
  ~BCOInterpolateImageFunctionBase() override{};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Compute the BCO coefficients. */
  CoefContainerType EvaluateCoef(const ContinuousIndexValueType& indexValue) const;

  /** Used radius for the BCO */
  unsigned int m_Radius;
  /** Used winsize for the BCO */
  unsigned int m_WinSize;
  /** Optimisation Coefficient */
  double m_Alpha;

private:
  BCOInterpolateImageFunctionBase(const Self&) = delete;
  void operator=(const Self&) = delete;
};


template <class TInputImage, class TCoordRep = double>
class ITK_EXPORT BCOInterpolateImageFunction : public otb::BCOInterpolateImageFunctionBase<TInputImage, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef BCOInterpolateImageFunction                             Self;
  typedef BCOInterpolateImageFunctionBase<TInputImage, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  itkTypeMacro(BCOInterpolateImageFunction, BCOInterpolateImageFunctionBase);
  itkNewMacro(Self);
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::InputImageType      InputImageType;
  typedef typename Superclass::InputPixelType      InputPixelType;
  typedef typename Superclass::RealType            RealType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::IndexValueType      IndexValueType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::CoefContainerType   CoefContainerType;

  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& index) const override;

protected:
  BCOInterpolateImageFunction(){};
  ~BCOInterpolateImageFunction() override{};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  BCOInterpolateImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;
};


template <typename TPixel, unsigned int VImageDimension, class TCoordRep>
class ITK_EXPORT BCOInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>
  : public otb::BCOInterpolateImageFunctionBase<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>
{
public:
  /** Standard class typedefs.*/
  typedef BCOInterpolateImageFunction                                                           Self;
  typedef BCOInterpolateImageFunctionBase<otb::VectorImage<TPixel, VImageDimension>, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>                                                               Pointer;
  typedef itk::SmartPointer<const Self>                                                         ConstPointer;

  itkTypeMacro(BCOInterpolateImageFunction, BCOInterpolateImageFunctionBase);
  itkNewMacro(Self);
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  typedef typename Superclass::OutputType          OutputType;
  typedef typename Superclass::InputImageType      InputImageType;
  typedef typename Superclass::InputPixelType      InputPixelType;
  typedef typename Superclass::RealType            RealType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::IndexValueType      IndexValueType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::CoefContainerType   CoefContainerType;

  OutputType EvaluateAtContinuousIndex(const ContinuousIndexType& index) const override;

protected:
  BCOInterpolateImageFunction(){};
  ~BCOInterpolateImageFunction() override{};
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  BCOInterpolateImageFunction(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBCOInterpolateImageFunction.hxx"
#endif

#endif
