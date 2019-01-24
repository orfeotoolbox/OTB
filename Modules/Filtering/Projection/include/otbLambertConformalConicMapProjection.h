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

#ifndef otbLambertConformalConicMapProjection_h
#define otbLambertConformalConicMapProjection_h

#include "otbGenericMapProjection.h"

namespace otb
{
/** \class LambertConformalConicMapProjection
 * \brief This class implements the Lambert Conformal Conic map projection.
 * It converts coordinates in longitude, latitude (WGS84) to Lambert Conformal Conic map coordinates.
 *
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT LambertConformalConicMapProjection : public GenericMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef LambertConformalConicMapProjection  Self;
  typedef GenericMapProjection<TTransform>    Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LambertConformalConicMapProjection, GenericMapProjection);

  void   SetStandardParallel1(double degree);
  void   SetStandardParallel2(double degree);
  void   SetStandardParallels(double parallel1Degree, double parallel2Degree);
  void   SetFalseEasting(double falseEasting);
  void   SetFalseNorthing(double falseNorthing);
  double GetFalseNorthing() const;
  double GetFalseEasting() const;
  void  SetAllParameters(double parallel1Degree, double parallel2Degree,
                      double falseEasting, double falseNorthing);

protected:
  LambertConformalConicMapProjection();
  ~LambertConformalConicMapProjection() override {};

private:
  LambertConformalConicMapProjection(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLambertConformalConicMapProjection.hxx"
#endif

#endif
