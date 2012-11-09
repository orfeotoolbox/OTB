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
#include "otbDEMHandler.h"
#include "otbMacro.h"

#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimGeoidManager.h"
#include "ossim/base/ossimFilename.h"
#include "ossim/base/ossimDirectory.h"
#include "ossim/base/ossimGeoidEgm96.h"
#include "ossim/base/ossimRefPtr.h"

namespace otb
{

DEMHandler * DEMHandler::Instance()
{
  // Check for singleton
  if(m_Singleton.IsNull())
    {
    m_Singleton = ::itk::ObjectFactory<Self>::Create();
    }
  return m_Singleton;
}

DEMHandler * DEMHandler::New()
{
  return Instance();
}

DEMHandler
::DEMHandler() :
  m_ElevManager(ossimElevManager::instance())
{
}

void
DEMHandler
::OpenDEMDirectory(const char* DEMDirectory)
{
  ossimFilename ossimDEMDir;
  ossimDEMDir = ossimFilename(DEMDirectory);

  if (!m_ElevManager->loadElevationPath(ossimDEMDir))
    {
    itkExceptionMacro("Failed to open DEM Directory: " << ossimDEMDir);
    }
}

void
DEMHandler
::OpenDEMDirectory(const std::string& DEMDirectory)
{
  OpenDEMDirectory(DEMDirectory.c_str());
}

bool
DEMHandler
::IsValidDEMDirectory(const char* DEMDirectory)
{
  return m_ElevManager->loadElevationPath(DEMDirectory);
}

void
DEMHandler
::OpenGeoidFile(const char* geoidFile)
{
  if ((ossimGeoidManager::instance()->findGeoidByShortName("geoid1996")) == 0)
    {
    otbMsgDevMacro(<< "Opening geoid: " << geoidFile);
    ossimFilename           geoid(geoidFile);
    ossimRefPtr<ossimGeoid> geoidPtr = new ossimGeoidEgm96(geoid);
    if (geoidPtr->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
      {
      otbMsgDevMacro(<< "Geoid successfully opened");
      ossimGeoidManager::instance()->addGeoid(geoidPtr);
      geoidPtr.release();
      }
    else
      {
      otbMsgDevMacro(<< "Failure opening geoid");
      geoidPtr.release();
      }
    }
}

void
DEMHandler
::OpenGeoidFile(const std::string& geoidFile)
{
  OpenGeoidFile(geoidFile.c_str());
}

double
DEMHandler
::GetHeightAboveMSL(double lon, double lat) const
{
  double   height;
  ossimGpt ossimWorldPoint;
  ossimWorldPoint.lon = lon;
  ossimWorldPoint.lat = lat;
  height = m_ElevManager->getHeightAboveMSL(ossimWorldPoint);
  return height;
}

double
DEMHandler
::GetHeightAboveMSL(const PointType& geoPoint) const
{
  return GetHeightAboveMSL(geoPoint[0], geoPoint[1]);
}

double
DEMHandler
::GetHeightAboveEllipsoid(double lon, double lat) const
{
  double   height;
  ossimGpt ossimWorldPoint;
  ossimWorldPoint.lon = lon;
  ossimWorldPoint.lat = lat;
  otbMsgDevMacro(<< "Geoid offset: " << ossimGeoidManager::instance()->offsetFromEllipsoid(ossimWorldPoint));
  height = m_ElevManager->getHeightAboveEllipsoid(ossimWorldPoint);
  return height;
}

double
DEMHandler
::GetHeightAboveEllipsoid(const PointType& geoPoint) const
{
  return GetHeightAboveEllipsoid(geoPoint[0], geoPoint[1]);
}

void
DEMHandler
::SetDefaultHeightAboveEllipsoid(double h)
{
  m_ElevManager->setDefaultHeightAboveEllipsoid(h);
}

void
DEMHandler
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "DEMHandler" << std::endl;
}

} // namespace otb
