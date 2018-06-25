/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbBSplineInterpolateImageFunction_h
#define otbBSplineInterpolateImageFunction_h

#include <vector>

#include "itkInterpolateImageFunction.h"
#include "vnl/vnl_matrix.h"

#include "otbBSplineDecompositionImageFilter.h"
#include "itkConceptChecking.h"
#include "itkCovariantVector.h"

namespace otb
{
/** \class BSplineInterpolateImageFunction
 * \brief This class is an evolution of the itk::BSplineInterpolateImageFunction to handle
 * huge images with this interpolator. For more documentation, please refer to the original
 * class.
 * \sa itk::BSplineInterpolateImageFunction
 * \sa itk::BSplineDecompositionImageFilter
 * \sa otb::BSplineDecompositionImageFilter
 *
 * \ingroup ImageFunctions
 *
 * \ingroup OTBInterpolation
 */
template <
    class TImageType,
    class TCoordRep = double,
    class TCoefficientType = double>
class ITK_EXPORT BSplineInterpolateImageFunction :
  public itk::InterpolateImageFunction<TImageType, TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef BSplineInterpolateImageFunction                      Self;
  typedef itk::InterpolateImageFunction<TImageType, TCoordRep> Superclass;
  typedef itk::SmartPointer<Self>                              Pointer;
  typedef itk::SmartPointer<const Self>                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSplineInterpolateImageFunction, InterpolateImageFunction);

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro(Self);

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** InputImageType typedef support. */
  typedef typename Superclass::InputImageType InputImageType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;

  /** Region typedef support */
  typedef typename InputImageType::RegionType RegionType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** PointType typedef support */
  typedef typename Superclass::PointType PointType;

  /** Iterator typedef support */
  typedef itk::ImageLinearIteratorWithIndex<TImageType> Iterator;

  /** Internal Coefficient typedef support */
  typedef TCoefficientType CoefficientDataType;
  typedef itk::Image<CoefficientDataType,
      itkGetStaticConstMacro(ImageDimension)
      > CoefficientImageType;

  /** Define filter for calculating the BSpline coefficients */
  typedef otb::BSplineDecompositionImageFilter<TImageType, CoefficientImageType>
  CoefficientFilter;
  typedef typename CoefficientFilter::Pointer CoefficientFilterPointer;

  /** Evaluate the function at a ContinuousIndex position.
   *
   * Returns the B-Spline interpolated image intensity at a
   * specified point position. No bounds checking is done.
   * The point is assume to lie within the image buffer.
   *
   * ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& index) const override;

  /** Derivative typedef support */
  typedef itk::CovariantVector<OutputType,
      itkGetStaticConstMacro(ImageDimension)
      > CovariantVectorType;

  CovariantVectorType EvaluateDerivative(const PointType& point) const
  {
    ContinuousIndexType index;
    this->GetInputImage()->TransformPhysicalPointToContinuousIndex(point, index);
    return (this->EvaluateDerivativeAtContinuousIndex(index));
  }

  CovariantVectorType EvaluateDerivativeAtContinuousIndex(
    const ContinuousIndexType& x) const;

  /** Get/Sets the Spline Order, supports 0th - 5th order splines. The default
   *  is a 3rd order spline. */
  void SetSplineOrder(unsigned int SplineOrder);
  itkGetMacro(SplineOrder, int);

  /** Set the input image.  This must be set by the user. */
  void SetInputImage(const TImageType * inputData) override;

  /** Update coefficients filter. Coefficient filter are computed over the buffered
   region of the input image. */
  virtual void UpdateCoefficientsFilter(void);

protected:
  BSplineInterpolateImageFunction();
  ~BSplineInterpolateImageFunction() override {}
  void operator =(const Self&) = delete;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  // These are needed by the smoothing spline routine.
  std::vector<CoefficientDataType> m_Scratch;           // temp storage for processing of Coefficients
  typename TImageType::SizeType m_DataLength;        // Image size
  unsigned int m_SplineOrder;                        // User specified spline order (3rd or cubic is the default)

  typename CoefficientImageType::ConstPointer m_Coefficients;       // Spline coefficients

private:
  BSplineInterpolateImageFunction(const Self &) = delete;
  /** Determines the weights for interpolation of the value x */
  void SetInterpolationWeights(const ContinuousIndexType& x,
                               const vnl_matrix<long>& EvaluateIndex,
                               vnl_matrix<double>& weights,
                               unsigned int splineOrder) const;

  /** Determines the weights for the derivative portion of the value x */
  void SetDerivativeWeights(const ContinuousIndexType& x,
                            const vnl_matrix<long>& EvaluateIndex,
                            vnl_matrix<double>& weights,
                            unsigned int splineOrder) const;

  /** Precomputation for converting the 1D index of the interpolation neighborhood
    * to an N-dimensional index. */
  void GeneratePointsToIndex();

  /** Determines the indices to use give the splines region of support */
  void DetermineRegionOfSupport(vnl_matrix<long>& evaluateIndex,
                                const ContinuousIndexType& x,
                                unsigned int splineOrder) const;

  /** Set the indices in evaluateIndex at the boundaries based on mirror
    * boundary conditions. */
  void ApplyMirrorBoundaryConditions(vnl_matrix<long>& evaluateIndex,
                                     unsigned int splineOrder) const;

  Iterator               m_CIterator;       // Iterator for traversing spline coefficients.
  unsigned long          m_MaxNumberInterpolationPoints;    // number of neighborhood points used for interpolation
  std::vector<IndexType> m_PointsToIndex;     // Preallocation of interpolation neighborhood indices

  CoefficientFilterPointer m_CoefficientFilter;

  RegionType m_CurrentBufferedRegion;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBSplineInterpolateImageFunction.hxx"
#endif

#endif
