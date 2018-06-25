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


#ifndef otbEuclideanDistanceMetricWithMissingValuePow2_h
#define otbEuclideanDistanceMetricWithMissingValuePow2_h

#include "itkEuclideanDistanceMetric.h"

namespace otb {

namespace Statistics {

/** \class EuclideanDistanceMetricWithMissingValuePow2
 * \brief Euclidean comparison distance function facing missing value. The square root is not performed in this class.
 *
 * This class is derived from EuclideanDistanceMetric class. In addition
 * to the initial Evaluate method, the class does not perform calculation
 * when a component does not contain any data. The 'no data' case is perform through the \code SetToMissingValue() \endcode
 * which performs a \code Nan \endcode affectation.
 *
 * The class can be templated over any container that holds data elements, as
 * for template of EuclideanDistanceMetric.
 *
 * The only restriction is that elements have to support NaN values.
 *
 * \sa EuclideanDistanceMetric
 * \sa EuclideanDistanceMetricWithMissingValue
 *
 * \ingroup OTBImageManipulation
 */
template<class TVector>
class ITK_EXPORT EuclideanDistanceMetricWithMissingValuePow2 :
  public itk::Statistics::EuclideanDistanceMetric<TVector>
{
public:
  /** Standard "Self" typedef. */
  typedef EuclideanDistanceMetricWithMissingValuePow2          Self;
  typedef itk::Statistics::EuclideanDistanceMetric<TVector>    Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;
  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(EuclideanDistanceMetricWithMissingValuePow2, EuclideanDistanceMetric);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Type of the component of a vector */
  typedef typename TVector::ValueType ValueType;

  /** Gets the distance between the origin and x */
  double Evaluate(const TVector& x) const override;

  /** Gets the distance between x1 and x2 */
  double Evaluate(const TVector& x1, const TVector& x2) const override;

  /** Gets the cooridnate distance between a and b. NOTE: a and b
  * should be type of component */
  double Evaluate(const ValueType& a, const ValueType& b) const;

  /** Returns true if the distance between x and the origin is less
  * than radius */
  bool IsWithinRange(const TVector& x, const double radius) const
  {
    return Superclass::IsWithinRange(x, radius);
  }

  /** Check if a value is NaN or not */
  static bool IsMissingValue(const ValueType& v);

  /** Set a value to Nan */
  static void SetToMissingValue(ValueType& v);

protected:
  EuclideanDistanceMetricWithMissingValuePow2() {}
  ~EuclideanDistanceMetricWithMissingValuePow2() override {}
}; // end of class

} // end namespace statistics

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEuclideanDistanceMetricWithMissingValuePow2.hxx"
#endif

#endif
