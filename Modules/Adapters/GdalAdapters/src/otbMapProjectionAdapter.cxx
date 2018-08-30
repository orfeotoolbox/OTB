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


#include "otbMapProjectionAdapter.h"

#include <cassert>

#include "otbMacro.h"
#include "otbUtils.h"

#include "ogr_spatialref.h"
#include "ogr_srs_api.h"

namespace otb
{

MapProjectionAdapter::MapProjectionAdapter():
  m_ForwardMapProjection(nullptr), m_InverseMapProjection(nullptr)
{}

MapProjectionAdapter::~MapProjectionAdapter()
{}

bool MapProjectionAdapter::IsValid() const
{
  return m_ForwardMapProjection != nullptr && m_InverseMapProjection != nullptr;
}

std::string MapProjectionAdapter::GetWkt() const
{
  if (m_ForwardMapProjection == nullptr)
    {
    return "";
    }
  OGRSpatialReference * srs = m_ForwardMapProjection->GetTargetCS();
  
  assert(srs!=nullptr);
  
  char * cwkt;
  srs->exportToWkt(&cwkt);
  std::string wkt(cwkt);
  delete cwkt;
  
  return wkt;
}

void MapProjectionAdapter::SetWkt(const std::string& projectionRefWkt)
{
  bool success = InstantiateProjectionFromWkt(projectionRefWkt);

  if(!success)
    {
    itkExceptionMacro(<<"Could not instanciate map projection with following Wkt string: "<<projectionRefWkt);
    }
  
  this->Modified();
}

bool MapProjectionAdapter::InstantiateProjectionFromWkt(const std::string & wkt)
{
  // Create spatial references
  std::unique_ptr<OGRSpatialReference> wgs84_srs(OGRSpatialReference::GetWGS84SRS());
  std::unique_ptr<OGRSpatialReference> wkt_srs(new OGRSpatialReference());

  // This is ugly but importFromWkt seems to be const incorrect
  char * c_str = const_cast<char *>(wkt.c_str());
  OGRErr code = wkt_srs->importFromWkt(&c_str);

  if(code != OGRERR_NONE)
    return false;
  
  // Create forward and inverse map projections
  m_ForwardMapProjection = std::unique_ptr<OGRCoordinateTransformation>(OGRCreateCoordinateTransformation(wgs84_srs.get(),wkt_srs.get()));
  m_InverseMapProjection =  std::unique_ptr<OGRCoordinateTransformation>(OGRCreateCoordinateTransformation(wkt_srs.get(),wgs84_srs.get()));

  if(m_ForwardMapProjection == nullptr || m_InverseMapProjection == nullptr)
    return false;
  else
    return true;
}

void MapProjectionAdapter::InverseTransform(double x, double y, double z,
                                       double& lon, double& lat, double& h)
{
  if (m_InverseMapProjection == nullptr)
    itkExceptionMacro(<< "MapProjection has not been instanciated");

  lon=x;
  lat=y;
  h=z;

  int ret = m_InverseMapProjection->Transform(1,&lon,&lat);

  // assert or throw ?
  assert(ret==0);
}

void MapProjectionAdapter::ForwardTransform(double lon, double lat, double h,
                                       double& x, double& y, double& z)
{
  if (m_ForwardMapProjection == nullptr)
    itkExceptionMacro(<< "MapProjection has not been instanciated");
  
  lon=x;
  lat=y;
  h=z;

  int ret = m_ForwardMapProjection->Transform(1,&lon,&lat);

  // assert or throw ?
  assert(ret==0);
}

// void MapProjectionAdapter::ApplyParametersToProjection()
// {
//   // Start by identifying the projection, that will be necessary for
//   // the casting.
//   std::string projectionName = this->GetMapProjection()->getClassName().string();

//   StoreType::const_iterator it;

//   // Apply standard map projection parameters
//   ossimMapProjection* projection = dynamic_cast<ossimMapProjection*>(this->GetMapProjection());
//   // Set up origin

//   const ossimDatum* datum = ossimDatumFactory::instance()->wgs84(); //default value
//   it = m_ParameterStore.find("Datum");
//   if (it != m_ParameterStore.end())
//     {
//     datum = ossimDatumFactory::instance()->create((*it).second);
//     projection->setDatum(datum);
//     }

//   StoreType::const_iterator itX = m_ParameterStore.find("OriginX");
//   StoreType::const_iterator itY = m_ParameterStore.find("OriginY");
//   StoreType::const_iterator itZ = m_ParameterStore.find("OriginZ");

//   if (itX != m_ParameterStore.end() && itY != m_ParameterStore.end())
//     {
//     double originX = atof((*itX).second.c_str());
//     double originY = atof((*itY).second.c_str());
//     double originZ = 0;
//     if (itZ != m_ParameterStore.end())
//       {
//       originZ = atof((*itZ).second.c_str());
//       }
//     ossimGpt origin(originY, originX, originZ, datum);
//     projection->setOrigin(origin);
//     }

//   // Set up resolution
//   StoreType::const_iterator itResMeterX = m_ParameterStore.find("MetersPerPixelX");
//   StoreType::const_iterator itResMeterY = m_ParameterStore.find("MetersPerPixelY");
//   if (itResMeterX != m_ParameterStore.end() && itResMeterY != m_ParameterStore.end())
//     {
//     double resMeterX = atof((*itResMeterX).second.c_str());
//     double resMeterY = atof((*itResMeterY).second.c_str());
//     ossimDpt resMeter(resMeterX, resMeterY);
//     projection->setMetersPerPixel(resMeter);
//     }

//   // Apply parameters to LambertConformalConic
//   if (projectionName.compare("ossimLambertConformalConicProjection") == 0)
//     {
//     ossimLambertConformalConicProjection* lccProjection = dynamic_cast<ossimLambertConformalConicProjection*>(this->GetMapProjection());

//     it = m_ParameterStore.find("FalseNorthing");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());

//       lccProjection->setFalseNorthing(value);
//       }
//     it = m_ParameterStore.find("FalseEasting");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());

//       lccProjection->setFalseEasting(value);
//       }
//     it = m_ParameterStore.find("StandardParallel1");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       lccProjection->setStandardParallel1(value);
//       }
//     it = m_ParameterStore.find("StandardParallel2");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       lccProjection->setStandardParallel2(value);
//       }
//     }

//   // Apply parameters to Eckert4
//   if (projectionName.compare("ossimEckert4Projection") == 0)
//     {
//     ossimEckert4Projection* e4Projection = dynamic_cast<ossimEckert4Projection*>(this->GetMapProjection());

//     it = m_ParameterStore.find("FalseNorthing");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       e4Projection->setFalseNorthing(value);
//       }
//     it = m_ParameterStore.find("FalseEasting");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       e4Projection->setFalseEasting(value);
//       }
//     }

//   // Apply parameters to Mollweid
//   if (projectionName.compare("ossimMollweidProjection") == 0)
//     {
//     ossimMollweidProjection* mProjection = dynamic_cast<ossimMollweidProjection*>(this->GetMapProjection());

//     it = m_ParameterStore.find("FalseNorthing");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       mProjection->setFalseNorthing(value);
//       }
//     it = m_ParameterStore.find("FalseEasting");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       mProjection->setFalseEasting(value);
//       }
//     }

//   // Apply parameters to Sinusoidal
//   if (projectionName.compare("ossimSinusoidalProjection") == 0)
//     {
//     ossimSinusoidalProjection* sProjection = dynamic_cast<ossimSinusoidalProjection*>(this->GetMapProjection());

//     it = m_ParameterStore.find("FalseNorthing");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       sProjection->setFalseNorthing(value);
//       }
//     it = m_ParameterStore.find("FalseEasting");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       sProjection->setFalseEasting(value);
//       }
//     }

//   // Apply parameters to transmercator
//   if (projectionName.compare("ossimTransMercatorProjection") == 0)
//     {
//     ossimTransMercatorProjection* tmProjection = dynamic_cast<ossimTransMercatorProjection*> (this->GetMapProjection());
//     it = m_ParameterStore.find("ScaleFactor");
//     if (it != m_ParameterStore.end())
//       {
//       double scale = atof((*it).second.c_str());
//       tmProjection->setScaleFactor(scale);
//       }
//     it = m_ParameterStore.find("FalseNorthing");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       tmProjection->setFalseNorthing(value);
//       }
//     it = m_ParameterStore.find("FalseEasting");
//     if (it != m_ParameterStore.end())
//       {
//       double value = atof((*it).second.c_str());
//       tmProjection->setFalseEasting(value);
//       }
//     }

//   // Apply parameters to Utm
//   if (projectionName.compare("ossimUtmProjection") == 0)
//     {
//     ossimUtmProjection* utmProjection = dynamic_cast<ossimUtmProjection*>(this->GetMapProjection());
//     it = m_ParameterStore.find("Zone");
//     if (it != m_ParameterStore.end())
//       {
//       int zone = atoi((*it).second.c_str());
//       utmProjection->setZone(zone);
//       }
//     it = m_ParameterStore.find("Hemisphere");
//     if (it != m_ParameterStore.end())
//       {
//       utmProjection->setHemisphere((*it).second[0]);
//       }
//     }
// }

void MapProjectionAdapter::PrintMap() const
{
  otbLogMacro(Debug,<<"Map projection WKT: "<<GetWkt());
}

namespace Utils
{

int GetZoneFromGeoPoint(double lon, double lat)
{
  // TODO: Code forked from ossimUtmProjection, add proper copyright notice

  unsigned int result = 0;
  int lat_Degrees  = static_cast<int>(lat + 0.00000005);
  int long_Degrees = static_cast<int>(lon + 0.00000005);
  
  if (lon < M_PI)
    result = static_cast<unsigned int>((31 + ((180 * lon) / (6 * M_PI)) ) + 0.00000005);
  else
    result = static_cast<unsigned int>( (((180 * lon) / (6 * M_PI)) - 29) + 0.00000005);
  if (result > 60)
    result = 1;
    /* UTM special cases */
   if ((lat_Degrees > 55) && (lat_Degrees < 64) && (long_Degrees > -1)
       && (long_Degrees < 3))
      result = 31;
   if ((lat_Degrees > 55) && (lat_Degrees < 64) && (long_Degrees > 2)
       && (long_Degrees < 12))
      result = 32;
   if ((lat_Degrees > 71) && (long_Degrees > -1) && (long_Degrees < 9))
      result = 31;
   if ((lat_Degrees > 71) && (long_Degrees > 8) && (long_Degrees < 21))
      result = 33;
   if ((lat_Degrees > 71) && (long_Degrees > 20) && (long_Degrees < 33))
      result = 35;
   if ((lat_Degrees > 71) && (long_Degrees > 32) && (long_Degrees < 42))
      result = 37;

return result;
}
}
} // namespace otb
