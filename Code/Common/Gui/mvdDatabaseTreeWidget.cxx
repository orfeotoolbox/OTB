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
#include "mvdDatabaseTreeWidget.h"


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
#include "Core/mvdDataStream.h"
#include "Gui/mvdTreeWidgetItem.h"

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
  setDefaultDropAction( Qt::MoveAction );

  // setMouseTracking(true);
  setDragEnabled( true );
  
  setAcceptDrops( true );

  // setup contextual menu
  InitializeContextualMenu();

  //
  // do some connection
  QObject::connect(
    this,
    SIGNAL( itemChanged( QTreeWidgetItem* , int ) ),
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
  
  QObject::connect(
    this,
    SIGNAL( customContextMenuRequested( const QPoint& ) ),
    SLOT( OnCustomContextMenuRequested( const QPoint& ) )
  );
}

/*******************************************************************************/
void 
DatabaseTreeWidget::mouseMoveEvent( QMouseEvent * event )
{
#if BYPASS_MOUSE_EVENTS
  QTreeWidget::mouseMoveEvent( event );

#else // BYPASS_MOUSE_EVENTS
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
#endif // BYPASS_MOUSE_EVENTS
}

/*******************************************************************************/
void
DatabaseTreeWidget::mousePressEvent(QMouseEvent *event)
{
#if BYPASS_MOUSE_EVENTS
  QTreeWidget::mousePressEvent( event );

#else // BYPASS_MOUSE_EVENTS
  if (event->button() == Qt::LeftButton)
     {
     //
     // superclass event handling
     QTreeWidget::mousePressEvent(event);
     
     // remember start drag position
     m_StartDragPosition = event->pos();
     }

#endif // BYPASS_MOUSE_EVENTS
}

/*******************************************************************************/
void
DatabaseTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
  qDebug() << this << "::dragEnterEvent(" << event << ") :"
           << event->mimeData()->formats();

#if BYPASS_DRAG_AND_DROP_EVENTS
  QTreeWidget::dragEnterEvent( event );

#else // BYPASS_DRAG_AND_DROP_EVENTS
  if (event->mimeData()->hasUrls()/*("text/uri-list")*/)
    {
    event->acceptProposedAction();
    }

#endif // BYPASS_DRAG_AND_DROP_EVENTS
}

/*******************************************************************************/
void DatabaseTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
  // qDebug() << this << "::dragMoveEvent(" << event << ") :"
  //          << event->mimeData()->formats();

#if BYPASS_DRAG_AND_DROP_EVENTS
  QTreeWidget::dragMoveEvent( event );

#else // BYPASS_DRAG_AND_DROP_EVENTS
  //
  // if the mouse is within the QLabel geometry : allow drops
  if ( event->answerRect().intersects( this->geometry() ) )
    {
    event->acceptProposedAction();
    }
  else
    {
    event->ignore();
    }
#endif // BYPASS_DRAG_AND_DROP_EVENTS
}

/*******************************************************************************/
void 
DatabaseTreeWidget::dropEvent(QDropEvent *event)
{
  qDebug() << this << "::dropEvent(" << event << ")";

#if BYPASS_DRAG_AND_DROP_EVENTS
  if( event->mimeData()->hasFormat( "application/x-qabstractitemmodeldatalist" ) )
    {
    QByteArray byteArray(
      event->mimeData()->data( "application/x-qabstractitemmodeldatalist" )
    );
    QDataStream stream( &byteArray, QIODevice::ReadOnly );
    int count = 0;

    //
    // http://www.qtcentre.org/threads/8756-QTreeWidgetItem-mime-type

    typedef QMap< int, QVariant > QIntToVariantMap;

    struct Item
    {
      Item() : row( -1 ), col( -1 ), var() {}

      int row;
      int col;
      QIntToVariantMap var;
    };

    while( !stream.atEnd() )
      {
      Item item;

      stream >> item.row >> item.col >> item.var;

      qDebug() << item.row << item.col << item.var;

      ++ count;
      }

    qDebug() << count << "items.";
    }

  if( event->mimeData()->hasFormat( "application/x-qtreewidgetitemptrlist" ) )
    {
    QByteArray byteArray2(
      event->mimeData()->data( "application/x-qtreewidgetitemptrlist" )
    );
    QDataStream stream2( &byteArray2, QIODevice::ReadOnly );
    int count2 = 0;

    //
    // http://www.qtcentre.org/threads/8756-QTreeWidgetItem-mime-type

    QTreeWidgetItem* varItem = NULL;

    while( !stream2.atEnd() )
      {
      QVariant variant;

      stream2 >> variant;

      qDebug() << "Variant:" << variant;

      // http://www.qtfr.org/viewtopic.php?id=9630

      varItem = variant.value< QTreeWidgetItem* >();

      qDebug()
        << "Item (variant):"
        << varItem
        << varItem->text( 0 )
        << varItem->text( 1 )
        << varItem->text( 2 )
        << varItem->parent();

      ++ count2;
      }

    qDebug() << count2 << "items.";
    }

  QTreeWidget::dropEvent( event );

#else // BYPASS_DRAG_AND_DROP_EVENTS

  QList<QUrl> urls = event->mimeData()->urls();

  // cheking
  if (urls.isEmpty())
    return;

  // get the filename and send 
  QString fileName = urls.first().toLocalFile();
  if (fileName.isEmpty())
    {
    return;
    }
  else
    {
    emit ImageDropped( fileName );
    }

#endif // BYPASS_DRAG_AND_DROP_EVENTS
}

