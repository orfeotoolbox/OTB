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
#include "mvdLayerStackWidget.h"
#include "ui_mvdLayerStackWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdGui.h"
#include "mvdLayerStackItemModel.h"

namespace mvd
{

/*
  TRANSLATOR mvd::LayerStackWidget

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
LayerStackWidget
::LayerStackWidget( QWidget* p, Qt::WindowFlags flags  ):
  QWidget( p, flags ),
  m_UI( new mvd::Ui::LayerStackWidget() )
{
  m_UI->setupUi( this );

  m_UI->reloadButton->setVisible( false );
  m_UI->treeView->setDragEnabled( true );

  {
  QItemSelectionModel * ism = m_UI->treeView->selectionModel();

  m_UI->treeView->setModel( new LayerStackItemModel( m_UI->treeView ) );

  delete ism;
  ism = NULL;
  }

  InstallEventFilter( this );

  QObject::connect(
    m_UI->treeView->selectionModel(),
    SIGNAL( currentRowChanged( const QModelIndex &,  const QModelIndex & ) ),
    // to:
    this,
    SLOT( OnCurrentRowChanged( const QModelIndex &, const QModelIndex & ) )
    );

  QObject::connect(
    m_UI->treeView->selectionModel(),
    SIGNAL( selectionChanged( const QItemSelection &,  const QItemSelection & ) ),
    // to:
    this,
    SLOT( OnSelectionChanged( const QItemSelection &, const QItemSelection & ) )
    );

  QObject::connect(
    m_UI->topButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( TopButtonClicked() )
  );

  QObject::connect(
    m_UI->bottomButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( BottomButtonClicked() )
  );

  QObject::connect(
    m_UI->upButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( UpButtonClicked() )
  );

  QObject::connect(
    m_UI->downButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( DownButtonClicked() )
  );

  QObject::connect(
    m_UI->deleteButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( DeleteLayerRequested() )
  );

  QObject::connect(
    m_UI->deleteAllButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( DeleteAllLayersRequested() )
  );

  QObject::connect(
    m_UI->projectionButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( ProjectionButtonClicked() )
  );

  QObject::connect(
    m_UI->applyButton,
    SIGNAL( clicked() ),
    // to:
    this,
    SIGNAL( ApplyButtonClicked() )
  );
}

/*******************************************************************************/
LayerStackWidget
::~LayerStackWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
bool
LayerStackWidget
::eventFilter( QObject * object, QEvent * e )
{
  assert( object==m_UI->treeView );
  assert( e!=NULL );

  if( object!=m_UI->treeView )
    return false;

  switch( e->type() )
    {
    //
    // KEY RELEASE
    case QEvent::KeyRelease :
    {
    QKeyEvent * keyEvent = dynamic_cast< QKeyEvent * >( e );
    assert( keyEvent!=NULL );

    switch( keyEvent->key() )
      {
      case Qt::Key_C:
	if( keyEvent->modifiers()==Qt::ControlModifier &&
	    m_UI->treeView->currentIndex().isValid() )
	  {
	  emit CopyLayerRequested(
	    LayerStackItemModel::GetLayer(
	      m_UI->treeView->currentIndex()
	    )
	  );

	  return true;
	  }
	break;
      //
      case Qt::Key_Delete:
	if( keyEvent->modifiers()==Qt::NoModifier )
	  {
	  emit DeleteLayerRequested();

	  return true;
	  }
	else if( keyEvent->modifiers()==Qt::ShiftModifier )
	  {
	  emit DeleteAllLayersRequested();

	  return true;
	  }
	break;
      }
    }
    break;
    //
    // MOUSE-WHEEL
    case QEvent::Wheel :
    {
    QWheelEvent * wEvent = dynamic_cast< QWheelEvent * >( e );
    assert( wEvent!=NULL );

    if( wEvent->modifiers()==Qt::ControlModifier )
      {
      emit RotateLayersRequested(
    	wEvent->delta() / (MOUSE_WHEEL_STEP_FACTOR * MOUSE_WHEEL_STEP_DEGREES)
      );

      return true;
      }
    }
    break;
    //
    // other.
    default:
      break;
    }

  return false;
}

/*******************************************************************************/
const LayerStackItemModel *
LayerStackWidget
::GetItemModel() const
{
  return const_cast< LayerStackWidget * >( this )->GetItemModel();
}

/*******************************************************************************/
LayerStackItemModel *
LayerStackWidget
::GetItemModel()
{
  assert(
    m_UI->treeView->model()==
    qobject_cast< LayerStackItemModel * >( m_UI->treeView->model() )
    );

  return qobject_cast< LayerStackItemModel * >( m_UI->treeView->model() );
}

/*******************************************************************************/
void
LayerStackWidget
::InstallEventFilter( QObject * filter )
{
  assert( m_UI!=NULL );
  assert( m_UI->treeView!=NULL );

  m_UI->treeView->installEventFilter( filter );
}

/*******************************************************************************/
// void
// LayerStackWidget
// ::SetModel( LayerStackItemModel * itemModel )
// {
//   // See http://qt-project.org/doc/qt-4.8/qabstractitemview.html#setModel .
//   QItemSelectionModel * itemSelectionModel = m_UI->treeView->selectionModel();

//   m_UI->treeView->setModel( itemModel );

//   itemModel->setParent( m_UI->treeView );

//   delete itemSelectionModel;
//   itemSelectionModel = NULL;
// }

/*******************************************************************************/
void
LayerStackWidget
::SetApplyEnabled( bool enabled )
{
  assert( m_UI->applyButton!=NULL );

  m_UI->applyButton->setEnabled( enabled );
}

/*******************************************************************************/
void
LayerStackWidget
::SetCurrent( int row )
{
  assert( m_UI->treeView->selectionModel()!=NULL );

  // if( m_UI->treeView->selectionModel()->currentIndex().row()==row )
  //   return;

  if( row<0 )
    m_UI->treeView->selectionModel()->clearSelection();

  else
    m_UI->treeView->selectionModel()->select(
      m_UI->treeView->model()->index( row, 1 ),
      QItemSelectionModel::ClearAndSelect |
      QItemSelectionModel::Rows
  );

  /*
  m_UI->treeView->selectionModel()->setCurrentIndex(
    m_UI->treeView->model()->index( row, 1 ),
    QItemSelectionModel::ClearAndSelect |
    // QItemSelectionModel::Current |
    QItemSelectionModel::Rows
  );
  */
}

/*******************************************************************************/
void
LayerStackWidget
::SetDeleteEnabled( bool enabled )
{
  assert( m_UI!=NULL );

  assert( m_UI->deleteButton!=NULL );

  m_UI->deleteButton->setEnabled( enabled );
}

/*******************************************************************************/
void
LayerStackWidget
::SetMoveEnabled( bool enabled )
{
  assert( m_UI!=NULL );

  assert( m_UI->upButton!=NULL );
  assert( m_UI->downButton!=NULL );
  assert( m_UI->topButton!=NULL );
  assert( m_UI->bottomButton!=NULL );

  m_UI->upButton->setEnabled( enabled );
  m_UI->downButton->setEnabled( enabled );
  m_UI->topButton->setEnabled( enabled );
  m_UI->bottomButton->setEnabled( enabled );
}

/*******************************************************************************/
void
LayerStackWidget
::SetProjectionEnabled( bool enabled )
{
  assert( m_UI!=NULL );

  assert( m_UI->projectionButton!=NULL );

  m_UI->projectionButton->setEnabled( enabled );
}

/*******************************************************************************/
void
LayerStackWidget
::SetReloadEnabled( bool enabled )
{
  assert( m_UI!=NULL );

  assert( m_UI->reloadButton!=NULL );

  m_UI->reloadButton->setEnabled( enabled );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
LayerStackWidget
::OnCurrentRowChanged( const QModelIndex & current,
                       const QModelIndex & )
{
  // qDebug()
  //   << this
  //   << "::OnCurrentRowChange(" << current.row() << "," << previous.row() << ")";

  emit CurrentChanged( current.row() );
}

/*******************************************************************************/
void
LayerStackWidget
::OnSelectionChanged( const QItemSelection & selected,
                      const QItemSelection & )
{
  // qDebug()
  //   << this
  //   << "::OnSelectionChanged(" << selected << "," << deselected << ")";

  QModelIndexList indexes( selected.indexes() );
  // assert( indexes.empty() || indexes.size()==1 );

  emit SelectionChanged(
    indexes.empty()
    ? -1
    : indexes.front().row()
  );
}

} // end namespace 'mvd'
