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
#include "Gui/mvdDatabaseTreeWidget.h"
//#include "Gui/ui_mvdDatabaseTreeWidget.h"


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
#include "Core/mvdAlgorithm.h"

namespace mvd
{

/*
  TRANSLATOR mvd::DatabaseTreeWidget

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
DatabaseTreeWidget
::DatabaseTreeWidget( QWidget* parent  ):
  QTreeWidget( parent),
  m_DatasetFilename(""),
  m_EditionActive(false)
{
  //setMouseTracking(true);
  setDragEnabled(true);

  // setup contextual menu
  InitializeContextualMenu();

  //
  // do some connection
  QObject::connect(this,
                   SIGNAL( itemChanged( QTreeWidgetItem * , int ) ),
                   SLOT( OnItemChanged( QTreeWidgetItem* , int ) )
    );
}

/*******************************************************************************/
DatabaseTreeWidget
::~DatabaseTreeWidget()
{
}

/*******************************************************************************/
void
DatabaseTreeWidget
::InitializeContextualMenu()
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  
  QObject::connect(this,
                   SIGNAL(customContextMenuRequested(const QPoint&)),
                   SLOT(OnCustomContextMenuRequested(const QPoint&))
    );  
}

/*******************************************************************************/
void 
DatabaseTreeWidget::mouseMoveEvent( QMouseEvent * event )
{
  if ( !(event->buttons() &  Qt::LeftButton ))
    return;
  
  //start Drag ?
  int distance = ( event->pos() - m_StartDragPosition ).manhattanLength();
  if (distance < QApplication::startDragDistance())
    return;
    

  //Get current selection
  QTreeWidgetItem *selectedItem = currentItem();

  if ( selectedItem && selectedItem->parent() )
    {
    //TODO : get the image filename  of the selected dataset
    QByteArray itemData( ToStdString (m_DatasetFilename ).c_str() );
 
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);

    //Create drag
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
}

/*******************************************************************************/
void
DatabaseTreeWidget::mousePressEvent(QMouseEvent *event)
{   
  if (event->button() == Qt::LeftButton)
     {
     //
     // superclass event handling
     QTreeWidget::mousePressEvent(event);
     
     // remember start drag position
     m_StartDragPosition = event->pos();
     }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseTreeWidget::OnSelectedDatasetFilenameChanged(const QString& filename)
{
  //
  // update the dataset image filename to be used in the dragged mime data
  m_DatasetFilename = filename;
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnDeleteTriggered( const QString & id)
{
  qDebug() << this << " ::OnDeleteTriggered ("<<id<<")";

  emit DatasetToDeleteSelected( id );
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnRenameTriggered()
{
  // find the QTreeWidgetItem and make it editable
  m_ItemToEdit = itemAt( m_ContextualMenuClickedPosition );
  m_PreviousItemText = m_ItemToEdit->text(0);
  m_DefaultItemFlags  = m_ItemToEdit->flags(); 
  m_ItemToEdit->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

  // edit item
  // inspired from here:
  // http://www.qtcentre.org/archive/index.php/t-45857.html?s=ad4e7c45bbd9fd4bf5cc32853dd3fe82  
  m_EditionActive = true;
  editItem(m_ItemToEdit, 0);
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnItemChanged( QTreeWidgetItem* item , int column)
{
  if (m_EditionActive )
    {
    if (!item->text(column).isEmpty() )
      {
      // send a signal with the previous name and the new EditedName
      emit DatasetRenamed(m_PreviousItemText, item->text(column)  );
      }
    else
      {
      m_ItemToEdit->setText( column, m_PreviousItemText );
      }

    // initialize all needed variables
    m_PreviousItemText.clear();
    m_EditionActive = false;

    // set back default item flags
    m_ItemToEdit->setFlags( m_DefaultItemFlags );
    }
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnCustomContextMenuRequested(const QPoint& pos)
{
  // get the item
  QTreeWidgetItem* item = itemAt(pos);
  
  // if not the root item 
  if ( item && item->parent() ) 
    {
    // update clicked poistion
    m_ContextualMenuClickedPosition = pos;

    // menu 
    QMenu  menu;
    
    // 
    // create the desired action
    QAction * deleteNodeChild = new QAction(tr ("Delete Dataset") , &menu);

    // use a QSignalMapper to bundle parameterless signals and re-emit
    // them with parameters (QString here)
    QSignalMapper *signalMapperDelete = new QSignalMapper( this );
    signalMapperDelete->setMapping( deleteNodeChild, item->text(0) );
    
    QObject::connect( deleteNodeChild , 
                      SIGNAL(triggered()), 
                      signalMapperDelete, 
                      SLOT( map() ) 
      );

    // add action to the menu
    menu.addAction( deleteNodeChild );

    // connect the re-emitted signal to our slot 
    QObject::connect( 
      signalMapperDelete, 
      SIGNAL(mapped(const QString &)), 
      SLOT( OnDeleteTriggered(const QString &)) 
      );

    // 
    // create the desired action
    QAction * renameNodeChild = new QAction(tr ("Rename Dataset") , &menu);

    // 
    QObject::connect( renameNodeChild , 
                      SIGNAL(triggered()), 
                      this, 
                      SLOT( OnRenameTriggered( ) ) 
      );

    // add action to the menu
    menu.addAction( renameNodeChild );

    // show menu
    menu.exec( viewport()->mapToGlobal(pos) );
    }
}

} // end namespace 'mvd'

