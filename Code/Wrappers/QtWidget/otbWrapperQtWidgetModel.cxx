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

//#include <stdio.h>
#include <time.h>

namespace otb
{
namespace Wrapper
{

QtWidgetModel::QtWidgetModel(Application* app)
 : m_Application(app)
{
  m_Application->Init();
  m_ProgressWindow = new QWidget();
}

QtWidgetModel::~QtWidgetModel()
{
}

void QtWidgetModel::NotifyUpdate()
{
  // Update the parameters
  m_Application->UpdateParameters();

  // Notify all
  this->NotifyAll("ParametersUpdated");
}

void QtWidgetModel::ExecuteAndWriteOutput()
{
  AppliThread * taskAppli = new AppliThread( m_Application );
  taskAppli->start();
}

}
}
