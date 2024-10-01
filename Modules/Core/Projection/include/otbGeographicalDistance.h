/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
class ITK_EXPORT GeographicalDistance : public itk::Statistics::DistanceMetric<TVector>
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
  typedef TVector VectorType;

  /** Gets the distance between the origin point and x. This function
   *  work with SetOrigin() function */
  double Evaluate(const VectorType& x) const override;

  /* Gets the distance between x and y points */
  double Evaluate(const VectorType& x, const VectorType& y) const override;

  /** Set the earth radius */
  itkSetMacro(EarthRadius, double);

  /** Get the earth radius */
  itkGetMacro(EarthRadius, double);

protected:
  /** Constructor */
  GeographicalDistance();

  /** Destructor */
  ~GeographicalDistance() override
  {
  }

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GeographicalDistance(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Earth radius */
  double m_EarthRadius;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeographicalDistance.hxx"
#endif

#endif
