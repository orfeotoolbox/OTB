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

#include "otbSensorModelAdapter.h"

#include <cassert>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimSensorModelFactory.h"
#include "ossim/projection/ossimSensorModel.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/base/ossimTieGptSet.h"

namespace otb
{

SensorModelAdapter::SensorModelAdapter():
  m_SensorModel(NULL), m_TiePoints(NULL) // FIXME keeping the original value but...
{
  m_DEMHandler = DEMHandler::Instance();
  m_TiePoints = new ossimTieGptSet();
}

SensorModelAdapter::~SensorModelAdapter()
{
  if (m_SensorModel != NULL)
    {
    delete m_SensorModel;
    m_SensorModel = NULL;
    }
  if(m_TiePoints!=NULL)
    {
    delete m_TiePoints;
    m_TiePoints = NULL;
    }
}

void SensorModelAdapter::CreateProjection(const ImageKeywordlist& image_kwl)
{
  ossimKeywordlist geom;

  image_kwl.convertToOSSIMKeywordlist(geom);
  otbMsgDevMacro(<< "CreateProjection()");
  otbMsgDevMacro(<< "* type: " << geom.find("type"));

  m_SensorModel = ossimSensorModelFactory::instance()->createProjection(geom);
  if (m_SensorModel == NULL)
    {
      m_SensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
    }
}

bool SensorModelAdapter::IsValidSensorModel()
{
  if (m_SensorModel == NULL)
    {
      return false;
    }
  else
    {
      return true;
    }
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y, double z,
                                               double& lon, double& lat, double& h) const
{
  if (this->m_SensorModel == NULL)
    {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  ossimDpt ossimPoint(x, y);
  ossimGpt ossimGPoint;

  this->m_SensorModel->lineSampleHeightToWorld(ossimPoint,z, ossimGPoint);

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h = ossimGPoint.hgt;
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y,
                                               double& lon, double& lat, double& h) const
{
  if (this->m_SensorModel == NULL)
    {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  ossimDpt ossimPoint(x, y);
  ossimGpt ossimGPoint;

  this->m_SensorModel->lineSampleToWorld(ossimPoint, ossimGPoint);

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h = ossimGPoint.hgt;
}

void SensorModelAdapter::InverseTransformPoint(double lon, double lat, double h,
                                               double& x, double& y, double& z) const
{
  if (this->m_SensorModel == NULL)
    {
    itkExceptionMacro(<< "InverseTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  // Initialize with value from the function parameters
  ossimGpt ossimGPoint(lat, lon, h);
  ossimDpt ossimDPoint;

  this->m_SensorModel->worldToLineSample(ossimGPoint, ossimDPoint); 

  x = ossimDPoint.x;
  y = ossimDPoint.y;
  z = ossimGPoint.height();
}


void SensorModelAdapter::InverseTransformPoint(double lon, double lat,
                                               double& x, double& y, double& z) const
{
  if (this->m_SensorModel == NULL)
    {
    itkExceptionMacro(<< "InverseTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  // Get elevation from DEMHandler
  double h = m_DEMHandler->GetHeightAboveEllipsoid(lon,lat);

  // Initialize with value from the function parameters
  ossimGpt ossimGPoint(lat, lon, h);
  ossimDpt ossimDPoint;

  this->m_SensorModel->worldToLineSample(ossimGPoint, ossimDPoint); 

  x = ossimDPoint.x;
  y = ossimDPoint.y;
  z = ossimGPoint.height();
}

void SensorModelAdapter::AddTiePoint(double x, double y, double z, double lon, double lat)
{
  // Create the tie point
  ossimDpt imagePoint(x,y);
  ossimGpt ossimGPoint(lat, lon, z);

  // Add the tie point to the container
  m_TiePoints->addTiePoint(new ossimTieGpt(ossimGPoint,imagePoint,0));
}

void SensorModelAdapter::AddTiePoint(double x, double y, double lon, double lat)
{
  // Create the tie point
  ossimDpt imagePoint(x,y);

  // Get elevation from DEMHandler
  double z = m_DEMHandler->GetHeightAboveEllipsoid(lon,lat);

  ossimGpt ossimGPoint(lat, lon, z);

  // Add the tie point to the container
  m_TiePoints->addTiePoint(new ossimTieGpt(ossimGPoint,imagePoint,0));
}


void SensorModelAdapter::ClearTiePoints()
{
  m_TiePoints->clearTiePoints();
}

double SensorModelAdapter::Optimize()
{
  double precision = 0.;

  // If tie points and model are allocated
  if(m_SensorModel != NULL)
    {
    // try to retrieve a sensor model
    ossimSensorModel * sensorModel = NULL;
    sensorModel = dynamic_cast<ossimSensorModel *>(m_SensorModel);

    if(sensorModel != NULL)
      {
      // Call optimize fit
      precision  = sensorModel->optimizeFit(*m_TiePoints);
      }
    }

  // Return the precision
  return precision;
}

bool SensorModelAdapter::ReadGeomFile(const std::string & infile)
{
  ossimKeywordlist geom;

  geom.add(infile.c_str());
  
  m_SensorModel = ossimSensorModelFactory::instance()->createProjection(geom);
  
  if (m_SensorModel == NULL)
    {
    m_SensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
    }
  
  return (m_SensorModel != NULL);
}

bool SensorModelAdapter::WriteGeomFile(const std::string & outfile)
{
  // If tie points and model are allocated
  if(m_SensorModel != NULL)
    {
    // try to retrieve a sensor model
    ossimSensorModel * sensorModel = NULL;
    sensorModel = dynamic_cast<ossimSensorModel *>(m_SensorModel);

    ossimKeywordlist geom;

    bool success = sensorModel->saveState(geom);

    if(success)
      {
      return geom.write(outfile.c_str());
      }
    }
  return false;
}

} // namespace otb
