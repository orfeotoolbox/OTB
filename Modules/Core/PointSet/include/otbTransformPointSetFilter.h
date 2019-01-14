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

#ifndef otbTransformPointSetFilter_h
#define otbTransformPointSetFilter_h

#include "otbPointSetToPointSetFilter.h"

namespace otb
{

/** \class TransformPointSetFilter
 * \brief TransformPointSetFilter applies a transform to all the
 * points of a PointSet.
 *
 * The additional content of the PointSet is left untouched.
 *
 * PointSets that have added information like normal vector on the points, will
 * have to take care of transforming this data by other means.
 *
 * \ingroup PointSetFilters
 *
 * \ingroup OTBPointSet
 */
template <class TInputPointSet, class TOutputPointSet, class TTransform>
class ITK_EXPORT TransformPointSetFilter :
  public PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef TransformPointSetFilter                                   Self;
  typedef PointSetToPointSetFilter<TInputPointSet, TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  typedef TInputPointSet                       InputPointSetType;
  typedef TOutputPointSet                      OutputPointSetType;
  typedef typename InputPointSetType::Pointer  InputPointSetPointer;
  typedef typename OutputPointSetType::Pointer OutputPointSetPointer;

  /** Type for representing coordinates. */
  typedef typename TInputPointSet::CoordRepType CoordRepType;

  /** Type of the transform. */
  typedef TTransform TransformType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TransformPointSetFilter, PointSetToPointSetFilter);

  /** Set transform. */
  itkSetObjectMacro(Transform, TransformType);

  /** Get transform. */
  itkGetObjectMacro(Transform, TransformType);

protected:
  TransformPointSetFilter();
  ~TransformPointSetFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Generate Requested Data */
  void GenerateData(void) override;

  /** Transform to apply to all the PointSet points. */
  typename TransformType::Pointer m_Transform;

private:
  TransformPointSetFilter(const TransformPointSetFilter &) = delete;
  void operator =(const TransformPointSetFilter&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTransformPointSetFilter.hxx"
#endif

#endif
