/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbGeographicalDistance_h
#define otbGeographicalDistance_h

#include "itkNumericTraits.h"
#include "itkDistanceMetric.h"

namespace otb
{
/**
 * \class GeographicalDistance
 * \brief Compute the metric distance between two geographical points
 *
 * Vector length must be at least two, otherwise the Evaluate method
 * will throw an exception.
 *
 * First dimension is assumed to be the longitude, and second
 * dimension is assumed to be the latitude.
 *
 * This distance uses the Haversine formula, which can be found here:
 * http://www.movable-type.co.uk/scripts/latlong.html
 *
 * Please note that the computation makes use of the earth radius in
 * meters. Mean radius (6371 km) is used as a default value, but one
 * might use local values from spheroid or geoid for better precision.
 *
 *
 * \ingroup OTBProjection
 */
template <class TVector>
class ITK_EXPORT GeographicalDistance :
    public itk::Statistics::DistanceMetric<TVector>
{
public:
  /** Standard class typedefs. */
  typedef GeographicalDistance                     Self;
  typedef itk::Statistics::DistanceMetric<TVector> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeographicalDistance, itk::Statistics::DistanceMetric);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Useful typedefs */
  typedef TVector                                  VectorType;

  /** Gets the distance between the origin point and x. This function
   *  work with SetOrigin() function */
  double Evaluate(const VectorType & x) const ITK_OVERRIDE;

  /* Gets the distance between x and y points */
  double Evaluate(const VectorType & x, const VectorType & y) const ITK_OVERRIDE;

  /** Set the earth radius */
  itkSetMacro(EarthRadius, double);

  /** Get the earth radius */
  itkGetMacro(EarthRadius, double);

protected:
  /** Constructor */
  GeographicalDistance();

  /** Destructor */
  ~GeographicalDistance() ITK_OVERRIDE{}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  GeographicalDistance(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  /** Earth radius */
  double m_EarthRadius;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbGeographicalDistance.txx"
#endif

#endif
