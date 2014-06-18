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
#include "mvdDatabaseBrowserWidgetTest.h"
#include "ui_mvdDatabaseBrowserWidgetTest.h"


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
#if !DISABLE_CUSTOM_TW_TEST
#  include "Gui/mvdTreeWidgetItem.h"
#endif

#define ENABLE_DISPLAY_ID 1

namespace mvd
{

/*
  TRANSLATOR mvd::DatabaseBrowserWidgetTest

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
DatabaseBrowserWidgetTest
::DatabaseBrowserWidgetTest( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::DatabaseBrowserWidgetTest() ),
  m_StartDragPosition(),
  m_SearchText()
{
  m_UI->setupUi( this );

  SetupUI();

  /*
  // Devember 12th, 2013 - Trainee example. 
  QPushButton* button=new QPushButton ("Click me", this);
  layout()->addWidget(button);
  */

  //
  // Forward signals.

  QObject::connect(
    m_UI->databaseTreeWidget,
    SIGNAL( ItemMoved( QTreeWidgetItem*, QTreeWidgetItem* ) ),
    // to:
    this,
    SIGNAL( ItemMoved( QTreeWidgetItem*, QTreeWidgetItem* ) )
  );

  QObject::connect(
    m_UI->databaseTreeWidget,
    SIGNAL( AddItemRequested( QTreeWidgetItem* ) ),
    // to:
    this,
    SIGNAL( AddItemRequested( QTreeWidgetItem* ) )
  );

  QObject::connect(
    m_UI->databaseTreeWidget,
    SIGNAL( DeleteItemRequested( QTreeWidgetItem* ) ),
    // to:
    this,
    SIGNAL( DeleteItemRequested( QTreeWidgetItem* ) )
  );

  QObject::connect(
    m_UI->databaseTreeWidget,
    SIGNAL( ItemTextChanged( QTreeWidgetItem*, int ) ),
    // to:
    this,
    SIGNAL( ItemTextChanged( QTreeWidgetItem*, int ) )
  );
}

/*******************************************************************************/
DatabaseBrowserWidgetTest
::~DatabaseBrowserWidgetTest()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
const QTreeWidgetItem*
DatabaseBrowserWidgetTest
::GetRootItem() const
{
  return m_UI->databaseTreeWidget->invisibleRootItem();
}

/*****************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidgetTest
::GetRootItem()
{
  return m_UI->databaseTreeWidget->invisibleRootItem();
}

/*****************************************************************************/
DatabaseBrowserWidgetTest::QTreeWidgetItemList
DatabaseBrowserWidgetTest
::GetItems() const
{
  return
    m_UI->databaseTreeWidget->findItems(
      "*",
      Qt::MatchWildcard | Qt::MatchRecursive,
      0
    );
}

