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

#ifndef otbBSplinesInterpolateTransformDisplacementFieldGenerator_h
#define otbBSplinesInterpolateTransformDisplacementFieldGenerator_h

#include "otbPointSetWithTransformToDisplacementFieldGenerator.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkBSplineScatteredDataPointSetToImageFilter.h"
#include "itkBSplineControlPointImageFunction.h"
#include "otbImage.h"
#include "otbMath.h"
#include <complex>

namespace otb
{
/** \class BSplinesInterpolateTransformDisplacementFieldGenerator
 *  \brief This class generate the displacement field by using spline interpolation on the parameters of the transform.
 *
 *  Spline interpolation of non regularly scattered data is provided
 *  by the itk::BSplineScatteredDataPointSetToImageFilter. It allows interpolation using any spline order and implements a multi-level approach.
 *
 *  This filter is used for each parameter. One can also specify the indices of the angular parameters. Angular parameters are first
 *  converted to complex exponential, the interpolated and converted back to the angular space. This is done to avoid interpolating angular discontinuities,
 *  which is a non-sense.
 *
 *  \sa itk::BSplineScatteredDataPointSetToImageFilter
 *
 * \ingroup OTBDisplacementField
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT BSplinesInterpolateTransformDisplacementFieldGenerator
  : public PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef BSplinesInterpolateTransformDisplacementFieldGenerator                          Self;
  typedef PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                                        Pointer;
  typedef itk::SmartPointer<const Self>                                                  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(BSplinesInterpolateTransformDisplacementFieldGenerator, PointSetWithTransformsToDisplacementFieldGenerator);

  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType                PointSetType;
  typedef typename Superclass::PointSetPointerType         PointSetPointerType;
  typedef typename Superclass::DisplacementFieldType        DisplacementFieldType;
  typedef typename Superclass::DisplacementFieldPointerType DisplacementFieldPointerType;
  typedef typename Superclass::IndexType                   IndexType;
  typedef typename DisplacementFieldType::PixelType         PixelType;
  typedef typename Superclass::ValueType                   ValueType;
  typedef typename Superclass::PointType                   PointType;
  typedef typename Superclass::IndexVectorType             IndexVectorType;
  typedef typename Superclass::DistanceVectorType          DistanceVectorType;
  typedef typename Superclass::ParametersType              ParametersType;

  /** Filters definition */
  typedef itk::Vector<ValueType, 2>                                          PointSetDataType;
  typedef otb::Image<PointSetDataType, DisplacementFieldType::ImageDimension> InternalImageType;
  typedef itk::PointSet<PointSetDataType, PointSetType::PointDimension>      InternalPointSetType;
  typedef itk::BSplineScatteredDataPointSetToImageFilter<InternalPointSetType, InternalImageType>
  SPlineInterpolateFilterType;
  typedef typename SPlineInterpolateFilterType::Pointer SPlineInterpolateFilterPointerType;
  typedef itk::BSplineControlPointImageFunction<InternalImageType>            BSplinerType;

  /**
   * Add an angular parameter. Angular parameters are transformed by complex exponential during interpolation so as to avoid discontinuity.
   * \param index The index of the angular parameter.
   */
  void AddAngularParameter(unsigned int index);
  /**
   * Remove an angular parameter.
   * \param index The index of the angular parameter.
   */
  void RemoveAngularParameter(unsigned int index);

  itkSetMacro(SplineOrder, unsigned int);
  itkGetMacro(SplineOrder, unsigned int);
  itkSetMacro(NumberOfControlPoints, unsigned int);
  itkGetMacro(NumberOfControlPoints, unsigned int);
  itkSetMacro(NumberOfLevels, unsigned int);
  itkGetMacro(NumberOfLevels, unsigned int);

protected:
  /** Constructor */
  BSplinesInterpolateTransformDisplacementFieldGenerator();
  /** Destructor */
  ~BSplinesInterpolateTransformDisplacementFieldGenerator() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Main computation method */
  void GenerateData() override;

  void GenerateOutputInformation() override;

  /**
   * Test whether a parameter is angular or not.
   * \param index The index of the parameter to test.
   */
  bool IsAngular(unsigned int index);

private:
  BSplinesInterpolateTransformDisplacementFieldGenerator(const Self &) = delete;
  void operator =(const Self&) = delete;

  IndexVectorType m_AngularParameters;

  unsigned int m_SplineOrder;
  unsigned int m_NumberOfControlPoints;
  unsigned int m_NumberOfLevels;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBSplinesInterpolateTransformDisplacementFieldGenerator.hxx"
#endif

#endif
