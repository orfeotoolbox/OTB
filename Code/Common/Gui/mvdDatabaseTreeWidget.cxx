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
/* SUB-CLASSES DECLARATION                                                   */

class DatabaseTreeWidget::EditionContext
{
  // Q_OBJECT;

public:
  EditionContext() :
    m_Item( NULL ),
    m_Text(),
    m_Column( -1 )
  {
  }

  inline bool IsEditable() const
  {
    return m_Item!=NULL && m_Item->flags().testFlag( Qt::ItemIsEditable );
  }

  inline bool IsActive() const
  {
    return IsEditable() && m_Column>=0;
  }

  inline QTreeWidgetItem* GetItem()
  {
    return m_Item;
  }

  inline void SetItem( QTreeWidgetItem* item )
  {
    EditItem();

    m_Item = item;
  }

  inline int GetColumn() const
  {
    return m_Column;
  }

  inline void EditItem( int column =-1 )
  {
    if( column<0 )
      {
      m_Column = -1;
      m_Text.clear();
      }
    else
      {
      assert( m_Item!=NULL );
      assert( m_Column<0 );

      m_Column = column;

      m_Text = m_Item->text( column );
      }
  }

  inline void RestoreItem()
  {
    assert( m_Item!=NULL );
    assert( m_Column>=0 );

    m_Item->setText( m_Column, m_Text );

    m_Column = -1;
  }

private:
  QTreeWidgetItem* m_Item;
  QString m_Text;
  int m_Column;
};

/*****************************************************************************/
/* SUB-CLASSES DEFINITION                                                    */

/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatabaseTreeWidget
::DatabaseTreeWidget( QWidget* parent  ):
  TreeWidget( parent ),
  // m_DatasetFilename(),
  // m_EditionActive( false )
  m_EditionContext( new EditionContext() )
{
  // setMouseTracking(true);

  //
  // do some connection
  QObject::connect(
    this,
    SIGNAL( itemChanged( QTreeWidgetItem* , int ) ),
    SLOT( OnItemChanged( QTreeWidgetItem* , int ) )
  );

  QObject::connect(
    this,
    SIGNAL( customContextMenuRequested( const QPoint& ) ),
    SLOT( OnCustomContextMenuRequested( const QPoint& ) )
  );
}

/*******************************************************************************/
DatabaseTreeWidget
::~DatabaseTreeWidget()
{
  delete m_EditionContext;
  m_EditionContext = NULL;
}

/*******************************************************************************/
void 
DatabaseTreeWidget::mouseMoveEvent( QMouseEvent * event )
{
  TreeWidget::mouseMoveEvent( event );
}

/*******************************************************************************/
void
DatabaseTreeWidget::mousePressEvent(QMouseEvent *event)
{
  TreeWidget::mousePressEvent( event );
}

/*******************************************************************************/
void
DatabaseTreeWidget::dropEvent(QDropEvent *event)
{
  // qDebug() << this << "::dropEvent(" << event << ")";

  /*
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
  */

  /*
  if( event->mimeData()->hasFormat( "application/x-qtreewidgetitemptrlist" ) )
    {
    QByteArray byteArray(
      event->mimeData()->data( "application/x-qtreewidgetitemptrlist" )
    );

    QDataStream stream( &byteArray, QIODevice::ReadOnly );

    int count = 0;

    //
    // http://www.qtcentre.org/threads/8756-QTreeWidgetItem-mime-type

    QTreeWidgetItem* varItem = NULL;

    while( !stream.atEnd() )
      {
      QVariant variant;

      stream >> variant;

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

      ++ count;
      }

    qDebug() << count << "items.";
    }
  */

  TreeWidget::dropEvent( event );
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

  bool result = TreeWidget::dropMimeData( parent, index, data, action );

  qDebug() << "->" << result;

  return result;
}

