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

#include "mvdMaplaApplication.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperApplicationRegistry.h"

//
// Monteverdi includes (sorted by alphabetic order)

//
#ifdef OTB_USE_QT
#include "mvdOTBApplicationsModel.h"
#endif

//
// Class implementation.
namespace mvd
{

/*
  TRANSLATOR mvd::MaplaApplication

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
MaplaApplication::MaplaApplication(QApplication* qtApp) : I18nApplication(qtApp)
{
}

/*******************************************************************************/
MaplaApplication::~MaplaApplication()
{
  otb::Wrapper::ApplicationRegistry::CleanRegistry();
}

/*******************************************************************************/
void MaplaApplication::virtual_InitializeCore()
{
  I18nApplication::virtual_InitializeCore();

  setObjectName("MaplaApplication");

  InitializeCore("Monteverdi Application Launcher", QString("OTB %1").arg(OTB_VERSION_STRING), "OrfeoToolBox", "orfeo-toolbox.org");
}

/*******************************************************************************/
void MaplaApplication::Foo()
{
  SetModel(new OTBApplicationsModel(this));
}

/*******************************************************************************/
/* SLOTS                                                                       */

} // end namespace 'mvd'