/*******************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidgetTest
::InsertLeafItem( QTreeWidgetItem* parent,
                  const QString& text,
                  const QVariant& id,
                  const QStringList& columns )
{
  return
    InsertItem(
      parent,
#if DISABLE_CUSTOM_TW_ITEM
      ITEM_TYPE_LEAF,
#else // DISABLE_CUSTOM_TW_ITEM
      TreeWidgetItem::ITEM_TYPE_LEAF,
#endif // DISABLE_CUSTOM_TW_ITEM
      text,
      id,
      columns
    );
}

/*******************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidgetTest
::InsertItem( QTreeWidgetItem* parent,
#if DISABLE_CUSTOM_TW_ITEM
              ItemType type,
#else // DISABLE_CUSTOM_TW_ITEM
              TreeWidgetItem::ItemType type,
#endif // DISABLE_CUSTOM_TW_ITEM
              const QString& text,
              const QVariant& id,
              const QStringList& columns )
{
#if DISABLE_CUSTOM_TW_ITEM
  QTreeWidgetItem * item =
    new QTreeWidgetItem( parent, QStringList( text ) << QString() << columns, type );

  item->setData( COLUMN_INDEX_ID, ITEM_ROLE_ID, id );
  item->setText( COLUMN_INDEX_ID, id.toString() );

  switch( type )
    {
    case ITEM_TYPE_NODE:
      item->setChildIndicatorPolicy(
        QTreeWidgetItem::ShowIndicator
      );
      item->setFlags(
        // Qt::ItemIsDragEnabled |
        Qt::ItemIsDropEnabled |
        Qt::ItemIsEditable |
        Qt::ItemIsEnabled
      );
    break;

    case ITEM_TYPE_LEAF:
      item->setChildIndicatorPolicy(
        QTreeWidgetItem::DontShowIndicator
      );
      item->setFlags(
        Qt::ItemIsDragEnabled |
        Qt::ItemIsEditable |
        Qt::ItemIsEnabled |
        Qt::ItemIsSelectable
      );
      break;

    default:
      assert( false && "Unhandled ItemType." );
      break;
    }

#else // DISABLE_CUSTOM_TW_ITEM
  TreeWidgetItem* item =
    new TreeWidgetItem(
      parent,
      text,
      id,
      columns,
      type
    );
#endif // DISABLE_CUSTOM_TW_ITEM
  return item;
}

/*******************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidgetTest
::InsertNodeItem( QTreeWidgetItem* parent,
                  const QString& text,
                  const QVariant& id,
                  const QStringList& columns )
{
  return
    InsertItem(
      parent,
#if DISABLE_CUSTOM_TW_ITEM
      ITEM_TYPE_NODE,
#else // DISABLE_CUSTOM_TW_ITEM
      TreeWidgetItem::ITEM_TYPE_NODE,
#endif // DISABLE_CUSTOM_TW_ITEM
      text,
      id,
      columns
    );
}

/*****************************************************************************/
void
DatabaseBrowserWidgetTest
::SetupUI()
{
  // m_UI->databaseTreeWidget->setSelectionBehaviour(
  //   QAbstractItemView::SelectRows
  // );
  m_UI->databaseTreeWidget->setDragEnabled( true );
  m_UI->databaseTreeWidget->setAcceptDrops( true );
  // m_UI->databaseTreeWidget->setDragDropMode( QAbstractItemView::DragDrop );


  //
  // Header columns.
  m_UI->databaseTreeWidget->headerItem()->setText( 1, "Id" );
  m_UI->databaseTreeWidget->headerItem()->setText( 2, "Info" );

// Conditionaly display some usefull columns.
// #if (!defined( _DEBUG ) && FORCE_DISABLE) || FORCE_ENABLE
#if (!defined( _DEBUG ) && 1) || 0
  m_UI->databaseTreeWidget->setColumnHidden( 1, true );
  m_UI->databaseTreeWidget->setColumnHidden( 2, true );
#endif

  // set placeholder text
#if (QT_VERSION >= 0x407000)
  m_UI->m_SearchLine->setPlaceholderText( tr( "Search Dataset..." ) );
#endif
}

/*******************************************************************************/
/*
DatabaseTreeWidget*
DatabaseBrowserWidgetTest
::GetDatabaseTreeWidget()
{
  return m_UI->databaseTreeWidget;
}
*/

