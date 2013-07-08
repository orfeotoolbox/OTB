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
#include "ApplicationsWrapper/mvdApplicationsToolBoxController.h"


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
#include "Core/mvdTypes.h"
//
#include "Gui/mvdApplicationsToolBox.h"
//
#include "ApplicationsWrapper/mvdApplicationLauncher.h"
#include "ApplicationsWrapper/mvdApplicationsBrowser.h"
#include "ApplicationsWrapper/mvdOTBApplicationsModel.h"

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
ApplicationsToolBoxController
::ApplicationsToolBoxController( ApplicationsToolBox* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
ApplicationsToolBoxController
::~ApplicationsToolBoxController()
{
}

/*******************************************************************************/
void
ApplicationsToolBoxController
::Connect( AbstractModel* model )
{
  //
  // Access widget
  ApplicationsToolBox* widget = GetWidget< ApplicationsToolBox >();

  //
  // Access model.
  OTBApplicationsModel* appModel = qobject_cast< OTBApplicationsModel* >(model);
  assert( appModel!=NULL );

  //
  // connections
  QObject::connect(appModel->GetApplicationsBrowser(), 
                   SIGNAL( AvailableApplicationsTagsChanged(const ApplicationsTagContainer&, 
                                                            const ApplicationDocNameToNameMap& ) ),
                   widget,
                   SLOT( OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer&, 
                                                            const ApplicationDocNameToNameMap& ) ) );
}

/*******************************************************************************/
void
ApplicationsToolBoxController
::Disconnect( AbstractModel* model )
{
  //
  // Access widget
  ApplicationsToolBox* widget = GetWidget< ApplicationsToolBox >();

  //
  // Access model.
  OTBApplicationsModel* appModel = qobject_cast< OTBApplicationsModel* >(model);
  assert( appModel!=NULL );

  //
  // disconnect app browser
  QObject::disconnect(appModel->GetApplicationsBrowser(),
                      SIGNAL( AvailableApplicationsTagsChanged(const ApplicationsTagContainer&, 
                                                               const ApplicationDocNameToNameMap& ) ),
                      widget,
                      SLOT( OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer&, 
                                                               const ApplicationDocNameToNameMap& ) )
    );
}

/*******************************************************************************/
void
ApplicationsToolBoxController
::ResetWidget()
{
  // Reset widget.
  ResetDatasetTree();

  // NOTE : no signal to emit here. Done in the class ApplicationsBrowser
  // aggregated in the model
}

/*******************************************************************************/
void
ApplicationsToolBoxController
::ResetDatasetTree()
{
  //
  // Access model.
  OTBApplicationsModel* model = GetModel< OTBApplicationsModel >();
  assert( model!=NULL );

  // 
  // Fill the widget. It also reset the datatree before filling the
  // tree...
  model->FillTree();
}

/*******************************************************************************/
/*
QWidget *
ApplicationsToolBoxController
::GetSelectedApplicationWidget( const QString& appName )
{
#if 0
  //
  // Access model.
  OTBApplicationsModel* model = GetModel< OTBApplicationsModel >();
  assert( model!=NULL );
#endif

  return ApplicationLauncher::NewOtbApplicationWidget( appName );
}
*/

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
