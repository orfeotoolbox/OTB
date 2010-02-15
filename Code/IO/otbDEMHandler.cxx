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

#include "elevation/ossimElevManager.h"
#include "base/ossimGeoidManager.h"
#include "base/ossimFilename.h"
#include "base/ossimDirectory.h"
#include "base/ossimGeoidEgm96.h"

namespace otb
{

DEMHandler
::DEMHandler():
  m_ElevManager(ossimElevManager::instance())
{
}

DEMHandler
::~DEMHandler()
{
}

void
DEMHandler
::OpenDEMDirectory(const char* DEMDirectory)
{
  ossimFilename ossimDEMDir;
  ossimDEMDir=ossimFilename(DEMDirectory);

  ossimDirectory od(DEMDirectory);
  if (!m_ElevManager->loadElevationPath(ossimDEMDir))
  {
    itkExceptionMacro("Failed to open DEM Directory: "<<ossimDEMDir);
  }
}

void
DEMHandler
::OpenGeoidFile(const char* geoidFile)
{
  if ((ossimGeoidManager::instance()->findGeoidByShortName("geoid1996")) == 0)
  {
    otbMsgDevMacro( << "Opening geoid: " << geoidFile);
    ossimFilename geoid(geoidFile);
    ossimGeoid* geoidPtr = new ossimGeoidEgm96(geoid);
    if (geoidPtr->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
    {
       ossimGeoidManager::instance()->addGeoid(geoidPtr);
    }
    else
    {
      delete geoidPtr;
    }
  }
}


double
DEMHandler
::GetHeightAboveMSL(const PointType& geoPoint) const
{
  double height;
  ossimGpt ossimWorldPoint;
  ossimWorldPoint.lon=geoPoint[0];
  ossimWorldPoint.lat=geoPoint[1];
  height=m_ElevManager->getHeightAboveMSL(ossimWorldPoint);
  return height;
}

double
DEMHandler
::GetHeightAboveEllipsoid(const PointType& geoPoint) const
{
  double height;
  ossimGpt ossimWorldPoint;
  ossimWorldPoint.lon=geoPoint[0];
  ossimWorldPoint.lat=geoPoint[1];
  height=m_ElevManager->getHeightAboveEllipsoid(ossimWorldPoint);
  return height;
}

void
DEMHandler
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "DEMHandler" << std::endl;
}

} // namespace otb
