/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbMapProjectionDeprecated_txx
#define otbMapProjectionDeprecated_txx

#include "otbMapProjectionDeprecated.h"

namespace otb
{

template<MapProjectionType::MapProj TMapProj>
inline std::string GetMapProjectionDeprecatedString()
{
  return "";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::ALBERS>()
{
  return "Albers";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::AZIMEQUDIST>()
{
  return "AzimEquDist";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::BNG>()
{
  return "Bng";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::BONNE>()
{
  return "Bonne";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::CADRG>()
{
  return "Cadrg";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::CASSINI>()
{
  return "Cassini";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::CYLEQUAREA>()
{
  return "CylEquArea";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::ECKERT6>()
{
  return "Eckert6";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::GNOMONIC>()
{
  return "Gnomonic";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::LLXY>()
{
  return "Llxy";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::EQUDISTCYL>()
{
  return "EquDistCyl";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::MERCATOR>()
{
  return "Mercator";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::MILLER>()
{
  return "Miller";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::NEWZEALANDMAPGRID>()
{
  return "NewZealandMapGrid";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::OBLIQUEMERCATOR>()
{
  return "ObliqueMercator";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::ORTHOGRAPHIC>()
{
  return "OrthoGraphic";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::POLARSTEREO>()
{
  return "PolarStereo";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::POLYCONIC>()
{
  return "Polyconic";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::SPACEOBLIQUEMERCATOR>()
{
  return "SpaceObliqueMercator";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::STEREOGRAPHIC>()
{
  return "Stereographic";
}

template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::TRANSCYLEQUAREA>()
{
  return "TransCylEquArea";
}


template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::UPS>()
{
  return "Ups";
}


template<>
inline std::string GetMapProjectionDeprecatedString<MapProjectionType::VANDERGRINTEN>()
{
  return "VanDerGrinten";
}


template <MapProjectionType::MapProj TMapProj,
          TransformDirection::TransformationDirection TTransform>
MapProjectionDeprecated<TMapProj, TTransform>
::MapProjectionDeprecated()
{
  this->SetWkt(GetMapProjectionDeprecatedString<TMapProj>());
}

}

#endif