/*******************************************************************************/
QStringList
DatabaseTreeWidget
::mimeTypes() const
{
  // qDebug() << this << "::mimeTypes()";

  QStringList mimeTypes( QTreeWidget::mimeTypes() );

  mimeTypes << "application/x-qtreewidgetitemptrlist";

  return mimeTypes;
}

/*******************************************************************************/
QMimeData*
DatabaseTreeWidget
::mimeData( const QList< QTreeWidgetItem* > items ) const
{
  qDebug() << this << "::mimeData(" << items << ")";

  QMimeData* mimeData = QTreeWidget::mimeData( items );

  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

  QByteArray byteArray;
  QDataStream stream( &byteArray, QIODevice::WriteOnly );

  for( QTreeWidgetItemList::const_iterator it( items.begin() );
       it!=items.end();
       ++it )
    {
    /*
    qDebug()
      << "QTreeWidgetItem::parent()==" << ( *it )->parent();
    qDebug()
      << "Pointer:" << static_cast< void* >( *it );
    qDebug()
      << "Variant:" << QVariant::fromValue< QTreeWidgetItem* >( *it );
    */

    // http://www.qtfr.org/viewtopic.php?id=9630
    // stream << *it;
    stream << QVariant::fromValue< QTreeWidgetItem* >( *it );
    }

  mimeData->setData( "application/x-qtreewidgetitemptrlist", byteArray );

  /*
  qDebug() << mimeData->formats();

  for( QTreeWidgetItemList::const_iterator it( items.begin() );
       it!=items.end();
       ++it )
    {
    QTreeWidgetItem* item = *it;

    qDebug()
      << item->type() << item->text( 0 ) << item->text( 1 ) << item->text( 2 );
    }
  */

  return mimeData;
}

/*******************************************************************************/
bool
DatabaseTreeWidget::dropMimeData( QTreeWidgetItem* parent,
                                  int index,
                                  const QMimeData* data,
                                  Qt::DropAction action )
{
  qDebug()
    << this << "::dropMimeData("
    << parent << ","
    << index << ","
    << data << ","
    << action << ")";

  bool result = QTreeWidget::dropMimeData( parent, index, data, action );

  qDebug() << "->" << result;

  return result;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseTreeWidget
::OnSelectedDatasetFilenameChanged( const QString& filename )
{
  //
  // update the dataset image filename to be used in the dragged mime data
  m_DatasetFilename = filename;
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnDeleteTriggered( const QString & id)
{
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
      // dynamic cast to get the id
      TreeWidgetItem* dsItem = 
        dynamic_cast<TreeWidgetItem *>( item );

      // send the new alias of the dataset / it identifier
      emit DatasetRenamed(dsItem->text(column), dsItem->GetHash()  );
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

/******************************************************************************/
void
DatabaseTreeWidget
::keyPressEvent( QKeyEvent * event )
{
  // triggered only if an item (and not the root one) is selected
  if ( currentItem() && currentItem()->parent() )
    {

    switch (event->key())
      {
      case Qt::Key_F2:
      {
      // find the QTreeWidgetItem and make it editable
      m_ItemToEdit = currentItem();
      m_PreviousItemText = m_ItemToEdit->text(0);
      m_DefaultItemFlags  = m_ItemToEdit->flags(); 
      m_ItemToEdit->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

      // edit item
      // inspired from here:
      // http://www.qtcentre.org/archive/index.php/t-45857.html?s=ad4e7c45bbd9fd4bf5cc32853dd3fe82  
      m_EditionActive = true;
      editItem(m_ItemToEdit, 0);
      
      break;
      }
      case Qt::Key_Delete:
      {
      // cast to DatasetTreeItem
      TreeWidgetItem* item 
        = dynamic_cast<TreeWidgetItem *>( currentItem());
        
      emit DatasetToDeleteSelected( item->GetHash() );

      break;
      }
      }

    }
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnCustomContextMenuRequested(const QPoint& pos)
{
  // get the item
  TreeWidgetItem* item = dynamic_cast<TreeWidgetItem *>( itemAt(pos) );
  
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
    signalMapperDelete->setMapping( deleteNodeChild, item->GetHash() );
    
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
