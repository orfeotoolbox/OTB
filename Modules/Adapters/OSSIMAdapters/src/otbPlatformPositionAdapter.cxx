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
  delete m_SensorModel;
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
    const std::string clsname = sensor->RTTI_vinfo().getname();
    delete sensor;
    itkExceptionMacro(<< "Cannot create sensor model from geom of type `"<<clsname<<"`, which doesn't inherit `ossimGeometricSarSensorModel`.");
    }
}

void PlatformPositionAdapter::GetPlatformPosition(
    double line, std::vector<double>& position, std::vector<double>& speed)
{
  assert(m_SensorModel);
  m_SensorModel->getPlatformPositionAtLine(line, position, speed);
}

void PlatformPositionAdapter::GetPlatformPositionAtTime(
    ossimplugins::JSDDateTime time, std::vector<double>& position, std::vector<double>& speed)
{
  assert(m_SensorModel);
  m_SensorModel->get_platformPosition()->getPlatformPositionAtTime(time, position, speed);
}

} // namespace otb
