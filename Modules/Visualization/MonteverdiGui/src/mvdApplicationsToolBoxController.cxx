/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
ApplicationsToolBoxController
::ApplicationsToolBoxController( ApplicationsToolBox* widget, QObject* p ) :
  AbstractModelController( widget, p )
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
  QObject::connect(
    appModel->GetBrowser(), 
    SIGNAL(
      AvailableApplicationsTagsChanged( const ApplicationsTagContainer&, 
                                        const ApplicationDocNameToNameMap& ) ),
    widget,
    SLOT(
      OnAvailableApplicationsTagsChanged( const ApplicationsTagContainer&, 
                                          const ApplicationDocNameToNameMap& ) )
  );
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
  QObject::disconnect(
    appModel->GetBrowser(),
    SIGNAL(
      AvailableApplicationsTagsChanged( const ApplicationsTagContainer&, 
                                        const ApplicationDocNameToNameMap& ) ),
    widget,
    SLOT(
      OnAvailableApplicationsTagsChanged( const ApplicationsTagContainer&, 
                                          const ApplicationDocNameToNameMap& ) )
    );
}

/*******************************************************************************/
void
ApplicationsToolBoxController
::ClearWidget()
{
}

/*******************************************************************************/
void
ApplicationsToolBoxController
::virtual_ResetWidget( bool )
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
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
