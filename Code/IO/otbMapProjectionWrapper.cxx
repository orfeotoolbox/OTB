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
#include "otbMacro.h"

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

namespace otb
{

MapProjectionWrapper::MapProjectionWrapper():
  m_MapProjection(NULL), m_ProjectionRefWkt(""), reinstanciateProjection(true)
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
  if ((reinstanciateProjection) /*|| (m_MapProjection == NULL)*/)
    {
    this->InstanciateProjection();
    }

  return this->m_MapProjection;
}

MapProjectionWrapper::InternalMapProjectionConstPointer MapProjectionWrapper::GetMapProjection() const
{
  itkDebugMacro("returning MapProjection address " << this->m_MapProjection);
  if ((reinstanciateProjection) /*|| (m_MapProjection == NULL)*/)
    {
    itkExceptionMacro(<< "m_MapProjection not up-to-date, call InstanciateProjection() first");
    }

  return this->m_MapProjection;
}


std::string MapProjectionWrapper::GetWkt()
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
  reinstanciateProjection = true;
  this->InstanciateProjection();
  this->Modified();
}

bool MapProjectionWrapper::InstanciateProjection()
{
  if ((this->reinstanciateProjection) || (m_MapProjection == NULL))
    {
    ossimKeywordlist      kwl;
    ossimOgcWktTranslator wktTranslator;

    bool projectionInformationAvailable = wktTranslator.toOssimKwl(m_ProjectionRefWkt, kwl);

    if (!projectionInformationAvailable)
      {
      otbMsgDevMacro(<< "WARNING: Impossible to create the projection from string: " << m_ProjectionRefWkt);
      return false;
      }

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

    this->reinstanciateProjection = false;
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

void MapProjectionWrapper::PrintMap() const
{
  std::cout << m_MapProjection->print(std::cout);
}
} // namespace otb
