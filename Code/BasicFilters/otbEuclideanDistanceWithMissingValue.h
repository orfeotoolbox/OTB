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

#include "itkEuclideanDistance.h"

namespace otb {

namespace Statistics {

/** \class EuclideanDistanceWithMissingValue
 * \brief Euclidean distance function facing missing value.
 *
 * This class is derived from EuclideanDistance class. In addition
 * to the initial Evaluate method, the class does not perform calculation
 * when a component does not contain any data.
 *
 * The class can be templated over any container that holds data elements, as
 * for template of EuclideanDistance.
 *
 * The only restriction is that elemnts have to support \code NaN \endcode.
 *
 * \sa EuclideanDistance
 */
template< class TVector >
class ITK_EXPORT EuclideanDistanceWithMissingValue :
  public itk::Statistics::EuclideanDistance< TVector >
{
  public:
    /** Standard "Self" typedef. */
    typedef EuclideanDistanceWithMissingValue Self;
    typedef itk::Statistics::EuclideanDistance< TVector > Superclass;
    typedef itk::SmartPointer< Self > Pointer ;
    typedef itk::SmartPointer<const Self> ConstPointer;
    typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;

    /** Run-time type information (and related methods). */
    itkTypeMacro(EuclideanDistanceWithMissingValue, EuclideanDistance);

    /** Method for creation through the object factory. */
    itkNewMacro(Self) ;

    /** Type of the component of a vector */
    typedef typename TVector::ValueType ValueType ;

    /** Gets the distance between the origin and x */
    double Evaluate(const TVector &x) const ;

    /** Gets the distance between x1 and x2 */
    double Evaluate(const TVector &x1, const TVector &x2) const ;

    /** Gets the cooridnate distance between a and b. NOTE: a and b
    * should be type of component */
    double Evaluate(const ValueType &a, const ValueType &b) const ;

    /** Returns true if the distance between x and the origin is less
    * than radius */
    bool IsWithinRange(const TVector &x, const double radius) const {
      return Superclass::IsWithinRange( x, radius ); }

    /** Check if a value is NaN or not */
    static bool IsMissingValue ( const ValueType & v) ;

    /** Set a value to Nan */
    static void SetToMissingValue ( ValueType & v );

  protected:
    EuclideanDistanceWithMissingValue() {}
    virtual ~EuclideanDistanceWithMissingValue() {}
} ; // end of class

} // end namespace statistics

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbEuclideanDistanceWithMissingValue.txx"
#endif

#endif

