/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbFlexibleDistanceWithMissingValue_h
#define otbFlexibleDistanceWithMissingValue_h

#include "otbEuclideanDistanceMetricWithMissingValuePow2.h"

namespace otb {

namespace Statistics {

/** \class FlexibleDistanceWithMissingValue
 * \brief Non-Euclidean distance function facing missing value.
 *
 * This class is derived from EuclideanDistanceMetricWithMissingValuePow2 class to
 * deal with missing data. In addition, the initial Evaluate method performs
 * a flexible calculation of distance when a component does contain data.
 *
 * The general formulation is defined as:
 * \f$
    d(x, y) = \sum_i | x_i^a - y_i^a |^b.
  \f$
 *
 * The class can be templated over any container that holds data elements, as
 * for template of EuclideanDistanceMetric.
 *
 * The only restriction is that elements have to support \code NaN \endcode.
 *
 * \sa EuclideanDistanceMetricWithMissingValuePow2
 *
 * \ingroup OTBImageManipulation
 */
template<class TVector>
class ITK_EXPORT FlexibleDistanceWithMissingValue :
  public otb::Statistics::EuclideanDistanceMetricWithMissingValuePow2<TVector>
{
public:
  /** Standard "Self" typedef. */
  typedef FlexibleDistanceWithMissingValue                                Self;
  typedef otb::Statistics::EuclideanDistanceMetricWithMissingValuePow2<TVector> Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FlexibleDistanceWithMissingValue, EuclideanDistanceMetricWithMissingValuePow2);

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

  /** Set the value of the parameter a and b.
   * Those parameters are defined as static component in order to call it far
   * away from the instantiation of the class (through the SOM training for instance) */
  static double Alpha;
  static double Beta;
  static void SetAlphaBeta(double a, double b);

  /** test if it is ordinary Euclidean distance */
  static bool IsEuclidean();

protected:
  FlexibleDistanceWithMissingValue() {}
  ~FlexibleDistanceWithMissingValue() override {}
};  // end of class

/** Static variable instantiation */
template<class TVector>
double FlexibleDistanceWithMissingValue<TVector>::Alpha = 1.0;

template<class TVector>
double FlexibleDistanceWithMissingValue<TVector>::Beta = 2.0;

} // end namespace statistics

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlexibleDistanceWithMissingValue.hxx"
#endif

#endif
