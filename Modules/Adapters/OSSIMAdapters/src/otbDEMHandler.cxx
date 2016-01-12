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

#include <cassert>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimGeoidManager.h"
#include "ossim/base/ossimFilename.h"
#include "ossim/base/ossimDirectory.h"
#include "ossim/base/ossimGeoidEgm96.h"
#include "ossim/base/ossimRefPtr.h"
#include <ossim/elevation/ossimImageElevationDatabase.h>
#pragma GCC diagnostic pop
#else
#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimGeoidManager.h"
#include "ossim/base/ossimFilename.h"
#include "ossim/base/ossimDirectory.h"
#include "ossim/base/ossimGeoidEgm96.h"
#include "ossim/base/ossimRefPtr.h"
#include <ossim/elevation/ossimImageElevationDatabase.h>
#endif



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

DEMHandler
::DEMHandler() :
  m_GeoidFile(""),
  m_DefaultHeightAboveEllipsoid(0)
{
  assert( ossimElevManager::instance()!=NULL );

  ossimElevManager::instance()->setDefaultHeightAboveEllipsoid(m_DefaultHeightAboveEllipsoid);
  // Force geoid fallback
  ossimElevManager::instance()->setUseGeoidIfNullFlag(true);
}

void
DEMHandler
::OpenDEMDirectory(const char* DEMDirectory)
{
  assert( ossimElevManager::instance()!=NULL );

  ossimFilename ossimDEMDir( DEMDirectory );

  if (!ossimElevManager::instance()->loadElevationPath(ossimDEMDir))
    {
    // In ossim elevation database factory code, the
    // ossimImageElevationDatabase is explicitly disabled by a #if 0
    // guard, because it causes problem when loading ossim related
    // application. Therefore, we explicitly call
    // ossimImageElevationDatabase here to allow for general elevation
    // images support.
    ossimRefPtr<ossimElevationDatabase> imageElevationDatabase = new ossimImageElevationDatabase;

    if(!imageElevationDatabase->open(DEMDirectory))
      {
      itkExceptionMacro("Failed to open DEM Directory: " << ossimDEMDir);
      }
    else
      {
      otbMsgDevMacro(<< "DEM directory contains general elevation image files: " << ossimDEMDir);
      ossimElevManager::instance()->addDatabase(imageElevationDatabase.get());
      }
    }
}


void
DEMHandler
::ClearDEMs()
{
  assert( ossimElevManager::instance()!=NULL );

  ossimElevManager::instance()->clear();
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
  assert( ossimElevManager::instance()!=NULL );

  //Try to load elevation source
  bool result = ossimElevManager::instance()->loadElevationPath(DEMDirectory);

  if (!result)
    {
      // we explicitly call ossimImageElevationDatabase here to allow for general elevation
      // images support and test the open method to check if the directory .
      ossimRefPtr<ossimElevationDatabase> imageElevationDatabase = new ossimImageElevationDatabase;
      result = imageElevationDatabase->open(DEMDirectory);
    }
  return result;
}

bool
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
      // Ossim does not allow retrieving the geoid file path
      // We therefore must keep it on our side
      m_GeoidFile = geoidFile;
      otbMsgDevMacro(<< "Geoid successfully opened");
      ossimGeoidManager::instance()->addGeoid(geoidPtr);
      geoidPtr.release();

      // The previous flag will be ignored if
      // defaultHeightAboveEllipsoid is not NaN
      assert( ossimElevManager::instance()!=NULL );

      ossimElevManager::instance()->setDefaultHeightAboveEllipsoid(ossim::nan());

      return true;
      }
    else
      {
      otbMsgDevMacro(<< "Failure opening geoid");
      geoidPtr.release();

      itkExceptionMacro( << "Failed to open geoid file: '" << geoidFile << "'" );

      return false;
      }
    }

  return false;
}

bool
DEMHandler
::OpenGeoidFile(const std::string& geoidFile)
{
  return OpenGeoidFile(geoidFile.c_str());
}

double
DEMHandler
::GetHeightAboveMSL(double lon, double lat) const
{
  double   height;
  ossimGpt ossimWorldPoint;

  ossimWorldPoint.lon = lon;
  ossimWorldPoint.lat = lat;

  assert( ossimElevManager::instance()!=NULL );

  height = ossimElevManager::instance()->getHeightAboveMSL(ossimWorldPoint);

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

  assert( ossimElevManager::instance()!=NULL );

  height = ossimElevManager::instance()->getHeightAboveEllipsoid(ossimWorldPoint);

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
  // Ossim does not allow retrieving the default height above
  // ellipsoid We therefore must keep it on our side
  m_DefaultHeightAboveEllipsoid = h;

  assert( ossimElevManager::instance()!=NULL );

  ossimElevManager::instance()->setDefaultHeightAboveEllipsoid(h);
}

double
DEMHandler
::GetDefaultHeightAboveEllipsoid() const
{
  // Ossim does not allow retrieving the default height above
  // ellipsoid We therefore must keep it on our side
  return m_DefaultHeightAboveEllipsoid;
}

unsigned int
DEMHandler
::GetDEMCount() const
{
  assert( ossimElevManager::instance()!=NULL );

  return ossimElevManager::instance()->getNumberOfElevationDatabases();
}

std::string DEMHandler::GetDEMDirectory(unsigned int idx) const
{
  std::string demDir = "";

  if(ossimElevManager::instance()->getNumberOfElevationDatabases() > 0)
    {
    assert( ossimElevManager::instance()!=NULL );

    demDir = ossimElevManager::instance()->getElevationDatabase(idx)->getConnectionString().string();
    }
  return demDir;
}

std::string DEMHandler::GetGeoidFile() const
{
  // Ossim does not allow retrieving the geoid file path
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