/*******************************************************************************/
void
DatabaseTreeWidget
::EditItem( int column )
{
  assert( m_EditionContext!=NULL );
  assert( m_EditionContext->GetItem()!=NULL );

  m_EditionContext->EditItem( column );

  if( m_EditionContext->IsActive() )
    editItem( m_EditionContext->GetItem(), m_EditionContext->GetColumn() );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseTreeWidget
::OnAddItemTriggered()
{
  assert( m_EditionContext!=NULL );
  assert( m_EditionContext->GetItem()!=NULL );

  emit AddItemRequested( m_EditionContext->GetItem() );
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnDeleteItemTriggered()
{
  assert( m_EditionContext!=NULL );
  assert( m_EditionContext->GetItem()!=NULL );

  emit DeleteItemRequested( m_EditionContext->GetItem() );
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnRenameItemTriggered()
{
  EditItem( 0 );

  /*
  // find the QTreeWidgetItem and make it editable
  m_ItemToEdit = 
  m_PreviousItemText = m_ItemToEdit->text(0);
  // m_DefaultItemFlags  = m_ItemToEdit->flags(); 
  // m_ItemToEdit->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);

  // edit item
  // inspired from here:
  // http://www.qtcentre.org/archive/index.php/t-45857.html?s=ad4e7c45bbd9fd4bf5cc32853dd3fe82  
  m_EditionActive = true;

  editItem( m_ItemToEdit, 0 );
  */
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnItemChanged( QTreeWidgetItem* item , int column )
{
  assert( m_EditionContext!=NULL );

  if( !m_EditionContext->IsActive() )
    return;

  if( item->text( column ).isEmpty() )
    m_EditionContext->RestoreItem();
  else
    {
    /*
    // dynamic cast to get the id
    TreeWidgetItem* dsItem = 
      dynamic_cast<TreeWidgetItem *>( item );

    // send the new alias of the dataset / it identifier
    emit DatasetRenamed( dsItem->text( column ), dsItem->GetHash() );
    */

    assert( m_EditionContext->GetItem()==item );

    emit ItemTextChanged( item, column );
    }

  m_EditionContext->EditItem();
}

/******************************************************************************/
void
DatabaseTreeWidget
::keyPressEvent( QKeyEvent* event )
{
  assert( currentItem()!=NULL );

  assert( m_EditionContext!=NULL );

  QTreeWidgetItem * item = currentItem();

  switch( event->key() )
    {
    case Qt::Key_F2:
        m_EditionContext->SetItem( item );
        EditItem( 0 );
      break;

    case Qt::Key_Delete:
        m_EditionContext->SetItem( item );
        if( m_EditionContext->IsEditable() )
          emit DeleteItemRequested( m_EditionContext->GetItem() );
      break;

    default:
      TreeWidget::keyPressEvent( event );
      break;
      /*
    {
    // cast to DatasetTreeItem
    TreeWidgetItem* item 
      = dynamic_cast<TreeWidgetItem *>( currentItem() );
        
    emit DatasetToDeleteSelected( item->GetHash() );
    break;
    }
      */
    }
}

/*******************************************************************************/
void
DatabaseTreeWidget::OnCustomContextMenuRequested(const QPoint& pos)
{
  // Get  item.
  QTreeWidgetItem* qtwi = itemAt( pos );

  if( qtwi==NULL )
    return;

  TreeWidgetItem* item =
    dynamic_cast< TreeWidgetItem* >( itemAt( pos ) );

  assert( item!=NULL );


  /*
  QString itemType;

  switch( item->type() )
    {
    case TreeWidgetItem::ITEM_TYPE_NONE:
      break;

    case TreeWidgetItem::ITEM_TYPE_NODE:
      itemType = tr( "group" );
      break;

    case TreeWidgetItem::ITEM_TYPE_LEAF:
      itemType = tr( "dataset" );
      break;

    default:
      assert( false && "Unhandled TreeWidgetItem::ItemType value!" );
      break;
    };
  */

  // Remember context-menu clicked position.
  //
  // This is done to later find cicked item because QTreeWidgetItem is
  // not derived from QObject and QSignalMapper only supports int,
  // QString, QObject and QWiget.
  //
  // This is also the reason why delete and rename triggers are
  // duplicated.
  // m_ContextualMenuClickedPosition = pos;

  // Remember clicked item.
  assert( m_EditionContext!=NULL );
  m_EditionContext->SetItem( item );

  // Create context-menu.
  QMenu menu;

  // Insert actions into context-menu.
  if( item->parent()!=NULL &&
      item->type()==TreeWidgetItem::ITEM_TYPE_LEAF )
    {
    /*
    AddMappedAction(
      &menu,
      tr( "Delete dataset" ),
      item->GetHash(),
      this,
      SIGNAL( DatasetToDeleteSelected( const QString& ) )
    );
    */

    AddAction(
      &menu,
      tr( "Delete dataset" ),
      this,
      SLOT( OnDeleteItemTriggered() )
    );

    if( item->flags().testFlag( Qt::ItemIsEditable ) )
      AddAction(
        &menu,
        tr ("Rename dataset"),
        this, 
        SLOT( OnRenameItemTriggered() )
      );
    }

  if( item->type()==TreeWidgetItem::ITEM_TYPE_NODE )
    {
    AddAction(
      &menu,
      tr( "Add group" ),
      this,
      SLOT( OnAddItemTriggered() )
    );

    QAction * action2 =
      AddAction(
        &menu,
        tr( "Delete group" ),
        this,
        SLOT( OnDeleteItemTriggered() )
      );

    action2->setEnabled(
      item->childCount()<=0 && m_EditionContext->IsEditable()
    );

    QAction * action3 =
      AddAction(
        &menu,
        tr( "Rename group" ),
        this,
        SLOT( OnRenameItemTriggered() )
      );

    action3->setEnabled( m_EditionContext->IsEditable() );
    }

  // Display and activate context-menu.
  menu.exec( viewport()->mapToGlobal( pos ) );
}

/*******************************************************************************/
QAction*
DatabaseTreeWidget::AddAction( QMenu* menu,
                               const QString& text,
                               QObject* receiver,
                               const char* method,
                               Qt::ConnectionType type )
{
  assert( menu!=NULL );
  assert( receiver!=NULL );
  assert( method!=NULL );

  QAction* action = menu->addAction( text );

  QObject::connect(
    action,
    SIGNAL( triggered() ),
    // to:
    receiver,
    method,
    // with:
    type
  );

  return action;
}

/*******************************************************************************/
QAction*
DatabaseTreeWidget::AddMappedAction( QMenu* menu,
                                     const QString& text,
                                     const QString& data,
                                     QObject* receiver,
                                     const char* slot,
                                     Qt::ConnectionType type )
{
  assert( menu!=NULL );
  assert( receiver!=NULL );
  assert( slot!=NULL );

  // Add signal-mapped action into menu.
  QSignalMapper* signalMapper = AddMappedAction( menu, text );
  assert(
    signalMapper->parent()==qobject_cast< QAction* >( signalMapper->parent() )
  );

  // Get action back.
  QAction* action = qobject_cast< QAction* >( signalMapper->parent() );
  assert( action!=NULL );

  // Set signal mapping.
  signalMapper->setMapping( action, data );

  // Connect mapped signal.
  QObject::connect(
    signalMapper,
    SIGNAL( mapped( const QString& ) ),
    // to:
    receiver,
    slot,
    // with:
    type
  );


  return action;
}

/*******************************************************************************/
QSignalMapper *
DatabaseTreeWidget::AddMappedAction( QMenu* menu,
                                     const QString& text )
{
  assert( menu!=NULL );

  // Add menu action.
  QAction * action = menu->addAction( text );

  // Pre-create signal-mapper.
  QSignalMapper* signalMapper = new QSignalMapper( action );

  // Connect menu-action trigger to signal-mapper.
  QObject::connect(
    action,
    SIGNAL( triggered() ),
    // to:
    signalMapper,
    SLOT( map() )
  );

  // Return parented signal-mapper.
  return signalMapper;
}

} // end namespace 'mvd'
