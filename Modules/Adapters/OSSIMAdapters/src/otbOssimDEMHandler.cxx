/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbOssimDEMHandler.h"
#include "otbMacro.h"

#include <cassert>

#include "otb_ossim.h"

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
OssimDEMHandler::Pointer OssimDEMHandler::m_Singleton = nullptr;

OssimDEMHandler::Pointer OssimDEMHandler::Instance()
{
  if (m_Singleton.GetPointer() == nullptr)
  {
    m_Singleton = itk::ObjectFactory<Self>::Create();

    if (m_Singleton.GetPointer() == nullptr)
    {
      m_Singleton = new OssimDEMHandler;
    }
    m_Singleton->UnRegister();
  }

  return m_Singleton;
}

OssimDEMHandler::OssimDEMHandler() : m_GeoidFile(""), m_DefaultHeightAboveEllipsoid(0)
{
  assert(ossimElevManager::instance() != NULL);

  ossimElevManager::instance()->setDefaultHeightAboveEllipsoid(m_DefaultHeightAboveEllipsoid);
  // Force geoid fallback
  ossimElevManager::instance()->setUseGeoidIfNullFlag(true);
}

void OssimDEMHandler::OpenDEMDirectory(const char* DEMDirectory)
{
  assert(ossimElevManager::instance() != NULL);

  ossimFilename ossimDEMDir(DEMDirectory);

  if (!ossimElevManager::instance()->loadElevationPath(ossimDEMDir))
  {
    // In ossim elevation database factory code, the
    // ossimImageElevationDatabase is explicitly disabled by a #if 0
    // guard, because it causes problem when loading ossim related
    // application. Therefore, we explicitly call
    // ossimImageElevationDatabase here to allow for general elevation
    // images support.
    ossimRefPtr<ossimElevationDatabase> imageElevationDatabase = new ossimImageElevationDatabase;

    if (!imageElevationDatabase->open(DEMDirectory))
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


void OssimDEMHandler::ClearDEMs()
{
  assert(ossimElevManager::instance() != NULL);

  ossimElevManager::instance()->clear();
}


void OssimDEMHandler::OpenDEMDirectory(const std::string& DEMDirectory)
{
  OpenDEMDirectory(DEMDirectory.c_str());
}

bool OssimDEMHandler::IsValidDEMDirectory(const char* DEMDirectory)
{
  assert(ossimElevManager::instance() != NULL);

  // Try to load elevation source
  bool result = ossimElevManager::instance()->loadElevationPath(DEMDirectory);

  if (!result)
  {
    // we explicitly call ossimImageElevationDatabase here to allow for general elevation
    // images support and test the open method to check if the directory .
    ossimRefPtr<ossimElevationDatabase> imageElevationDatabase = new ossimImageElevationDatabase;
    result                                                     = imageElevationDatabase->open(DEMDirectory);
  }
  return result;
}

bool OssimDEMHandler::OpenGeoidFile(const char* geoidFile)
{
  if ((ossimGeoidManager::instance()->findGeoidByShortName("geoid1996")) == nullptr)
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
      assert(ossimElevManager::instance() != NULL);

      ossimElevManager::instance()->setDefaultHeightAboveEllipsoid(ossim::nan());

      return true;
    }
    else
    {
      otbMsgDevMacro(<< "Failure opening geoid");
      geoidPtr.release();

      itkExceptionMacro(<< "Failed to open geoid file: '" << geoidFile << "'");

      return false;
    }
  }

  return false;
}

bool OssimDEMHandler::OpenGeoidFile(const std::string& geoidFile)
{
  return OpenGeoidFile(geoidFile.c_str());
}

double OssimDEMHandler::GetHeightAboveMSL(double lon, double lat) const
{
  double   height;
  ossimGpt ossimWorldPoint;

  ossimWorldPoint.lon = lon;
  ossimWorldPoint.lat = lat;

  assert(ossimElevManager::instance() != NULL);

  height = ossimElevManager::instance()->getHeightAboveMSL(ossimWorldPoint);

  return height;
}

double OssimDEMHandler::GetHeightAboveMSL(const PointType& geoPoint) const
{
  return GetHeightAboveMSL(geoPoint[0], geoPoint[1]);
}

double OssimDEMHandler::GetHeightAboveEllipsoid(double lon, double lat) const
{
  double   height;
  ossimGpt ossimWorldPoint;

  ossimWorldPoint.lon = lon;
  ossimWorldPoint.lat = lat;

  assert(ossimElevManager::instance() != NULL);

  height = ossimElevManager::instance()->getHeightAboveEllipsoid(ossimWorldPoint);

  return height;
}

double OssimDEMHandler::GetHeightAboveEllipsoid(const PointType& geoPoint) const
{
  return GetHeightAboveEllipsoid(geoPoint[0], geoPoint[1]);
}

void OssimDEMHandler::SetDefaultHeightAboveEllipsoid(double h)
{
  // Ossim does not allow retrieving the default height above
  // ellipsoid We therefore must keep it on our side
  m_DefaultHeightAboveEllipsoid = h;

  assert(ossimElevManager::instance() != NULL);

  ossimElevManager::instance()->setDefaultHeightAboveEllipsoid(h);
}

double OssimDEMHandler::GetDefaultHeightAboveEllipsoid() const
{
  // Ossim does not allow retrieving the default height above
  // ellipsoid We therefore must keep it on our side
  return m_DefaultHeightAboveEllipsoid;
}

unsigned int OssimDEMHandler::GetDEMCount() const
{
  assert(ossimElevManager::instance() != NULL);

  return ossimElevManager::instance()->getNumberOfElevationDatabases();
}

std::string OssimDEMHandler::GetDEMDirectory(unsigned int idx) const
{
  std::string demDir = "";

  if (ossimElevManager::instance()->getNumberOfElevationDatabases() > 0)
  {
    assert(ossimElevManager::instance() != NULL);

    demDir = ossimElevManager::instance()->getElevationDatabase(idx)->getConnectionString().string();
  }
  return demDir;
}

std::string OssimDEMHandler::GetGeoidFile() const
{
  // Ossim does not allow retrieving the geoid file path
  // We therefore must keep it on our side
  return m_GeoidFile;
}

void OssimDEMHandler::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "OssimDEMHandler" << std::endl;
}

} // namespace otb
