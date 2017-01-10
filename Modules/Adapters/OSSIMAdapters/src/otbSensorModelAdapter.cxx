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

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimSensorModelFactory.h"
#include "ossim/projection/ossimSensorModel.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/base/ossimTieGptSet.h"

#pragma GCC diagnostic pop
#else
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimSensorModelFactory.h"
#include "ossim/projection/ossimSensorModel.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/base/ossimTieGptSet.h"

#endif


namespace otb
{

SensorModelAdapter::SensorModelAdapter():
  m_SensorModel(ITK_NULLPTR), m_TiePoints(ITK_NULLPTR) // FIXME keeping the original value but...
{
  m_DEMHandler = DEMHandler::Instance();
  m_TiePoints = new ossimTieGptSet();
}

SensorModelAdapter::~SensorModelAdapter()
{
  delete m_SensorModel;
  delete m_TiePoints;
}

void SensorModelAdapter::CreateProjection(const ImageKeywordlist& image_kwl)
{
  ossimKeywordlist geom;

  image_kwl.convertToOSSIMKeywordlist(geom);
  otbMsgDevMacro(<< "CreateProjection()");
  otbMsgDevMacro(<< "* type: " << geom.find("type"));

  m_SensorModel = ossimSensorModelFactory::instance()->createProjection(geom);
  if (m_SensorModel == ITK_NULLPTR)
    {
    m_SensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
    }
}

bool SensorModelAdapter::IsValidSensorModel() const
{
  return m_SensorModel != ITK_NULLPTR;
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y, double z,
                                               double& lon, double& lat, double& h) const
{
  if (this->m_SensorModel == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  ossimDpt ossimPoint( internal::ConvertToOSSIMFrame(x),
                       internal::ConvertToOSSIMFrame(y));
  ossimGpt ossimGPoint;

  this->m_SensorModel->lineSampleHeightToWorld(ossimPoint,z, ossimGPoint);

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h = ossimGPoint.hgt;
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y,
                                               double& lon, double& lat, double& h) const
{
  if (this->m_SensorModel == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  ossimDpt ossimPoint( internal::ConvertToOSSIMFrame(x),
                       internal::ConvertToOSSIMFrame(y));
  ossimGpt ossimGPoint;

  this->m_SensorModel->lineSampleToWorld(ossimPoint, ossimGPoint);

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h = ossimGPoint.hgt;
}

void SensorModelAdapter::InverseTransformPoint(double lon, double lat, double h,
                                               double& x, double& y, double& z) const
{
  if (this->m_SensorModel == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "InverseTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  // Initialize with value from the function parameters
  ossimGpt ossimGPoint(lat, lon, h);
  ossimDpt ossimDPoint;

  this->m_SensorModel->worldToLineSample(ossimGPoint, ossimDPoint);

  x = internal::ConvertFromOSSIMFrame(ossimDPoint.x);
  y = internal::ConvertFromOSSIMFrame(ossimDPoint.y);
  z = ossimGPoint.height();
}


void SensorModelAdapter::InverseTransformPoint(double lon, double lat,
                                               double& x, double& y, double& z) const
{
  if (this->m_SensorModel == ITK_NULLPTR)
    {
    itkExceptionMacro(<< "InverseTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
    }

  // Get elevation from DEMHandler
  double h = m_DEMHandler->GetHeightAboveEllipsoid(lon,lat);

  // Initialize with value from the function parameters
  ossimGpt ossimGPoint(lat, lon, h);
  ossimDpt ossimDPoint;

  this->m_SensorModel->worldToLineSample(ossimGPoint, ossimDPoint);

  x = internal::ConvertFromOSSIMFrame(ossimDPoint.x);
  y = internal::ConvertFromOSSIMFrame(ossimDPoint.y);
  z = ossimGPoint.height();
}

void SensorModelAdapter::AddTiePoint(double x, double y, double z, double lon, double lat)
{
  // Create the tie point
  ossimDpt imagePoint( internal::ConvertToOSSIMFrame(x),
                       internal::ConvertToOSSIMFrame(y));
  ossimGpt ossimGPoint(lat, lon, z);

  // Add the tie point to the container
  m_TiePoints->addTiePoint(new ossimTieGpt(ossimGPoint,imagePoint,0));
}

void SensorModelAdapter::AddTiePoint(double x, double y, double lon, double lat)
{
  // Create the tie point
  ossimDpt imagePoint( internal::ConvertToOSSIMFrame(x),
                       internal::ConvertToOSSIMFrame(y));

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
  if(m_SensorModel != ITK_NULLPTR)
    {
    // try to retrieve a sensor model

    ossimSensorModel * sensorModel = dynamic_cast<ossimSensorModel *>(m_SensorModel);

    ossimRpcProjection * simpleRpcModel = dynamic_cast<ossimRpcProjection *>(m_SensorModel);

     //Handle exceptions
    if ( (sensorModel == ITK_NULLPTR ) && (simpleRpcModel == ITK_NULLPTR ) )
       itkExceptionMacro(<< "Optimize(): error, both dynamic_cast from ossimProjection* to ossimSensorModel* / ossimRpcProjection* failed.");

    if(sensorModel != ITK_NULLPTR )
      {
      // Call optimize fit
      precision  = sensorModel->optimizeFit(*m_TiePoints);
      }
    else if (simpleRpcModel != ITK_NULLPTR)
      {
      // Call optimize fit
      precision  = simpleRpcModel->optimizeFit(*m_TiePoints);
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

  if (m_SensorModel == ITK_NULLPTR)
    {
    m_SensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
    }

  // otbMsgDevMacro(<< "ReadGeomFile("<<geom<<") -> " << m_SensorModel);
  return (m_SensorModel != ITK_NULLPTR);
}

bool SensorModelAdapter::WriteGeomFile(const std::string & outfile)
{
  // If tie points and model are allocated
  if(m_SensorModel != ITK_NULLPTR)
    {
    // try to retrieve a sensor model
    ossimSensorModel * sensorModel = dynamic_cast<ossimSensorModel *>(m_SensorModel);

    ossimRpcProjection * simpleRpcModel = dynamic_cast<ossimRpcProjection *>(m_SensorModel);

    //Handle exceptions
    if ( (sensorModel == ITK_NULLPTR ) && (simpleRpcModel == ITK_NULLPTR ) )
       itkExceptionMacro(<< "Optimize(): error, both dynamic_cast from ossimProjection* to ossimSensorModel* / ossimRpcProjection* failed.");

    ossimKeywordlist geom;
    bool success = false;
    if(sensorModel != ITK_NULLPTR )
      {
      // Save state
      success = sensorModel->saveState(geom);
      }
    else if (simpleRpcModel != ITK_NULLPTR)
      {
      // Save state
      success = simpleRpcModel->saveState(geom);
      }

    if(success)
      {
      return geom.write(outfile.c_str());
      }
    }
  return false;
}

} // namespace otb
