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
#include "Gui/mvdDatabaseBrowserController.h"


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
#include "Core/mvdDatabaseModel.h"
//
#include "Gui/mvdDatabaseBrowserWidget.h"

#include "Core/mvdVectorImageModel.h"
//
#include "Gui/mvdDatabaseTreeWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::DatabaseBrowserController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatabaseBrowserController
::DatabaseBrowserController( DatabaseBrowserWidget* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
DatabaseBrowserController
::~DatabaseBrowserController()
{
}

/*******************************************************************************/
void
DatabaseBrowserController
::Connect( AbstractModel* model )
{
  DatabaseBrowserWidget* widget = GetWidget< DatabaseBrowserWidget >();

  QObject::connect(
    widget, SIGNAL( CurrentDatasetChanged( const QString& )  ),
    // to:
    this, SLOT( OnCurrentDatasetChanged( const QString& ) )
  );

  //
  QObject::connect(
    this, 
    SIGNAL( SelectedDatasetFilenameChanged( const QString& )  ),
    // to:
    widget->GetDatabaseTreeWidget(), 
    SLOT( OnSelectedDatasetFilenameChanged( const QString& ) )
    );
}

/*******************************************************************************/
void
DatabaseBrowserController
::Disconnect( AbstractModel* model )
{
  DatabaseBrowserWidget* widget = GetWidget< DatabaseBrowserWidget >();

  QObject::disconnect(
    widget, SIGNAL( CurrentDatasetChanged( const QString& )  ),
    // from:
    this, SLOT( OnCurrentDatasetChanged( const QString& ) )
  );
  
  //
  QObject::disconnect(
    this, 
    SIGNAL( SelectedDatasetFilenameChanged( const QString& )  ),
    // to:
    widget->GetDatabaseTreeWidget(), 
    SLOT( OnSelectedDatasetFilenameChanged( const QString& ) )
    );
}

/*******************************************************************************/
void
DatabaseBrowserController
::ResetWidget()
{
  qDebug() << this << "::ResetWidget()";

  // Reset widget.
  ResetDatasetTree();

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
DatabaseBrowserController
::ResetDatasetTree()
{
  //
  // Access widget.
  DatabaseBrowserWidget* widget = GetWidget< DatabaseBrowserWidget >();

  //
  // Access model.
  DatabaseModel* model = GetModel< DatabaseModel >();
  assert( model!=NULL );

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
  // Block widget's signals...
  //...but force call to valueChanged() slot to force refresh.
  widget->blockSignals( true );
  {
  // TODO: Fill in widget.
  widget->SetDatasetList( model->QueryDatasetModels() );
  }
  widget->blockSignals( false );
  }
  this->blockSignals( false );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseBrowserController
::OnCurrentDatasetChanged( const QString& name )
{
  if( name.isEmpty() )
    return;

  DatabaseModel* model = GetModel< DatabaseModel >();

  // set the newly selected dataset id
  model->SelectDatasetModel( name );

  // emit selected dataset image filename
  VectorImageModel * imageModel
    =  qobject_cast< VectorImageModel *>(model->GetSelectedDatasetModel()->GetSelectedImageModel());

  //
  // this signal is used to pass the current dataset input filename.
  // it is connected to the DatabaseBrowserWidget custom QTreeWidget
  emit SelectedDatasetFilenameChanged(imageModel->GetFilename());
}

} // end namespace 'mvd'
