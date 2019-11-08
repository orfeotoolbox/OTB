/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbSarSensorModelAdapter.h"

#include <cassert>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "ossim/ossimSarSensorModel.h"
#include "ossim/ossimPluginProjectionFactory.h"
#pragma GCC diagnostic pop
#else
#include "ossim/ossimSarSensorModel.h"
#include "ossim/ossimPluginProjectionFactory.h"
#endif


namespace otb
{

SarSensorModelAdapter::SarSensorModelAdapter() : m_SensorModel(nullptr)
{
}

SarSensorModelAdapter::~SarSensorModelAdapter()
{
}

bool SarSensorModelAdapter::LoadState(const ImageKeywordlist& image_kwl)
{
  // InternalModelPointer model = new ossimplugins::ossimSarSensorModel();

  ossimKeywordlist geom;
  image_kwl.convertToOSSIMKeywordlist(geom);


  m_SensorModel.reset(dynamic_cast<ossimplugins::ossimSarSensorModel*>(ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom)));

  return IsValidSensorModel();
}

bool SarSensorModelAdapter::SaveState(ImageKeywordlist& image_kwl)
{
  if (m_SensorModel)
  {
    ossimKeywordlist geom;

    bool success = m_SensorModel->saveState(geom);

    if (success)
    {
      image_kwl.Clear();
      image_kwl.SetKeywordlist(geom);
    }
    return success;
  }

  return false;
}

bool SarSensorModelAdapter::IsValidSensorModel() const
{
  return m_SensorModel != nullptr;
}

bool SarSensorModelAdapter::Deburst(std::vector<std::pair<unsigned long, unsigned long>>& lines, std::pair<unsigned long, unsigned long>& samples,
                                    bool onlyValidSample)
{
  if (m_SensorModel)
  {
    return m_SensorModel->deburst(lines, samples, onlyValidSample);
  }

  return false;
}

bool SarSensorModelAdapter::BurstExtraction(const unsigned int burst_index, std::pair<unsigned long, unsigned long>& lines,
                                            std::pair<unsigned long, unsigned long>& samples, bool allPixels)
{
  if (m_SensorModel)
  {
    return m_SensorModel->burstExtraction(burst_index, lines, samples, allPixels);
  }

  return false;
}

bool SarSensorModelAdapter::DeburstAndConcatenate(std::vector<std::pair<unsigned long, unsigned long>>& linesBursts,
                                                  std::vector<std::pair<unsigned long, unsigned long>>& samplesBursts, unsigned int& linesOffset,
                                                  unsigned int first_burstInd, bool inputWithInvalidPixels)
{
  if (m_SensorModel)
  {
    return m_SensorModel->deburstAndConcatenate(linesBursts, samplesBursts, linesOffset, first_burstInd, inputWithInvalidPixels);
  }

  return false;
}

bool SarSensorModelAdapter::Overlap(std::pair<unsigned long, unsigned long>& linesUp, std::pair<unsigned long, unsigned long>& linesLow,
                                    std::pair<unsigned long, unsigned long>& samplesUp, std::pair<unsigned long, unsigned long>& samplesLow,
                                    unsigned int burstIndUp, bool inputWithInvalidPixels)
{
  if (m_SensorModel)
  {
    return m_SensorModel->overlap(linesUp, linesLow, samplesUp, samplesLow, burstIndUp, inputWithInvalidPixels);
  }

  return false;
}

bool SarSensorModelAdapter::ImageLineToDeburstLine(const std::vector<std::pair<unsigned long, unsigned long>>& lines, unsigned long imageLine,
                                                   unsigned long& deburstLine)
{
  return ossimplugins::ossimSarSensorModel::imageLineToDeburstLine(lines, imageLine, deburstLine);
}

void SarSensorModelAdapter::DeburstLineToImageLine(const std::vector<std::pair<unsigned long, unsigned long>>& lines, unsigned long deburstLine,
                                                   unsigned long& imageLine)
{
  ossimplugins::ossimSarSensorModel::deburstLineToImageLine(lines, deburstLine, imageLine);
}

