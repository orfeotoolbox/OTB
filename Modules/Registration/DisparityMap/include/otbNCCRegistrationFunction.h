/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbNCCRegistrationFunction_h
#define otbNCCRegistrationFunction_h

#include "itkPDEDeformableRegistrationFunction.h"
#include "itkPoint.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkCentralDifferenceImageFunction.h"

namespace otb
{

/**
 * \class NCCRegistrationFunction
 *
 * This class has been copied from its itk version to provide a quick fix of itk bug #8288.
 * It is intended to be removed once the bug has been corrected in ITK.
 *
 * For more information, please refer to the ITK original class.
 *
 * \todo Check if the bug has been corrected in the next ITK version.
 *
 * \sa itk::NCCRegistrationFunction
 * \ingroup FiniteDifferenceFunctions
 *
 * \ingroup OTBDisparityMap
 */
template <class TFixedImage, class TMovingImage, class TDisplacementField>
class ITK_EXPORT NCCRegistrationFunction : public itk::PDEDeformableRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField>
{
public:
  /** Standard class typedefs. */
  typedef NCCRegistrationFunction Self;
  typedef itk::PDEDeformableRegistrationFunction<TFixedImage, TMovingImage, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(NCCRegistrationFunction, PDEDeformableRegistrationFunction);

  /** MovingImage image type. */
  typedef typename Superclass::MovingImageType    MovingImageType;
  typedef typename Superclass::MovingImagePointer MovingImagePointer;

  /** FixedImage image type. */
  typedef typename Superclass::FixedImageType    FixedImageType;
  typedef typename Superclass::FixedImagePointer FixedImagePointer;
  typedef typename FixedImageType::IndexType     IndexType;
  typedef typename FixedImageType::SizeType      SizeType;
  typedef typename FixedImageType::SpacingType   SpacingType;

  /** Displacement field type. */
  typedef typename Superclass::DisplacementFieldType        DisplacementFieldType;
  typedef typename Superclass::DisplacementFieldTypePointer DisplacementFieldTypePointer;

  /** Inherit some enums from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Inherit some enums from the superclass. */
  typedef typename Superclass::PixelType        PixelType;
  typedef typename Superclass::RadiusType       RadiusType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  //  typedef typename Superclass::NeighborhoodType    BoundaryNeighborhoodType;
  typedef typename Superclass::FloatOffsetType FloatOffsetType;
  typedef typename Superclass::TimeStepType    TimeStepType;

  /** Interpolator type. */
  typedef double CoordRepType;
  typedef itk::InterpolateImageFunction<MovingImageType, CoordRepType> InterpolatorType;
  typedef typename InterpolatorType::Pointer   InterpolatorPointer;
  typedef typename InterpolatorType::PointType PointType;
  typedef itk::LinearInterpolateImageFunction<MovingImageType, CoordRepType> DefaultInterpolatorType;

  /** Covariant vector type. */
  typedef itk::CovariantVector<double, itkGetStaticConstMacro(ImageDimension)> CovariantVectorType;

  /** Gradient calculator type. */
  typedef itk::CentralDifferenceImageFunction<FixedImageType> GradientCalculatorType;
  typedef typename GradientCalculatorType::Pointer            GradientCalculatorPointer;

  /** Set the moving image interpolator. */
  void SetMovingImageInterpolator(InterpolatorType* ptr)
  {
    m_MovingImageInterpolator = ptr;
  }

  /** Get the moving image interpolator. */
  InterpolatorType* GetMovingImageInterpolator(void)
  {
    return m_MovingImageInterpolator;
  }

  /** This class uses a constant timestep of 1. */
  TimeStepType ComputeGlobalTimeStep(void* itkNotUsed(GlobalData)) const override
  {
    return m_TimeStep;
  }

  /** Return a pointer to a global data structure that is passed to
   * this object from the solver at each calculation.  */
  void* GetGlobalDataPointer() const override
  {
    GlobalDataStruct* global = new GlobalDataStruct();
    return global;
  }

  /** Release memory for global data structure. */
  void ReleaseGlobalDataPointer(void* GlobalData) const override
  {
    delete (GlobalDataStruct*)GlobalData;
  }

  /** Set the object's state before each iteration. */
  void InitializeIteration() override;

  /** This method is called by a finite difference solver image filter at
   * each pixel that does not lie on a data set boundary */
  PixelType ComputeUpdate(const NeighborhoodType& neighborhood, void* globalData, const FloatOffsetType& offset = FloatOffsetType(0.0)) override;

protected:
  NCCRegistrationFunction();
  ~NCCRegistrationFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** FixedImage image neighborhood iterator type. */
  typedef itk::ConstNeighborhoodIterator<FixedImageType> FixedImageNeighborhoodIteratorType;

  /** A global data type for this class of equation. Used to store
   * iterators for the fixed image. */
  struct GlobalDataStruct
  {
    FixedImageNeighborhoodIteratorType m_FixedImageIterator;
  };

private:
  NCCRegistrationFunction(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Cache fixed image information. */
  SpacingType m_FixedImageSpacing;
  PointType   m_FixedImageOrigin;

  /** Function to compute derivatives of the fixed image. */
  GradientCalculatorPointer m_FixedImageGradientCalculator;

  /** Function to interpolate the moving image. */
  InterpolatorPointer m_MovingImageInterpolator;

  /** The global timestep. */
  TimeStepType m_TimeStep;

  /** Threshold below which the denominator term is considered zero. */
  double m_DenominatorThreshold;

  /** Threshold below which two intensity value are assumed to match. */
  double m_IntensityDifferenceThreshold;

  mutable double m_MetricTotal;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNCCRegistrationFunction.hxx"
#endif

#endif
