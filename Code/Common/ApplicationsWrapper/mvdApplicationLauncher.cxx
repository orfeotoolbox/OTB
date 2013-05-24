/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "ApplicationsWrapper/mvdApplicationLauncher.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperApplication.h"
 
//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"
#include "ApplicationsWrapper/mvdWrapperQtWidgetView.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ApplicationLauncher

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ApplicationLauncher
::ApplicationLauncher( QObject* parent ) :
  QObject( parent )
{
}

/*******************************************************************************/
ApplicationLauncher
::~ApplicationLauncher()
{
}

/*******************************************************************************/
QWidget* 
ApplicationLauncher
::GetSelectedApplicationWidget(const QString& appname)
{
  // Create module
  otb::Wrapper::Application::Pointer app 
    = otb::Wrapper::ApplicationRegistry::CreateApplication( ToStdString (appname) );
  
  if ( app.IsNull() )
    {
    std::cerr << "Could not Load application " << ToStdString( appname ) << std::endl;
    }
  else
    {
    QSettings settings;

    // Search for elev parameters
    std::vector<std::string> parameters = app->GetParametersKeys();

    for(std::vector<std::string>::const_iterator it = parameters.begin();
        it!=parameters.end();++it)
      {
      std::size_t lastDot = (it->find_last_of('.'));
      
      if(lastDot != std::string::npos)
        {
        std::string lastKey = it->substr(lastDot+1,it->size()-lastDot-1);

        if(lastKey == "dem" && settings.contains("srtmDirActive") && settings.value("srtmDirActive").toBool())
          {
          app->EnableParameter(*it);
          app->SetParameterString(*it,settings.value("srtmDir").toString().toStdString());
          }
        if(lastKey == "geoid" && settings.contains("geoidPathActive") && settings.value("geoidPathActive").toBool())
          {
          app->EnableParameter(*it);
          app->SetParameterString(*it,settings.value("geoidPath").toString().toStdString());
          }
        }
      }

    // Create GUI based on module
    Wrapper::QtWidgetView* gui = new Wrapper::QtWidgetView(app);
    gui->CreateGui();

    return gui;
    }

  return NULL;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void 
ApplicationLauncher
::OnApplicationToLaunchSelected(const QString& appname)
{
  QWidget * gui = GetSelectedApplicationWidget( appname );
  
  if (gui)
    {
    // MainWidget : that contains the view and any other widget
    // (progress, logs...)
    QMainWindow* mainWindow =  new QMainWindow();
    mainWindow->setWindowIcon(QIcon( ":/otb_small.png" ));
    //mainWindow->setWindowTitle(QString(gui->->GetDocName()).append(" - ").append(OTB_VERSION_STRING));
 
    // build the main window, central widget is the plugin view, other
    // are docked widget (progress, logs...)
    mainWindow->setCentralWidget( gui );

    // Connect the View "Quit" signal, to the mainWindow close slot
    QObject::connect(gui, SIGNAL(QuitSignal()), mainWindow, SLOT(close()));
  
    // Show the main window
    mainWindow->show();
    }
}

} // end namespace 'mvd'
