/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbEuclideanDistanceMetricWithMissingValue_h
#define otbEuclideanDistanceMetricWithMissingValue_h

#include "otbEuclideanDistanceMetricWithMissingValuePow2.h"

namespace otb {

namespace Statistics {

/** \class EuclideanDistanceMetricWithMissingValue
 * \brief Euclidean distance function facing missing value.
 *
 * This class is derived from EuclideanDistanceMetricWithMissingValuePow2 class that handle the missing value
 * functonnalities. Here, the square root is included in the evaluation...
 *
 * The class can be templated over any container that holds data elements, as
 * for template of EuclideanDistanceMetric.
 *
 * The only restriction is that elements have to support NaN values.
 *
 * \sa EuclideanDistanceMetric
 * \sa EuclideanDistanceMetricWithMissingValuePow2
 *
 * \ingroup OTBImageManipulation
 */
template<class TVector>
class ITK_EXPORT EuclideanDistanceMetricWithMissingValue :
  public otb::Statistics::EuclideanDistanceMetricWithMissingValuePow2<TVector>
{
public:
  /** Standard "Self" typedef. */
  typedef EuclideanDistanceMetricWithMissingValue Self;
  typedef otb::Statistics::EuclideanDistanceMetricWithMissingValuePow2<TVector>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::MeasurementVectorSizeType
  MeasurementVectorSizeType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(EuclideanDistanceMetricWithMissingValue, EuclideanDistanceMetricWithMissingValuePow2);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Type of the component of a vector */
  typedef typename TVector::ValueType ValueType;

  /** Gets the distance between the origin and x */
  double Evaluate(const TVector& x) const override
  {
    return ::std::sqrt(Superclass::Evaluate(x));
  }

  /** Gets the distance between x1 and x2 */
  double Evaluate(const TVector& x1, const TVector& x2) const override
  {
    return ::std::sqrt(Superclass::Evaluate(x1, x2));
  }

  /** Gets the cooridnate distance between a and b. NOTE: a and b
  * should be type of component */
  double Evaluate(const ValueType& a, const ValueType& b) const
  {
    return ::std::sqrt(Superclass::Evaluate(a, b));
  }

  /** Returns true if the distance between x and the origin is less
  * than radius */
  bool IsWithinRange(const TVector& x, const double radius) const
  {
    return Superclass::IsWithinRange(x, radius);
  }

protected:
  EuclideanDistanceMetricWithMissingValue() {}
  ~EuclideanDistanceMetricWithMissingValue() override {}
}; // end of class

} // end namespace statistics

} // end namespace otb

#endif
