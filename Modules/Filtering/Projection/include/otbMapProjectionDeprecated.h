/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMapProjectionDeprecated_h
#define otbMapProjectionDeprecated_h

#include "otbGenericMapProjection.h"


namespace otb
{

namespace MapProjectionType
{
enum MapProj
{
  ALBERS,
  AZIMEQUDIST,
  BNG,
  BONNE,
  CADRG,
  CASSINI,
  CYLEQUAREA,
  ECKERT6,
  GNOMONIC,
  LLXY,
  EQUDISTCYL,
  MERCATOR,
  MILLER,
  NEWZEALANDMAPGRID,
  OBLIQUEMERCATOR,
  ORTHOGRAPHIC,
  POLARSTEREO,
  POLYCONIC,
  SPACEOBLIQUEMERCATOR,
  STEREOGRAPHIC,
  TRANSCYLEQUAREA,
  UPS,
  VANDERGRINTEN,
};
}

/** \class MapProjectionDeprecated
 *  \brief The only purpose of this class is to maintain backward compatibility.
 *
 *  GenericMapProjection should be used instead.
 *
 *
 *
 * \ingroup OTBProjection
 */
template <MapProjectionType::MapProj TMapProj,
          TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT MapProjectionDeprecated : public GenericMapProjection<TTransform>
{
public:

  /** Standard class typedefs. */
  typedef MapProjectionDeprecated           Self;
  typedef GenericMapProjection<TTransform>  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MapProjectionDeprecated, GenericMapProjection);

protected:
  MapProjectionDeprecated();
  ~MapProjectionDeprecated() ITK_OVERRIDE {};

private:
  MapProjectionDeprecated(const Self &); //purposely not implemented
  void operator =(const Self&);   //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMapProjectionDeprecated.txx"
#endif

#endif