/*****************************************************************************/
void
DatabaseBrowserWidgetTest
::SetCurrentDataset( const QString& hash )
{
  // qDebug() << this << "::SetCurrentDataset(" << hash << ")";

#if QT_VERSION < QT_VERSION_CHECK( 4, 8, 1 )
  // Workaround for MANTIS-934 (crash/memory corruption after having
  // imported second image).
#else
  assert( m_UI!=NULL );
  assert( m_UI->databaseTreeWidget!=NULL );

  QList< QTreeWidgetItem* > items(
    m_UI->databaseTreeWidget->findItems(
      hash,
      Qt::MatchExactly | Qt::MatchRecursive,
      TreeWidgetItem::COLUMN_INDEX_HASH
    )
  );

  // assert( items.isEmpty() || items.size() == 1 );

  // qDebug()
  //   << ( items.isEmpty() ? "NONE" : items.first()->text( 0 ) )
  //   << m_UI->databaseTreeWidget->selectionModel()->selectedIndexes().size();

  m_UI->databaseTreeWidget->setCurrentItem(
    items.isEmpty()
    ? NULL
    : items.first(),
    TreeWidgetItem::COLUMN_INDEX_TEXT,
    QItemSelectionModel::Clear |
    QItemSelectionModel::Select |
    QItemSelectionModel::Current
  );
#endif
}

/*****************************************************************************/
void
DatabaseBrowserWidgetTest
::InstallTreeEventFilter( QObject* eventFilter )
{
  m_UI->databaseTreeWidget->installEventFilter( eventFilter );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseBrowserWidgetTest
::on_databaseTreeWidget_currentItemChanged( QTreeWidgetItem* current,
					    QTreeWidgetItem* previous )
{
  //
  // Current

  // TODO: Should be DatabaseModel::DatasetId but widgets should not depend on models!!!
  QString currentHash;

#if DISABLE_CUSTOM_TW_ITEM
  if( current!=NULL && current->parent()!=NULL )
    currentHash = current->text( COLUMN_INDEX_HASH );

#else // DISABLE_CUSTOM_TW_ITEM
  TreeWidgetItem* currentItem = dynamic_cast< TreeWidgetItem* >( current );

  if( currentItem!=NULL && currentItem->parent()!=NULL )
    // if current is root and not NULL get the Id of the corresponding
    // Dataset.
    currentHash = currentItem->GetHash();
#endif // DISABLE_CUSTOM_TW_ITEM

  //
  // Previous

  // TODO: Should be DatabaseModel::DatasetId but widgets should not depend on models!!!
  QString previousHash;

#if DISABLE_CUSTOM_TW_ITEM
  if( previous!=NULL )
    previousHash = previous->text( COLUMN_INDEX_HASH );

#else // DISABLE_CUSTOM_TW_ITEM
  TreeWidgetItem* previousItem = dynamic_cast< TreeWidgetItem* >( previous );

  if( previousItem!=NULL )
    previousHash = previousItem->GetHash();
#endif // DISABLE_CUSTOM_TW_ITEM

  //
  // Emit event.
  emit CurrentDatasetChanged( currentHash, previousHash );
}

/*******************************************************************************/
void
DatabaseBrowserWidgetTest
::on_m_SearchLine_textChanged( const QString& search )
{
  // qDebug() << this << "on_m_SearchLine_textChanged(" << search << ")";

  //
  // get the search text
  m_SearchText = search;

#if DISABLE_CUSTOM_TW_ITEM

#else // DISABLE_CUSTOM_TW_ITEM
  QTreeWidgetItemList items( GetItems() );

  // qDebug() << items;

  for( QTreeWidgetItemList::iterator it( items.begin() );
       it!=items.end();
       ++it )
    {
    QTreeWidgetItem* qtwi = *it;
    assert( qtwi!=NULL );

    assert( qtwi==dynamic_cast< TreeWidgetItem* >( qtwi ) );
    TreeWidgetItem* item = dynamic_cast< TreeWidgetItem* >( qtwi );

    if( item==NULL )
      {
      // TODO: Implement general case when needed.
      }
    else
      {
      item->setHidden(
        item->GetType()==TreeWidgetItem::ITEM_TYPE_LEAF &&
        !item->GetText().contains( search, Qt::CaseInsensitive )
      );
      }

    // qDebug()
    //   << item->text( 0 ) << ":" << ( item->isHidden() ? "HIDDEN" : "VISIBLE" );
    }
#endif // DISABLE_CUSTOM_TW_ITEM
}

} // end namespace 'mvd'
