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

#include "otbPlatformPositionAdapter.h"

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#include "ossimGeometricSarSensorModel.h"
#include "projection/ossimProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"

namespace otb {

PlatformPositionAdapter::PlatformPositionAdapter():
  m_SensorModel(NULL)
{
}

PlatformPositionAdapter::~PlatformPositionAdapter()
{
  if (m_SensorModel != NULL)
    {
    delete m_SensorModel;
    m_SensorModel = NULL;
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

} // namespace otb
