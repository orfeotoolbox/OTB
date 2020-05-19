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

#include "mvdOTBApplicationsModel.h"


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

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplicationsBrowser.h"
#include "mvdApplicationLauncher.h"

namespace mvd
{
/*
  TRANSLATOR mvd::OTBApplicationsModel

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
OTBApplicationsModel::OTBApplicationsModel(QObject* p) : AbstractModel(p), m_ApplicationsBrowser(NULL), m_ApplicationLauncher(NULL)
{
  //
  // instantiate apps browser
  m_ApplicationsBrowser = new ApplicationsBrowser(this);

  //
  // instantiate app launcher
  m_ApplicationLauncher = new ApplicationLauncher(this);

  // initialize the app browser with empty string. The default
  // behavior use the env ITK_AUTOLOAD_PATH
  // m_ApplicationsBrowser->SetAutoLoadPath("");
}

/*******************************************************************************/
OTBApplicationsModel::~OTBApplicationsModel()
{
}

/*******************************************************************************/
void OTBApplicationsModel::virtual_BuildModel(void*)
{
  // nothing to do
}

/*******************************************************************************/
void OTBApplicationsModel::FillTree()
{
  //
  // this method is connected to the widget via a signal. In
  // ApplicationBrowser, when the map is filled with tags/Apps, a
  // signal is emitted and received by the widget to fill its tree.
  m_ApplicationsBrowser->SearchAvailableApplicationsTags();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
