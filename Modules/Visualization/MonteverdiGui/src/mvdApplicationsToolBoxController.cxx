/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "mvdApplicationsToolBoxController.h"


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
//
#include "mvdTypes.h"
//
#include "mvdApplicationsToolBox.h"
//
#include "mvdApplicationLauncher.h"
#include "mvdApplicationsBrowser.h"
#include "mvdOTBApplicationsModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ApplicationsToolBoxController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ApplicationsToolBoxController::ApplicationsToolBoxController(ApplicationsToolBox* widget, QObject* p) : AbstractModelController(widget, p)
{
}

/*******************************************************************************/
ApplicationsToolBoxController::~ApplicationsToolBoxController()
{
}

/*******************************************************************************/
void ApplicationsToolBoxController::Connect(AbstractModel* model)
{
  //
  // Access widget
  ApplicationsToolBox* widget = GetWidget<ApplicationsToolBox>();

  //
  // Access model.
  OTBApplicationsModel* appModel = qobject_cast<OTBApplicationsModel*>(model);
  assert(appModel != NULL);

  //
  // connections
  QObject::connect(appModel->GetBrowser(), SIGNAL(AvailableApplicationsTagsChanged(const ApplicationsTagContainer&)), widget,
                   SLOT(OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer&)));
}

/*******************************************************************************/
void ApplicationsToolBoxController::Disconnect(AbstractModel* model)
{
  //
  // Access widget
  ApplicationsToolBox* widget = GetWidget<ApplicationsToolBox>();

  //
  // Access model.
  OTBApplicationsModel* appModel = qobject_cast<OTBApplicationsModel*>(model);
  assert(appModel != NULL);

  //
  // disconnect app browser
  QObject::disconnect(appModel->GetBrowser(), SIGNAL(AvailableApplicationsTagsChanged(const ApplicationsTagContainer&)), widget,
                      SLOT(OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer&)));
}

/*******************************************************************************/
void ApplicationsToolBoxController::ClearWidget()
{
}

/*******************************************************************************/
void ApplicationsToolBoxController::virtual_ResetWidget(bool)
{
  // Reset widget.
  ResetDatasetTree();

  // NOTE : no signal to emit here. Done in the class ApplicationsBrowser
  // aggregated in the model
}

/*******************************************************************************/
void ApplicationsToolBoxController::ResetDatasetTree()
{
  //
  // Access model.
  OTBApplicationsModel* model = GetModel<OTBApplicationsModel>();
  assert(model != NULL);

  //
  // Fill the widget. It also reset the datatree before filling the
  // tree...
  model->FillTree();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
