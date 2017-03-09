/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbPlatformPositionAdapter.h"

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#include "ossim/ossimGeometricSarSensorModel.h"
#include "ossim/otb/PlatformPosition.h"
#include "ossim/projection/ossimProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"


namespace otb {

PlatformPositionAdapter::PlatformPositionAdapter():
  m_SensorModel(ITK_NULLPTR)
{
}

PlatformPositionAdapter::~PlatformPositionAdapter()
{
  if (m_SensorModel != ITK_NULLPTR)
    {
    delete m_SensorModel;
    m_SensorModel = ITK_NULLPTR;
    }
}


void PlatformPositionAdapter::CreateSensorModel(const ImageKeywordlist& image_kwl)
{
  ossimKeywordlist geom;

  image_kwl.convertToOSSIMKeywordlist(geom);
  otbMsgDevMacro(<< "CreateSensorModel()");
  otbMsgDevMacro(<< "* type: " << geom.find("type"));

  ossimProjection* sensor =  ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);

  m_SensorModel = dynamic_cast<ossimplugins::ossimGeometricSarSensorModel*>(sensor);
  if (!m_SensorModel)
  {
    otbMsgDevMacro(<< "CreateSensorModel() failed");
  }
}

void PlatformPositionAdapter::GetPlatformPosition(
    double line, std::vector<double>& position, std::vector<double>& speed)
{
  m_SensorModel->getPlatformPositionAtLine(line, position, speed);
}

void PlatformPositionAdapter::GetPlatformPositionAtTime(
    ossimplugins::JSDDateTime time, std::vector<double>& position, std::vector<double>& speed)
{
       m_SensorModel->get_platformPosition()->getPlatformPositionAtTime(time, position, speed);
}

} // namespace otb
