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

#include "otbGeometricSarSensorModelAdapter.h"

#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/ossimGeometricSarSensorModel.h"
#include "ossim/projection/ossimProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/otb/JSDDateTime.h"
#pragma GCC diagnostic pop
#else
#include "ossim/ossimGeometricSarSensorModel.h"
#include "ossim/projection/ossimProjection.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/otb/JSDDateTime.h"
#endif


namespace otb {

GeometricSarSensorModelAdapter::GeometricSarSensorModelAdapter():
  m_SensorModel(ITK_NULLPTR)
{
}

GeometricSarSensorModelAdapter::~GeometricSarSensorModelAdapter()
{
  delete m_SensorModel;
}


void GeometricSarSensorModelAdapter::CreateSensorModel(const ImageKeywordlist& image_kwl)
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

ossimplugins::JSDDateTime GeometricSarSensorModelAdapter::getTime(double line)
{
  assert(m_SensorModel);
  return m_SensorModel->getTime(line);
}

void GeometricSarSensorModelAdapter::GetPlatformPositionAtLine(
  double line, std::vector<double>& position, std::vector<double>& speed)
{
  assert(m_SensorModel);
  m_SensorModel->getPlatformPositionAtLine(line, position, speed);
}

} // namespace otb
