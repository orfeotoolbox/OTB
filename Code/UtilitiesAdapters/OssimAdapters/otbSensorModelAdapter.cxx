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
  m_SensorModel(NULL), m_TiePoints(NULL), m_UseDEM(false), m_Epsilon(0.0001),  m_NbIter(1) // FIXME keeping the original value but...
{
  m_DEMHandler = DEMHandler::New();
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


void SensorModelAdapter::SetDEMDirectory(const std::string& directory)
{
  m_DEMHandler->OpenDEMDirectory(directory);
  m_UseDEM = true;
}

void SensorModelAdapter::SetGeoidFile(const std::string& geoidFile)
{
  m_DEMHandler->OpenGeoidFile(geoidFile);
}

void SensorModelAdapter::ForwardTransformPoint(double x, double y, double z,
                                               double& lon, double& lat, double& h) const
{
  ossimDpt ossimPoint(x, y);

  // Calculation
  ossimGpt ossimGPoint;

  if (this->m_SensorModel == NULL)
    {
    itkExceptionMacro(<< "ForwardTransformPoint(): Invalid Model pointer m_SensorModel == NULL !");
    }

  //Use of DEM: need iteration to reach the correct point
  if (this->m_UseDEM)
    {
    this->m_SensorModel->lineSampleToWorld(ossimPoint, ossimGPoint);
    lon = ossimGPoint.lon;
    lat = ossimGPoint.lat;

    ossimGpt ossimGPointRef = ossimGPoint;
    double height(0.), heightTmp(0.);
    double                diffHeight = 100; // arbitrary value
    itk::Point<double, 2> currentPoint;
    int                   nbIter = 0;

    otbMsgDevMacro(<< "USING DEM ! ");

    while ((diffHeight > m_Epsilon) && (nbIter < m_NbIter))
      {
      otbMsgDevMacro(<< "Iter " << nbIter);

      if (nbIter != 0) height = heightTmp;

      heightTmp = this->m_DEMHandler->GetHeightAboveMSL(lon, lat);

      this->m_SensorModel->lineSampleHeightToWorld(ossimPoint, heightTmp, ossimGPointRef);

      diffHeight = fabs(heightTmp - height);

      ++nbIter;
      }
    ossimGPoint = ossimGPointRef;
    }
  //Altitude of the point is provided (in the sensor coordinate) could be an
  //average elevation
  else if (z != -32768)
    {
    this->m_SensorModel->lineSampleHeightToWorld(ossimPoint, z, ossimGPoint);
    }
  //Otherwise, just don't consider the altitude
  else
    {
    this->m_SensorModel->lineSampleToWorld(ossimPoint, ossimGPoint);
    }

  lon = ossimGPoint.lon;
  lat = ossimGPoint.lat;
  h = ossimGPoint.hgt;
}

void SensorModelAdapter::InverseTransformPoint(double lon, double lat, double h,
                                               double& x, double& y, double& z) const
{
  // Initialize with value from the function parameters
  ossimGpt ossimGPoint(lat, lon, h);

  // In case a DEM is used, override the elevation parameter by the DEM value
  if (this->m_UseDEM)
    {
    double height = this->m_DEMHandler->GetHeightAboveMSL(lon, lat);
    // If the DEM handler cannot give the height for the (lon, lat) point,
    // either because the tile is not available or the value in the tile are all -32768,
    // it will return ossim::nan()
    ossimGPoint.height(height);
    }

  // If the 'h' parameter is -32768 (case where OTB classes use a default AverageElevation value)
  if (ossimGPoint.height() == -32768)
    {
    otbMsgDevMacro(<< "The given altitude corresponds to NoData (value is -32768)");
    ossimGPoint.height( ossim::nan() );
    }

  ossimDpt ossimDPoint;

  if (this->m_SensorModel == NULL)
    {
    itkExceptionMacro(<< "InverseTransformPoint(): Invalid Model pointer m_SensorModel == NULL !");
    }

  // Note: the -32768 is only here to show unknown altitude and should never be
  // passed to ossim.
  // We should either have a NaN, either a valid elevation value
  assert(ossim::isnan(ossimGPoint.height()) || (ossimGPoint.height() > -1000));

  this->m_SensorModel->worldToLineSample(ossimGPoint, ossimDPoint); //"worldToLineSample" call "lineSampleHeightToWorld" method for take in care elevation information.
  x = ossimDPoint.x;
  y = ossimDPoint.y;
  z = ossimGPoint.height();
}


void SensorModelAdapter::AddTiePoint(double x, double y, double z, double lon, double lat)
{
  // Create the tie point
  ossimDpt imagePoint(x,y);
  ossimGpt ossimGPoint(lat, lon);

  if (this->m_UseDEM)
    {
    double height = this->m_DEMHandler->GetHeightAboveMSL(lon, lat);
    if(!ossim::isnan(height))
      {
      ossimGPoint.height(height);
      }
    }
  else if (z != -32768)
    {
    ossimGPoint.height(z);
    }

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
