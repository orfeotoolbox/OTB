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
  //m_Application->RegisterListener( this );
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
  std::cout<<"********************************* ExecuteAndWriteOutput launched *********************************"<<std::endl;
  AppliThread * taskAppli = new AppliThread( m_Application );
  taskAppli->start();
  //WatchThread * taskWatch = new WatchThread( m_Application, m_ProgressWindow );
  //taskWatch->start();

 std::cout<<"********************************* ExecuteAndWriteOutput reporting *********************************"<<std::endl;

   // Prepare the progressbar window 


  std::cout<<"********************************** end ExecuteAndWriteOutput thread"<<std::endl;
  //m_Application->ExecuteAndWriteOutput();
}

/** The Notify */
/*
void QtWidgetModel::Notify(const std::string & event)
{
  std::cout<<"Notifycation done"<<std::endl;
  if (event == "OutputsUpdated")
     {
     }
   else if(event == "Quit")
     {
     }
}
*/
}
}