bool SarSensorModelAdapter::WorldToLineSampleYZ(const Point3DType& inGeoPoint, Point2DType& cr, Point2DType& yz) const
{
  if (!m_SensorModel)
  {
    return false;
  }

  ossimGpt inGpt;
  inGpt.lon = inGeoPoint[0];
  inGpt.lat = inGeoPoint[1];
  inGpt.hgt = inGeoPoint[2];

  ossimDpt outDpt;

  double y(0.), z(0.);
  m_SensorModel->worldToLineSampleYZ(inGpt, outDpt, y, z);

  if (outDpt.isNan())
    return false;

  cr[0] = outDpt.x;
  cr[1] = outDpt.y;
  yz[0] = y;
  yz[1] = z;

  return true;
}

bool SarSensorModelAdapter::WorldToLineSample(const Point3DType& inGeoPoint, Point2DType& cr) const
{
  if (!m_SensorModel)
  {
    return false;
  }

  ossimGpt inGpt;
  inGpt.lon = inGeoPoint[0];
  inGpt.lat = inGeoPoint[1];
  inGpt.hgt = inGeoPoint[2];

  ossimDpt outDpt;

  m_SensorModel->worldToLineSample(inGpt, outDpt);

  if (outDpt.isNan())
    return false;

  cr[0] = outDpt.x;
  cr[1] = outDpt.y;

  return true;
}

bool SarSensorModelAdapter::WorldToCartesian(const Point3DType& inGeoPoint, Point3DType& outCartesianPoint)
{
  ossimGpt inGpt;
  inGpt.lon = inGeoPoint[0];
  inGpt.lat = inGeoPoint[1];
  inGpt.hgt = inGeoPoint[2];

  ossimEcefPoint outCartesien(inGpt);

  if (outCartesien.isNan())
    return false;

  outCartesianPoint[0] = outCartesien.x();
  outCartesianPoint[1] = outCartesien.y();
  outCartesianPoint[2] = outCartesien.z();

  return true;
}

bool SarSensorModelAdapter::WorldToSatPositionAndVelocity(const Point3DType& inGeoPoint, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const
{
  if (!m_SensorModel)
  {
    return false;
  }

  ossimGpt inGpt;
  inGpt.lon = inGeoPoint[0];
  inGpt.lat = inGeoPoint[1];
  inGpt.hgt = inGeoPoint[2];

  ossimplugins::ossimSarSensorModel::TimeType azimuthTime;
  double                                      rangeTime;
  ossimEcefPoint                              sensorPos;
  ossimEcefVector                             sensorVel;

  const bool success = m_SensorModel->worldToAzimuthRangeTime(inGpt, azimuthTime, rangeTime, sensorPos, sensorVel);

  if (sensorPos.isNan() || !success)
    return false;

  satellitePosition[0] = sensorPos.x();
  satellitePosition[1] = sensorPos.y();
  satellitePosition[2] = sensorPos.z();

  satelliteVelocity[0] = sensorVel.x();
  satelliteVelocity[1] = sensorVel.y();
  satelliteVelocity[2] = sensorVel.z();

  return true;
}

bool SarSensorModelAdapter::LineToSatPositionAndVelocity(const double line, Point3DType& satellitePosition, Point3DType& satelliteVelocity) const
{
  if (!m_SensorModel)
  {
    return false;
  }

  ossimplugins::ossimSarSensorModel::TimeType azimuthTime;
  ossimEcefPoint                              sensorPos;
  ossimEcefVector                             sensorVel;

  m_SensorModel->lineToAzimuthTime(line, azimuthTime);
  m_SensorModel->interpolateSensorPosVel(azimuthTime, sensorPos, sensorVel);

  if (sensorPos.isNan() || sensorVel.isNan())
    return false;

  satellitePosition[0] = sensorPos.x();
  satellitePosition[1] = sensorPos.y();
  satellitePosition[2] = sensorPos.z();

  satelliteVelocity[0] = sensorVel.x();
  satelliteVelocity[1] = sensorVel.y();
  satelliteVelocity[2] = sensorVel.z();

  return true;
}


} // namespace otb
