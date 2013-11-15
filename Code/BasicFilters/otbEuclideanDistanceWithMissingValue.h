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

#ifndef __otbEuclideanDistanceWithMissingValue_h
#define __otbEuclideanDistanceWithMissingValue_h

#include "otbEuclideanDistanceWithMissingValuePow2.h"

namespace otb {

namespace Statistics {

/** \class EuclideanDistanceWithMissingValue
 * \brief Euclidean distance function facing missing value.
 *
 * This class is derived from EuclideanDistanceWithMissingValuePow2 class that handle the missing value
 * functonnalities. Here, the square root is included in the evaluation...
 *
 * The class can be templated over any container that holds data elements, as
 * for template of EuclideanDistanceMetric.
 *
 * The only restriction is that elements have to support NaN values.
 *
 * \sa EuclideanDistanceMetric
 * \sa EuclideanDistanceWithMissingValuePow2
 */
template<class TVector>
class ITK_EXPORT EuclideanDistanceWithMissingValue :
  public otb::Statistics::EuclideanDistanceWithMissingValuePow2<TVector>
{
public:
  /** Standard "Self" typedef. */
  typedef EuclideanDistanceWithMissingValue Self;
  typedef otb::Statistics::EuclideanDistanceWithMissingValuePow2<TVector>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::MeasurementVectorSizeType
  MeasurementVectorSizeType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(EuclideanDistanceWithMissingValue, EuclideanDistanceWithMissingValuePow2);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Type of the component of a vector */
  typedef typename TVector::ValueType ValueType;

  /** Gets the distance between the origin and x */
  double Evaluate(const TVector& x) const
  {
    return ::vcl_sqrt(Superclass::Evaluate(x));
  }

  /** Gets the distance between x1 and x2 */
  double Evaluate(const TVector& x1, const TVector& x2) const
  {
    return ::vcl_sqrt(Superclass::Evaluate(x1, x2));
  }

  /** Gets the cooridnate distance between a and b. NOTE: a and b
  * should be type of component */
  double Evaluate(const ValueType& a, const ValueType& b) const
  {
    return ::vcl_sqrt(Superclass::Evaluate(a, b));
  }

  /** Returns true if the distance between x and the origin is less
  * than radius */
  bool IsWithinRange(const TVector& x, const double radius) const
  {
    return Superclass::IsWithinRange(x, radius);
  }

protected:
  EuclideanDistanceWithMissingValue() {}
  virtual ~EuclideanDistanceWithMissingValue() {}
}; // end of class

} // end namespace statistics

} // end namespace otb

#endif
