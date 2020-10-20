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


#include "otbSensorModelAdapter.h"

#include <cassert>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Waddress"
#if defined(__GNUC__) && (__GNUC__ > 5)
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimSensorModelFactory.h"
#include "ossim/projection/ossimSensorModel.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/base/ossimTieGptSet.h"
#include "ossim/projection/ossimRpcSolver.h"
#include "ossim/imaging/ossimImageGeometry.h"

#pragma GCC diagnostic pop
#else
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimSensorModelFactory.h"
#include "ossim/projection/ossimSensorModel.h"
#include "ossim/projection/ossimRpcProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/base/ossimTieGptSet.h"
#include "ossim/projection/ossimRpcSolver.h"
#include "ossim/imaging/ossimImageGeometry.h"

#endif


namespace otb
{

SensorModelAdapter::SensorModelAdapter() : m_SensorModel(nullptr), m_TiePoints(nullptr) // FIXME keeping the original value but...
{
  m_TiePoints  = new ossimTieGptSet();
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
  if (m_SensorModel == nullptr)
  {
    m_SensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
  }
}

bool SensorModelAdapter::IsValidSensorModel() const
{
  return m_SensorModel != nullptr;
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y, double z, double& lon, double& lat, double& h) const
{
  if (this->m_SensorModel == nullptr)
  {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
  }

  ossimDpt ossimPoint(internal::ConvertToOSSIMFrame(x), internal::ConvertToOSSIMFrame(y));
  ossimGpt ossimGPoint;

  this->m_SensorModel->lineSampleHeightToWorld(ossimPoint, z, ossimGPoint);

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h   = ossimGPoint.hgt;
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y, double& lon, double& lat, double& h) const
{
  if (this->m_SensorModel == nullptr)
  {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
  }

  ossimDpt ossimPoint(internal::ConvertToOSSIMFrame(x), internal::ConvertToOSSIMFrame(y));
  ossimGpt ossimGPoint;

  this->m_SensorModel->lineSampleToWorld(ossimPoint, ossimGPoint);

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h   = ossimGPoint.hgt;
}

void SensorModelAdapter::InverseTransformPoint(double lon, double lat, double h, double& x, double& y, double& z) const
{
  if (this->m_SensorModel == nullptr)
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


void SensorModelAdapter::InverseTransformPoint(double lon, double lat, double& x, double& y, double& z) const
{
  if (this->m_SensorModel == nullptr)
  {
    itkExceptionMacro(<< "InverseTransformPoint(): Invalid sensor model (m_SensorModel pointer is null)");
  }

  // Get elevation from DEMHandler
  double h = DEMHandler::GetInstance().GetHeightAboveEllipsoid(lon, lat);

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
  ossimDpt imagePoint(internal::ConvertToOSSIMFrame(x), internal::ConvertToOSSIMFrame(y));
  ossimGpt ossimGPoint(lat, lon, z);

  // Add the tie point to the container
  m_TiePoints->addTiePoint(new ossimTieGpt(ossimGPoint, imagePoint, 0));
}

void SensorModelAdapter::AddTiePoint(double x, double y, double lon, double lat)
{
  // Create the tie point
  ossimDpt imagePoint(internal::ConvertToOSSIMFrame(x), internal::ConvertToOSSIMFrame(y));

  // Get elevation from DEMHandler
  double z = DEMHandler::GetInstance().GetHeightAboveEllipsoid(lon, lat);

  ossimGpt ossimGPoint(lat, lon, z);

  // Add the tie point to the container
  m_TiePoints->addTiePoint(new ossimTieGpt(ossimGPoint, imagePoint, 0));
}


void SensorModelAdapter::ClearTiePoints()
{
  m_TiePoints->clearTiePoints();
}

double SensorModelAdapter::Optimize()
{
  double precision = 0.;
  // If tie points and model are allocated
  if (m_SensorModel != nullptr)
  {
    // try to retrieve a sensor model

    ossimSensorModel* sensorModel = dynamic_cast<ossimSensorModel*>(m_SensorModel);

    ossimRpcProjection* simpleRpcModel = dynamic_cast<ossimRpcProjection*>(m_SensorModel);

    // Handle exceptions
    if ((sensorModel == nullptr) && (simpleRpcModel == nullptr))
      itkExceptionMacro(<< "Optimize(): error, both dynamic_cast from ossimProjection* to ossimSensorModel* / ossimRpcProjection* failed.");

    if (sensorModel != nullptr)
    {
      // Call optimize fit
      precision = sensorModel->optimizeFit(*m_TiePoints);
    }
    else if (simpleRpcModel != nullptr)
    {
      ossimRefPtr<ossimRpcSolver> rpcSolver = new ossimRpcSolver(false, false);

      std::vector<ossimDpt> imagePoints;
      std::vector<ossimGpt> groundPoints;
      m_TiePoints->getSlaveMasterPoints(imagePoints, groundPoints);
      rpcSolver->solveCoefficients(imagePoints, groundPoints);

#if OTB_OSSIM_VERSION < 20200
      ossimRefPtr<ossimRpcProjection> rpcProjection = dynamic_cast<ossimRpcProjection*>(rpcSolver->createRpcProjection()->getProjection());
#else
      ossimRefPtr<ossimRpcModel> rpcProjection = rpcSolver->getRpcModel();
#endif

      if (!rpcProjection)
      {
        itkExceptionMacro(<< "Optimize(): Failed to solve RPC!");
      }

      ossimKeywordlist geom;
      rpcProjection->saveState(geom);
#if OTB_OSSIM_VERSION < 20200
      simpleRpcModel->loadState(geom);
#else
      // we have to convert simpleRpcModel into an ossimRpcModel
      delete m_SensorModel;
      m_SensorModel = new ossimRpcModel;
      m_SensorModel->loadState(geom);
#endif

      precision = std::pow(rpcSolver->getRmsError(), 2);
    }
  }

  // Return the precision
  return precision;
}

bool SensorModelAdapter::ReadGeomFile(const std::string& infile)
{
  ossimKeywordlist geom;

  geom.add(infile.c_str());

  m_SensorModel = ossimSensorModelFactory::instance()->createProjection(geom);

  // search for ossimRpcProjection (not in ossimSensorModelFactory since OSSIM 2)
  const char* typeValue = geom.find(0, "type");
  if (m_SensorModel == nullptr && strcmp(typeValue, "ossimRpcProjection") == 0)
  {
    m_SensorModel = new ossimRpcProjection;
    if (!m_SensorModel->loadState(geom))
    {
      delete m_SensorModel;
      m_SensorModel = nullptr;
    }
  }

  if (m_SensorModel == nullptr)
  {
    m_SensorModel = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
  }

  // otbMsgDevMacro(<< "ReadGeomFile("<<geom<<") -> " << m_SensorModel);
  return (m_SensorModel != nullptr);
}

bool SensorModelAdapter::WriteGeomFile(const std::string& outfile)
{
  // If tie points and model are allocated
  if (m_SensorModel != nullptr)
  {
    // try to retrieve a sensor model
    ossimSensorModel* sensorModel = dynamic_cast<ossimSensorModel*>(m_SensorModel);

    ossimRpcProjection* simpleRpcModel = dynamic_cast<ossimRpcProjection*>(m_SensorModel);

    // Handle exceptions
    if ((sensorModel == nullptr) && (simpleRpcModel == nullptr))
      itkExceptionMacro(<< "Optimize(): error, both dynamic_cast from ossimProjection* to ossimSensorModel* / ossimRpcProjection* failed.");

    ossimKeywordlist geom;
    bool             success = false;
    if (sensorModel != nullptr)
    {
      // Save state
      success = sensorModel->saveState(geom);
    }
    else if (simpleRpcModel != nullptr)
    {
      // Save state
      success = simpleRpcModel->saveState(geom);
    }

    if (success)
    {
      return geom.write(outfile.c_str());
    }
  }
  return false;
}

} // namespace otb
