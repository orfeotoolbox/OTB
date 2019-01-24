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

#ifndef otbLambert2EtenduProjection_h
#define otbLambert2EtenduProjection_h

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{
/** \class Lambert2EtenduProjection
* \brief This class implements the Lambert 2 Etendu map projection.
* It converts coordinates in longitude, latitude (WGS84) to Lambert 2 Etendu map coordinates.
*
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT Lambert2EtenduProjection : public LambertConformalConicMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef Lambert2EtenduProjection                       Self;
  typedef LambertConformalConicMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Lambert2EtenduProjection, LambertConformalConicMapProjection);

protected:
  Lambert2EtenduProjection()
  {
    this->SetParameter("OriginX", "2.33722778");
    this->SetParameter("OriginY", "46.8");
    this->SetParameter("Datum", "NTF");
    this->SetParameter("FalseNorthing", "2200000");
    this->SetParameter("FalseEasting", "600000");
    this->SetParameter("StandardParallel1", "45.89891944");
    this->SetParameter("StandardParallel2", "47.69601389");
  }

  ~Lambert2EtenduProjection() override {}

private:
  Lambert2EtenduProjection(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // namespace otb

#endif
