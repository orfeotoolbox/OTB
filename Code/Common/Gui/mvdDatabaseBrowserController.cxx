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
#include "mvdDatabaseBrowserController.h"


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
//
#include "Gui/mvdDatasetTreeWidgetItem.h"

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
    this,
    SLOT( OnCurrentDatasetChanged( const QString& ) )
  );

  QObject::connect(
    model,
    SIGNAL( SelectedDatasetModelChanged( DatasetModel* ) ),
    // to:
    this,
    SLOT( OnSelectedDatasetModelChanged( DatasetModel* ) )
  );

  //
  QObject::connect(
    this, 
    SIGNAL( SelectedDatasetFilenameChanged( const QString& )  ),
    // to:
    widget->GetDatabaseTreeWidget(), 
    SLOT( OnSelectedDatasetFilenameChanged( const QString& ) )
    );

  //
  QObject::connect(
    widget->GetDatabaseTreeWidget(), 
    SIGNAL( DatasetToDeleteSelected( const QString& ) ),
    // to:
    this,
    SLOT( OnDatasetToDeleteSelected( const QString& ) )
    );

  //
  QObject::connect(
    widget->GetDatabaseTreeWidget(), 
    SIGNAL( DatasetRenamed( const QString &, const QString & )  ),
    // to:
    model,
    SLOT( OnDatasetRenamed(const QString&, const QString & ) )
    );

  //
  QObject::connect(
    model,
    SIGNAL( DatabaseChanged() ),
    this,
    SLOT( RefreshWidget() )
    );

  //
  QObject::connect(
    widget->GetDatabaseTreeWidget(),
    SIGNAL( ImageDropped(const QString &) ),
    this,
    SLOT( OnImageDropped(const QString &) )
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

  //
  QObject::disconnect(
    widget->GetDatabaseTreeWidget(), 
    SIGNAL( DatasetToDeleteSelected( const QString & )  ),
    // to:
    this, 
    SLOT( OnDatasetToDeleteSelected(const QString& ) )
    );
}

/*******************************************************************************/
void
DatabaseBrowserController
::ClearWidget()
{
  ResetDatasetTree( StringPairListType() );
}

/*******************************************************************************/
void
DatabaseBrowserController
::ResetWidget()
{
#if 0
  //
  // Access model.
  DatabaseModel* model = GetModel< DatabaseModel >();
  assert( model!=NULL );

  ResetDatasetTree( model->QueryDatasetModels() );

#else
  RefreshWidget();

#endif
}

/*******************************************************************************/
void
DatabaseBrowserController
::ResetDatasetTree( const StringPairListType& datasets )
{
  //
  // Access widget.
  DatabaseBrowserWidget* widget = GetWidget< DatabaseBrowserWidget >();

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
  widget->SetDatasetList( datasets );
  }
  widget->blockSignals( false );
  }
  this->blockSignals( false );
}

/*******************************************************************************/
void
DatabaseBrowserController
::CheckDatasetsConsistency()
{
  //
  // access widget.
  DatabaseBrowserWidget* widget = GetWidget< DatabaseBrowserWidget >();

  //
  // access model.
  const DatabaseModel* model = GetModel< DatabaseModel >();
  assert( model!=NULL );

  // get the tree
  QTreeWidget * tree = widget->GetDatabaseTreeWidget();

  // iterate on the dataset list, check consistency.
  // if not consistent add icon and disable QWidgetItem.
  for ( int topIdx = 0; topIdx < tree->topLevelItemCount(); topIdx++)
    {
    int nbChild = tree->topLevelItem(topIdx)->childCount();
    for ( int idx = 0; idx < nbChild; idx++ )
      {      
      DatasetTreeWidgetItem * currentDatasetItem = 
        dynamic_cast<DatasetTreeWidgetItem*>(
          tree->topLevelItem(topIdx)->child(idx)
          );
      QString datasetId =  currentDatasetItem->GetId();

      const DatasetModel* datasetModel = model->FindDatasetModel( datasetId );
      assert( datasetModel!=NULL );

      // check consistency
      if ( !datasetModel->IsConsistent() )
        {
        // disable inconsistent dataset
        currentDatasetItem->setDisabled(true);

        // add forbidden icon
        currentDatasetItem->setIcon(0, QIcon( ":/icons/forbidden" ));

        // add tootlip
        currentDatasetItem->setToolTip (0,tr ("Inconsistent Dataset disabled") );
        }
      }
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseBrowserController
::OnCurrentDatasetChanged( const QString& id )
{
  // qDebug() << this << "::OnCurrentDatasetChanged(" << id << ")";

  if( id.isEmpty() )
    return;

  DatabaseModel* model = GetModel< DatabaseModel >();

  // set the newly selected dataset id
  model->SelectDatasetModel( id );

  // emit selected dataset image filename
  VectorImageModel * imageModel
    =  qobject_cast< VectorImageModel *>(model->GetSelectedDatasetModel()->GetSelectedImageModel());

  //
  // this signal is used to pass the current dataset input filename.
  // it is connected to the DatabaseBrowserWidget custom QTreeWidget
  emit SelectedDatasetFilenameChanged(imageModel->GetFilename());
}

/*******************************************************************************/
void
DatabaseBrowserController
::RefreshWidget()
{
  DatabaseModel* model = GetModel< DatabaseModel >();
  assert( model!=NULL );

  // Reset widget.
  ResetDatasetTree( model->QueryDatasetModels() );

  // Check every dataset consistency  
  CheckDatasetsConsistency();
}

/*******************************************************************************/
void
DatabaseBrowserController
::OnSelectedDatasetModelChanged( DatasetModel* datasetModel )
{
  if( datasetModel==NULL )
    return;

  assert( GetWidget()!=NULL );
  assert( GetWidget()==GetWidget< DatabaseBrowserWidget >() );

  DatabaseBrowserWidget* widget = GetWidget< DatabaseBrowserWidget >();

  //
  // Block widget's signals here to avoid recursive
  // SelectedDatasetModelChanged(), CurrentDatasetModelChanged()
  // loop.
  bool areSignalsBlocked = widget->blockSignals( true );
  {
  widget->SetCurrentDataset( datasetModel->GetName() );
  }
  widget->blockSignals( areSignalsBlocked );
}

/*******************************************************************************/
void
DatabaseBrowserController
::OnImageDropped( const QString & imagefname )
{
  emit ImageToImportDropped( imagefname );
}

/*******************************************************************************/
void
DatabaseBrowserController
::OnDatasetToDeleteSelected( const QString& id )
{
  // Access database-model.
  assert( GetModel()==GetModel< DatabaseModel >() );
  DatabaseModel* databaseModel = GetModel< DatabaseModel >();
  assert( databaseModel!=NULL );

  // Access selected dataset-model (candidate to deletion).
  DatasetModel* datasetModel = databaseModel->FindDatasetModel( id );
  assert( datasetModel!=NULL );

  // Pop confirm delete dialog.
  QMessageBox::StandardButton button = QMessageBox::warning(
    GetWidget(),
    tr( "Warning!" ),
    tr( "Are you sure you want to delete dataset '%1'?" )
    .arg( datasetModel->GetAlias() ),
    QMessageBox::Yes | QMessageBox::No
  );

  if( button==QMessageBox::No )
    return;

  databaseModel->RemoveDatasetModel( id );
}

} // end namespace 'mvd'
