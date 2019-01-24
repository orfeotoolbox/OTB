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

#ifndef otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator_h
#define otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator_h

#include "otbPointSetWithTransformToDisplacementFieldGenerator.h"

namespace otb
{
/** \class NNearestTransformsLinearInterpolateDisplacementFieldGenerator
 *  \brief generate the displacement field by performing a linear interpolation
 *
 * This class generate the displacement field by performing a linear interpolation of
 * the displacements from the transforms induced by the n nearest points.
 *
 * \ingroup DisparityMap
 *
 * \ingroup OTBDisplacementField
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT NNearestTransformsLinearInterpolateDisplacementFieldGenerator
  : public PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef NNearestTransformsLinearInterpolateDisplacementFieldGenerator                   Self;
  typedef PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                                        Pointer;
  typedef itk::SmartPointer<const Self>                                                  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NNearestTransformsLinearInterpolateDisplacementFieldGenerator,
               PointSetWithTransformsToDisplacementFieldGenerator);

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
  typedef typename Superclass::TransformType               TransformType;
  typedef typename TransformType::ParametersType           ParametersType;

  itkSetMacro(NumberOfPoints, unsigned int);
  itkGetMacro(NumberOfPoints, unsigned int);

protected:
  /** Constructor */
  NNearestTransformsLinearInterpolateDisplacementFieldGenerator() {};
  /** Destructor */
  ~NNearestTransformsLinearInterpolateDisplacementFieldGenerator() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Main computation method */
  void GenerateData() override;

private:
  NNearestTransformsLinearInterpolateDisplacementFieldGenerator(const Self &) = delete;
  void operator =(const Self&) = delete;

  unsigned int m_NumberOfPoints;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNNearestTransformsLinearInterpolateDisplacementFieldGenerator.hxx"
#endif

#endif
