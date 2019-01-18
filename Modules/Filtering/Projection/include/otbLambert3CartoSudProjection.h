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

#ifndef otbLambert3CartoSudProjection_h
#define otbLambert3CartoSudProjection_h

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{
/** \class Lambert3CartoSudProjection
* \brief This class implements the Lambert3CartoSud map projection (RGF93 french geodesic system).
* It converts coordinates in longitude, latitude (WGS84) to Lambert 3 map coordinates.
*
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT Lambert3CartoSudProjection : public LambertConformalConicMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef Lambert3CartoSudProjection                     Self;
  typedef LambertConformalConicMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Lambert3CartoSudProjection, LambertConformalConicMapProjection);

protected:
  Lambert3CartoSudProjection()
  {
    this->SetParameter("OriginX", "3");
    this->SetParameter("OriginY", "46.5");
    this->SetParameter("Datum", "WE");
    this->SetParameter("FalseNorthing", "3200000");
    this->SetParameter("FalseEasting", "600000");
    this->SetParameter("StandardParallel1", "43.1992913888888888888888888889");
    this->SetParameter("StandardParallel2", "44.9960938888888888888888888888");
  }

  virtual ~Lambert3CartoSudProjection() {}

private:
  Lambert3CartoSudProjection(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // namespace otb

#endif
