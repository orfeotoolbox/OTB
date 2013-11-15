/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbFlexibleDistanceWithMissingValue_h
#define __otbFlexibleDistanceWithMissingValue_h

#include "otbEuclideanDistanceWithMissingValuePow2.h"

namespace otb {

namespace Statistics {

/** \class FlexibleDistanceWithMissingValue
 * \brief Non-Euclidean distance function facing missing value.
 *
 * This class is derived from EuclideanDistanceWithMissingValuePow2 class to
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
 * \sa EuclideanDistanceWithMissingValuePow2
 */
template<class TVector>
class ITK_EXPORT FlexibleDistanceWithMissingValue :
  public otb::Statistics::EuclideanDistanceWithMissingValuePow2<TVector>
{
public:
  /** Standard "Self" typedef. */
  typedef FlexibleDistanceWithMissingValue                                Self;
  typedef otb::Statistics::EuclideanDistanceWithMissingValuePow2<TVector> Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(FlexibleDistanceWithMissingValue, EuclideanDistanceWithMissingValuePow2);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Type of the component of a vector */
  typedef typename TVector::ValueType ValueType;

  /** Gets the distance between the origin and x */
  double Evaluate(const TVector& x) const;

  /** Gets the distance between x1 and x2 */
  double Evaluate(const TVector& x1, const TVector& x2) const;

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
   * away from the instanciation of the class (through the SOM training for instance) */
  static double Alpha;
  static double Beta;
  static void SetAlphaBeta(double a, double b);

  /** test if it is ordinary Euclidean distance */
  static bool IsEuclidean();

protected:
  FlexibleDistanceWithMissingValue() {}
  virtual ~FlexibleDistanceWithMissingValue() {}
};  // end of class

/** Static variable instanciation */
template<class TVector>
double FlexibleDistanceWithMissingValue<TVector>::Alpha = 1.0;

template<class TVector>
double FlexibleDistanceWithMissingValue<TVector>::Beta = 2.0;

} // end namespace statistics

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlexibleDistanceWithMissingValue.txx"
#endif

#endif
