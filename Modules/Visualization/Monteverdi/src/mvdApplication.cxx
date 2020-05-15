/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "mvdApplication.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdMainWindow.h"
#include "mvdStackedLayerModel.h"

//
#ifdef OTB_USE_QT
#include "mvdOTBApplicationsModel.h"
#endif

#if USE_OTB_APPS
#include "otbWrapperApplicationRegistry.h"
#endif

//
// Class implementation.
namespace mvd
{

/*
  TRANSLATOR mvd::Application

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
Application::Application(QApplication* qtApp) : I18nApplication(qtApp), m_OTBApplicationsModel(NULL)
{
}

/*******************************************************************************/
Application::~Application()
{
#if USE_OTB_APPS
  otb::Wrapper::ApplicationRegistry::CleanRegistry();
#endif
}

/*******************************************************************************/
/*
CountType
Application
::OpenDatabase()
{
  DatabaseModel* databaseModel = new DatabaseModel( this );

  DatabaseModel::BuildContext context;
  databaseModel->BuildModel( &context );

  SetModel( databaseModel );

  return context.m_NbOutdatedDatasetModels;
}
*/

/*******************************************************************************/
/*
void
Application
::CloseDatabase()
{
  // Delete model and release resources.
  SetModel( NULL );
}
*/

/*******************************************************************************/
void Application::OpenApplicationsBrowser()
{
#ifdef OTB_USE_QT
  m_OTBApplicationsModel = new OTBApplicationsModel(this);

  m_OTBApplicationsModel->BuildModel();
#endif
}

/*******************************************************************************/
void Application::virtual_InitializeCore()
{
  I18nApplication::virtual_InitializeCore();

  setObjectName("Application");

  InitializeCore(PROJECT_NAME, QString("OTB %1").arg(OTB_VERSION_STRING), "OrfeoToolBox", "orfeo-toolbox.org");

  //
  // Create the OTBApplications model
  OpenApplicationsBrowser();
}

/*******************************************************************************/
/* SLOTS                                                                       */

} // end namespace 'mvd'
