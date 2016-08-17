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
#include "mvdLayerStackController.h"


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
#include "mvdAbstractLayerModel.h"
#include "mvdFilenameInterface.h"
#include "mvdStackedLayerModel.h"
#include "mvdLayerStackItemModel.h"
#include "mvdLayerStackWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::LayerStackController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
LayerStackController
::LayerStackController( LayerStackWidget * widget, QObject * p ) :
  AbstractModelController( widget, p )
{
}

/*******************************************************************************/
LayerStackController
::~LayerStackController()
{
}

/*******************************************************************************/
void
LayerStackController
::ClearWidget()
{
}

/*******************************************************************************/
void
LayerStackController
::Connect( AbstractModel * model )
{
  assert( model==qobject_cast< StackedLayerModel * >( model ) );


  QObject::connect(
    model,
    SIGNAL( CurrentChanged( size_t ) ),
    // to:
    this,
    SLOT( OnStackedLayerCurrentChanged( size_t ) )
  );

  QObject::connect(
    model,
    SIGNAL( ContentChanged() ),
    // to:
    this,
    SLOT( OnStackedLayerContentChanged() )
  );

  QObject::connect(
    model,
    SIGNAL( ContentReset() ),
    // to:
    this,
    SLOT( OnStackedLayerContentReset() )
  );


  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  assert( widget->GetItemModel()!=NULL );

  widget->GetItemModel()->SetStack(
    qobject_cast< StackedLayerModel * >( model )
  );


  QObject::connect(
    widget,
    SIGNAL( SelectionChanged( int ) ),
    // to:
    this,
    SLOT( OnSelectionChanged( int ) )
  );

  QObject::connect(
    widget,
    SIGNAL( ProjectionButtonClicked() ),
    // to:
    this,
    SLOT( OnProjectionButtonClicked() )
  );  


  QObject::connect(
    widget,
    SIGNAL( TopButtonClicked() ),
    // to:
    model,
    SLOT( MoveCurrentToTop() )
  );

  QObject::connect(
    widget,
    SIGNAL( BottomButtonClicked() ),
    // to:
    model,
    SLOT( MoveCurrentToBottom() )
  );

  QObject::connect(
    widget,
    SIGNAL( UpButtonClicked() ),
    // to:
    model,
    SLOT( RaiseCurrent() )
  );

  QObject::connect(
    widget,
    SIGNAL( DownButtonClicked() ),
    // to:
    model,
    SLOT( LowerCurrent() )
  );

  QObject::connect(
    widget,
    SIGNAL( DeleteLayerRequested() ),
    // to:
    model,
    SLOT( DeleteCurrent() )
  );

  QObject::connect(
    widget,
    SIGNAL( DeleteAllLayersRequested() ),
    // to:
    model,
    SLOT( Clear() )
  );

  QObject::connect(
    widget,
    SIGNAL( CopyLayerRequested( const AbstractLayerModel * ) ),
    // to:
    this,
    SLOT( OnCopyLayerRequested( const AbstractLayerModel * ) )
  );

  QObject::connect(
    widget,
    SIGNAL( RotateLayersRequested( int ) ),
    // to:
    model,
    SLOT( RotateLayers( int ) )
  );

  QObject::connect(
    widget,
    SIGNAL( ApplyButtonClicked() ),
    // to:
    this,
    SIGNAL( ApplyAllRequested() )
  );
}

/*******************************************************************************/
void
LayerStackController
::Disconnect( AbstractModel * model )
{
  // assert( model==qobject_cast< StackedLayerModel * >( model ) );


  QObject::disconnect(
    model,
    SIGNAL( CurrentChanged( size_t ) ),
    // from:
    this,
    SLOT( OnStackedLayerCurrentChanged( size_t ) )
  );

  QObject::disconnect(
    model,
    SIGNAL( ContentChanged() ),
    // from:
    this,
    SLOT( OnStackedLayerContentChanged() )
  );

  QObject::disconnect(
    model,
    SIGNAL( ContentReset() ),
    // from:
    this,
    SLOT( OnStackedLayerContentReset() )
  );


  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  assert( widget->GetItemModel()!=NULL );

  widget->GetItemModel()->SetStack( NULL );


  QObject::disconnect(
    widget,
    SIGNAL( SelectionChanged( int ) ),
    // from:
    this,
    SLOT( OnSelectionChanged( int ) )
  );

  QObject::disconnect(
    widget,
    SIGNAL( ProjectionButtonClicked() ),
    // from:
    this,
    SLOT( OnProjectionButtonClicked() )
  );  


  QObject::disconnect(
    widget,
    SIGNAL( TopButtonClicked() ),
    // from:
    model,
    SLOT( MoveCurrentToTop() )
  );

  QObject::disconnect(
    widget,
    SIGNAL( BottomButtonClicked() ),
    // from:
    model,
    SLOT( MoveCurrentToBottom() )
  );

  QObject::disconnect(
    widget,
    SIGNAL( UpButtonClicked() ),
    // from:
    model,
    SLOT( RaiseCurrent() )
  );

  QObject::disconnect(
    widget,
    SIGNAL( DownButtonClicked() ),
    // from:
    model,
    SLOT( LowerCurrent() )
  );

  QObject::disconnect(
    widget,
    SIGNAL( DeleteLayerRequested() ),
    // from:
    model,
    SLOT( DeleteCurrent() )
  );

  QObject::disconnect(
    widget,
    SIGNAL( DeleteAllLayersRequested() ),
    // from:
    model,
    SLOT( Clear() )
  );

  QObject::disconnect(
    widget,
    SIGNAL( CopyLayerRequested( const AbstractLayerModel * ) ),
    // from:
    this,
    SLOT( OnCopyLayerRequested( const AbstractLayerModel * ) )
  );

  QObject::disconnect(
    widget,
    SIGNAL( RotateLayersRequested( int ) ),
    // from:
    model,
    SLOT( RotateLayers( int ) )
  );

  QObject::disconnect(
    widget,
    SIGNAL( ApplyButtonClicked() ),
    // to:
    this,
    SIGNAL( ApplyAllRequested() )
  );
}

