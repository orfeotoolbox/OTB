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
/** Initialize the singleton */
DEMHandler::Pointer DEMHandler::m_Singleton = NULL;

DEMHandler::Pointer DEMHandler::Instance()
{
  if(m_Singleton.GetPointer() == NULL)
    {
    m_Singleton = itk::ObjectFactory<Self>::Create();

    if(m_Singleton.GetPointer() == NULL)
      {
      m_Singleton = new DEMHandler;
      }
    m_Singleton->UnRegister();
    }
  
  return m_Singleton;
}

DEMHandler * DEMHandler::New()
{
  return Instance();
}

DEMHandler
::DEMHandler() :
  m_ElevManager(ossimElevManager::instance()),
  m_GeoidFile(""),
  m_DefaultHeightAboveEllipsoid(0)
{
  m_ElevManager->setDefaultHeightAboveEllipsoid(m_DefaultHeightAboveEllipsoid);
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
      // Ossim does not allow to retrieve the geoid file path
      // We therefore must keep it on our side
      m_GeoidFile = geoidFile;
      otbMsgDevMacro(<< "Geoid successfully opened");
      ossimGeoidManager::instance()->addGeoid(geoidPtr);
      geoidPtr.release();

      // Force geoid fallback
      m_ElevManager->setUseGeoidIfNullFlag(true);
      // The previous flag will be ignored if
      // defaultHeightAboveEllipsoid is not NaN
      m_ElevManager->setDefaultHeightAboveEllipsoid(ossim::nan());

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
  // Ossim does not allow to retrieve the default height above
  // ellipsoid We therefore must keep it on our side
  m_DefaultHeightAboveEllipsoid = h;
  m_ElevManager->setDefaultHeightAboveEllipsoid(h);
}

double
DEMHandler
::GetDefaultHeightAboveEllipsoid() const
{
  // Ossim does not allow to retrieve the default height above
  // ellipsoid We therefore must keep it on our side
  return m_DefaultHeightAboveEllipsoid;
}

std::string DEMHandler::GetDEMDirectory(unsigned int idx) const
{
  std::string demDir = "";

  if(m_ElevManager->getNumberOfElevationDatabases() > 0)
    {
    demDir = m_ElevManager->getElevationDatabase(idx)->getConnectionString().string();
    }
  return demDir;
}

std::string DEMHandler::GetGeoidFile() const
{
  // Ossim does not allow to retrieve the geoid file path
  // We therefore must keep it on our side
  return m_GeoidFile;
}

void
DEMHandler
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "DEMHandler" << std::endl;
}

} // namespace otb
