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

#include "otbMapProjectionWrapper.h"

#include <cassert>

#include "otbMacro.h"
#include "otbUtils.h"

#include "projection/ossimMapProjection.h"
#include "projection/ossimMapProjectionFactory.h"
#include "projection/ossimMapProjection.h"
#include "base/ossimGpt.h"
#include "base/ossimDpt.h"
#include "projection/ossimProjection.h"
#include "base/ossimEllipsoid.h"
#include "base/ossimEllipsoidFactory.h"
#include "base/ossimString.h"
#include "gdal/ossimOgcWktTranslator.h"

#include "projection/ossimUtmProjection.h"

namespace otb
{

MapProjectionWrapper::MapProjectionWrapper():
  m_MapProjection(NULL), m_ProjectionRefWkt(""), m_ReinstanciateProjection(true)
{
}

MapProjectionWrapper::~MapProjectionWrapper()
{
  if (m_MapProjection != NULL)
    {
    delete m_MapProjection;
    }
}

MapProjectionWrapper::InternalMapProjectionPointer MapProjectionWrapper::GetMapProjection()
{
  itkDebugMacro("returning MapProjection address " << this->m_MapProjection);
  if ((m_ReinstanciateProjection) || (m_MapProjection == NULL))
    {
    this->InstanciateProjection();
    }

  return this->m_MapProjection;
}

MapProjectionWrapper::InternalMapProjectionConstPointer MapProjectionWrapper::GetMapProjection() const
{
  itkDebugMacro("returning MapProjection address " << this->m_MapProjection);
  if ((m_ReinstanciateProjection) || (m_MapProjection == NULL))
    {
    itkExceptionMacro(<< "m_MapProjection not up-to-date, call InstanciateProjection() first");
    }

  return this->m_MapProjection;
}

std::string MapProjectionWrapper::GetWkt() const
{
  ossimKeywordlist kwl;
  this->GetMapProjection();
  if (m_MapProjection == NULL)
    {
    return "";
    }
  m_MapProjection->saveState(kwl);
  ossimOgcWktTranslator wktTranslator;
  std::string           wkt;
  wkt = wktTranslator.fromOssimKwl(kwl);
  return wkt;
}

void MapProjectionWrapper::SetWkt(std::string projectionRefWkt)
{
  this->m_ProjectionRefWkt = projectionRefWkt;
  m_ReinstanciateProjection = true;
  this->InstanciateProjection(); //Should not be needed, but it is...
  this->Modified();
}

void MapProjectionWrapper::SetParameter(std::string key, std::string value)
{
  m_ParameterStore[key] = value;
  m_ReinstanciateProjection = true;
  this->InstanciateProjection(); //Should not be needed, but it is...
  this->Modified();
}

std::string MapProjectionWrapper::GetParameter(std::string key) const
{
  // Please refer to the note in the header filer
  // we do NOT want to read from m_ParameterStore here!

  std::string projectionName = this->GetMapProjection()->getClassName();

  // Start by matching generic parameters
  const ossimMapProjection* projection = dynamic_cast<const ossimMapProjection*>(this->GetMapProjection());
  if (key.compare("Origin") == 0)
    {
    return Utils::ConvertToString(projection->origin());
    }
  if (key.compare("FalseNorthing") == 0)
    {
    return Utils::ConvertToString(projection->getFalseNorthing());
    }
  if (key.compare("FalseEasting") == 0)
    {
    return Utils::ConvertToString(projection->getFalseEasting());
    }
  if (key.compare("StandardParallel1") == 0)
    {
    return Utils::ConvertToString(projection->getStandardParallel1());
    }
  if (key.compare("StandardParallel2") == 0)
    {
    return Utils::ConvertToString(projection->getStandardParallel2());
    }
  if (key.compare("A") == 0)
    {
    return Utils::ConvertToString(projection->getA());
    }
  if (key.compare("B") == 0)
    {
    return Utils::ConvertToString(projection->getB());
    }
  if (key.compare("F") == 0)
    {
    return Utils::ConvertToString(projection->getF());
    }
  if (key.compare("MetersPerPixel") == 0)
    {
    return Utils::ConvertToString(projection->getMetersPerPixel());
    }
  if (key.compare("DecimalDegreesPerPixel") == 0)
    {
    return Utils::ConvertToString(projection->getDecimalDegreesPerPixel());
    }


  // Apply parameters to Utm
  if (projectionName.compare("ossimUtmProjection") == 0)
    {
    const ossimUtmProjection* projection = dynamic_cast<const ossimUtmProjection*>(this->GetMapProjection());
    if (key.compare("Zone") == 0)
      {
      return Utils::ConvertToString(projection->getZone());
      }
    if (key.compare("Hemisphere") == 0)
      {
      return Utils::ConvertToString(projection->getHemisphere());
      }
    }

  return "";
}

bool MapProjectionWrapper::InstanciateProjection()
{
  if ((this->m_ReinstanciateProjection) || (m_MapProjection == NULL))
    {
    ossimKeywordlist      kwl;
    ossimOgcWktTranslator wktTranslator;

    bool projectionInformationAvailable = wktTranslator.toOssimKwl(m_ProjectionRefWkt, kwl);

    if (projectionInformationAvailable)
      {

      //we don't want to have a ossimEquDistCylProjection here:
      //see discussion in May 2009 on ossim list;
      //a better solution might be available...
      std::string projectionString(kwl.find("type"));
      if (projectionString.find("ossimEquDistCylProjection") != string::npos)
        {
        otbMsgDevMacro(<< "WARNING: Not instanciating a ossimEquDistCylProjection: " << projectionString);
        otbMsgDevMacro(<< "Wkt was: " << kwl);
        otbMsgDevMacro(<< "From RefWkt: " << m_ProjectionRefWkt);
        return false;
        }

      m_MapProjection = ossimMapProjectionFactory::instance()->createProjection(kwl);

      }
    else
      {
      otbMsgDevMacro(<< "WARNING: Impossible to create the projection from Wkt: " << m_ProjectionRefWkt);
      otbMsgDevMacro(<< "Trying with string as a string (ossimUtmProjection or Utm would qualify");
      // Trying directly with the m_ProjectionRefWkt (is
      // ossimUtmProjection for example)
      ossimString name(m_ProjectionRefWkt);
      m_MapProjection = ossimMapProjectionFactory::instance()->createProjection(name);
      if (m_MapProjection == NULL)
        {
        // Trying directly extending the m_ProjectionRefWkt (convert the
        // Utm to ossimUtmProjection for example)
        ossimString extendedName("ossim");
        extendedName += m_ProjectionRefWkt;
        extendedName += "Projection";
        m_MapProjection = ossimMapProjectionFactory::instance()->createProjection(extendedName);
        }

      if (m_MapProjection == NULL) return false;

      }

    this->m_ReinstanciateProjection = false;
    this->ApplyParametersToProjection();
    return true;
    }
  return false;
}

void MapProjectionWrapper::InverseTransform(double x, double y, double z,
                                       double& lon, double& lat, double& h)
{
  if (m_MapProjection == NULL)
    {
    otbMsgDevMacro(<< "WARNING: Using identity");
    lon = x;
    lat = y;
    h = z;
    return;
    }

  ossimDpt ossimDPoint(x, y);

  //map projection
  ossimGpt ossimGPoint;
  ossimGPoint = this->GetMapProjection()->inverse(ossimDPoint);
  ossimGPoint.changeDatum(ossimDatumFactory::instance()->wgs84());

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h = z;
}

void MapProjectionWrapper::ForwardTransform(double lon, double lat, double h,
                                       double& x, double& y, double& z)
{
  if (m_MapProjection == NULL)
    {
    otbMsgDevMacro(<< "WARNING: Using identity");
    x = lon;
    y = lat;
    z = h;
    return;
    }

  ossimGpt ossimGPoint(lat, lon, h);

  //map projection
  ossimDpt ossimDPoint;
  ossimDPoint = this->GetMapProjection()->forward(ossimGPoint);

  x = ossimDPoint.x;
  y = ossimDPoint.y;
  z = h;
}

void MapProjectionWrapper::ApplyParametersToProjection()
{
  // Start by identifying the projection, that will be necessary for
  // the casting.
  std::string projectionName = this->GetMapProjection()->getClassName();

  StoreType::const_iterator it;

  // Apply parameters to Utm
  if (projectionName.compare("ossimUtmProjection") == 0)
    {
    ossimUtmProjection* projection = dynamic_cast<ossimUtmProjection*>(this->GetMapProjection());
    it = m_ParameterStore.find("Zone");
    if (it != m_ParameterStore.end())
      {
      int zone = atoi((*it).second.c_str());
      projection->setZone(zone);
      }
    it = m_ParameterStore.find("Hemisphere");
    if (it != m_ParameterStore.end())
      {
      projection->setHemisphere((*it).second[0]);
      }
    }
}

void MapProjectionWrapper::PrintMap() const
{
  std::cout << m_MapProjection->print(std::cout);
  std::cout << "Parameter store:\n";
  for (StoreType::const_iterator it = m_ParameterStore.begin();
       it != m_ParameterStore.end();
       ++it)
    {
    std::cout << "  " << (*it).first << ": " << (*it).second << "\n";
    }
}


namespace Utils
{

int GetZoneFromGeoPoint(double lon, double lat)
{
  //use ossim to handle the special case of UTM
  ossimGpt point(lat, lon);
  ossimUtmProjection projection;
  int zone = projection.computeZone(point);
  return zone;
}

}

} // namespace otb
