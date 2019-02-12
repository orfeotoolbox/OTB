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

#ifndef otbPointSetWithTransformToDisplacementFieldGenerator_h
#define otbPointSetWithTransformToDisplacementFieldGenerator_h

#include "otbPointSetToDisplacementFieldGenerator.h"
#include "itkTransform.h"

namespace otb
{
/** \class PointSetWithTransformToDisplacementFieldGenerator
 *  \brief Base class for filters generating a displacement field from a point set enriched with local transform information.
 *
 *  The output of the filters deriving from this base class can be passed to the itk::WarpImageFilter for image resampling.
 *
 *  \sa DisparityMapEstimationMethod
 *  \sa itk::WarpImageFilter
 *
 *  \ingroup DisparityMap
 *
 * \ingroup OTBDisplacementField
 */
template <class TPointSet, class TDisplacementField>
class ITK_EXPORT PointSetWithTransformToDisplacementFieldGenerator
  : public PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField>
{
public:
  /** Standard typedefs */
  typedef PointSetWithTransformToDisplacementFieldGenerator                  Self;
  typedef PointSetToDisplacementFieldGenerator<TPointSet, TDisplacementField> Superclass;
  typedef itk::SmartPointer<Self>                                           Pointer;
  typedef itk::SmartPointer<const Self>                                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(PointSetWithTransformToDisplacementFieldGenerator, PointSetToDisplacementFieldGenerator);

  /** Template parameters typedefs */
  typedef TPointSet                      PointSetType;
  typedef typename PointSetType::Pointer PointSetPointerType;

  typedef TDisplacementField                                DisplacementFieldType;
  typedef typename DisplacementFieldType::Pointer           DisplacementFieldPointerType;
  typedef typename DisplacementFieldType::IndexType         IndexType;
  typedef typename DisplacementFieldType::SizeType          SizeType;
  typedef typename DisplacementFieldType::SpacingType       SpacingType;
  typedef typename DisplacementFieldType::PointType         PointType;
  typedef typename DisplacementFieldType::InternalPixelType ValueType;

  /** More typedefs */
  typedef typename Superclass::DistanceVectorType DistanceVectorType;
  typedef typename Superclass::IndexVectorType    IndexVectorType;

  /** Transform typedefs */
  typedef itk::Transform<double, 2, 2>           TransformType;
  typedef typename TransformType::Pointer        TransformPointerType;
  typedef typename TransformType::ParametersType ParametersType;

  /** Set/Get the Transform. */
  itkSetObjectMacro(Transform, TransformType);
  itkGetObjectMacro(Transform, TransformType);

protected:
  /** Constructor */
  PointSetWithTransformToDisplacementFieldGenerator();
  /** Destructor */
  ~PointSetWithTransformToDisplacementFieldGenerator() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PointSetWithTransformToDisplacementFieldGenerator(const Self &) = delete;
  void operator =(const Self&) = delete;

  /**
   * The transform used for local registration.
   */
  TransformPointerType m_Transform;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetWithTransformToDisplacementFieldGenerator.hxx"
#endif

#endif
