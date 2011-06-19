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
#include "otbWrapperQtWidgetModel.h"
#include "otbWrapperOutputImageParameter.h"

namespace otb
{
namespace Wrapper
{

QtWidgetModel::QtWidgetModel(Application* app)
 : m_Application(app)
{

  m_Application->Init();

}

QtWidgetModel::~QtWidgetModel()
{
}

void QtWidgetModel::NotifyUpdate()
{
  m_Application->UpdateParameters();
}

void QtWidgetModel::ExecuteAndWriteOutput()
{
  m_Application->ExecuteAndWriteOutput();
}

}
}