/*******************************************************************************/
void
LayerStackController
::virtual_ResetWidget( bool )
{
}

/*******************************************************************************/
void
LayerStackController
::UpdateButtonsState()
{
  assert( GetModel()==GetModel< StackedLayerModel >() );
  StackedLayerModel * model = GetModel< StackedLayerModel >();
  assert( model!=NULL );

  assert( GetWidget()==GetWidget< LayerStackWidget >() );
  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  {
  size_t unk = 0;
  size_t gcs = 0;

  model->CountSRT( unk, gcs, gcs, gcs );
    
  widget->SetProjectionEnabled( unk==0 && !model->IsEmpty() );
  }

  widget->SetDeleteEnabled( !model->IsEmpty() );

  widget->SetReloadEnabled( !model->IsEmpty() );

  widget->SetMoveEnabled( model->GetCount()>1 );

  widget->SetApplyEnabled( model->GetCount()>1 );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
LayerStackController
::OnCopyLayerRequested( const AbstractLayerModel * layer )
{
  // qDebug() << this << "::OnCopyLayerRequested(" << layer << ")";

  assert( layer!=NULL );

  const FilenameInterface * interface =
    dynamic_cast< const FilenameInterface * >( layer );

  if( interface==NULL )
    return;

  assert( qApp!=NULL );
  assert( qApp->clipboard()!=NULL );
  assert( qApp->clipboard()->mimeData()!=NULL );

  QList< QUrl > urls;
  
  urls << QUrl::fromLocalFile( interface->GetFilename() );

  qDebug() << "URLs:" << urls;

  QMimeData * mimeData = new QMimeData();

  mimeData->setUrls( urls );
  mimeData->setText( interface->GetFilename() );

  qApp->clipboard()->setMimeData( mimeData );
}

/*******************************************************************************/
void
LayerStackController
::OnCurrentChanged( int index )
{
  // qDebug() << this << "::OnCurrentChanged(" << index << ")";

  assert( GetModel()==GetModel< StackedLayerModel >() );
  StackedLayerModel * model = GetModel< StackedLayerModel >();
  assert( model!=NULL );

  model->SetCurrent( index );
}

/*******************************************************************************/
void
LayerStackController
::OnProjectionButtonClicked()
{
  // qDebug() << this << "::OnProjectionButtonClicked()";

  assert( GetModel()==GetModel< StackedLayerModel >() );
  StackedLayerModel * model = GetModel< StackedLayerModel >();
  assert( model!=NULL );

  model->SetReference( model->GetCurrentIndex() );
}

/*******************************************************************************/
void
LayerStackController
::OnSelectionChanged( int index )
{
  // qDebug() << this << "::OnSelectionChanged(" << index << ")";

  assert( GetModel()==GetModel< StackedLayerModel >() );
  StackedLayerModel * model = GetModel< StackedLayerModel >();
  assert( model!=NULL );

  model->SetCurrent( index );
}

/*******************************************************************************/
void
LayerStackController
::OnStackedLayerCurrentChanged( size_t index )
{
  // qDebug() << this << "::OnStackedLayerCurrentChanged(" << index << ")";

  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  assert( widget->GetItemModel()!=NULL );

  bool signalsBlocked = widget->blockSignals( true );
  {
  widget->SetCurrent( index );
  }
  widget->blockSignals( signalsBlocked );
}

/*******************************************************************************/
void
LayerStackController
::OnStackedLayerContentChanged()
{
  // qDebug() << this << "::OnStackedLayerContentChanged()";

  UpdateButtonsState();
}

/*******************************************************************************/
void
LayerStackController
::OnStackedLayerContentReset()
{
  // qDebug() << this << "::OnStackedLayerContentChanged()";

  UpdateButtonsState();
}

} // end namespace 'mvd'
