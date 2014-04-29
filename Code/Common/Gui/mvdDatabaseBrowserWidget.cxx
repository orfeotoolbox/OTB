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
#include "mvdDatabaseBrowserWidget.h"
#include "ui_mvdDatabaseBrowserWidget.h"


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
#include "Gui/mvdDatabaseTreeWidget.h"
#include "Gui/mvdTreeWidgetItem.h"

#define ENABLE_DISPLAY_ID 1

namespace mvd
{

/*
  TRANSLATOR mvd::DatabaseBrowserWidget

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
DatabaseBrowserWidget
::DatabaseBrowserWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::DatabaseBrowserWidget() ),
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
    SIGNAL( ItemMoved( QTreeWidgetItem* ) ),
    // to:
    this,
    SIGNAL( ItemMoved( QTreeWidgetItem* ) )
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
DatabaseBrowserWidget
::~DatabaseBrowserWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
const QTreeWidgetItem*
DatabaseBrowserWidget
::GetRootItem() const
{
  return m_UI->databaseTreeWidget->invisibleRootItem();
}

/*****************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidget
::GetRootItem()
{
  return m_UI->databaseTreeWidget->invisibleRootItem();
}

/*****************************************************************************/
DatabaseBrowserWidget::QTreeWidgetItemList
DatabaseBrowserWidget
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
DatabaseBrowserWidget
::InsertLeafItem( QTreeWidgetItem* parent,
                  const QString& text,
                  const QVariant& id,
                  const QStringList& columns )
{
  return
    InsertItem(
      parent,
      TreeWidgetItem::ITEM_TYPE_LEAF,
      text,
      id,
      columns
    );
}

/*******************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidget
::InsertItem( QTreeWidgetItem* parent,
              TreeWidgetItem::ItemType type,
              const QString& text,
              const QVariant& id,
              const QStringList& columns )
{
  TreeWidgetItem* item =
    new TreeWidgetItem(
      parent,
      text,
      id,
      columns,
      type
    );

  return item;
}

/*******************************************************************************/
QTreeWidgetItem*
DatabaseBrowserWidget
::InsertNodeItem( QTreeWidgetItem* parent,
                  const QString& text,
                  const QVariant& id,
                  const QStringList& columns )
{
  return
    InsertItem(
      parent,
      TreeWidgetItem::ITEM_TYPE_NODE,
      text,
      id,
      columns
    );
}

/*****************************************************************************/
void
DatabaseBrowserWidget
::SetupUI()
{
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
DatabaseBrowserWidget
::GetDatabaseTreeWidget()
{
  return m_UI->databaseTreeWidget;
}
*/

/*****************************************************************************/
void
DatabaseBrowserWidget
::SetCurrentDataset( const QString& hash )
{
  // qDebug() << this << "::SetCurrentDataset(" << hash << ")";

  assert( m_UI!=NULL );
  assert( m_UI->databaseTreeWidget!=NULL );

  QList< QTreeWidgetItem* > items(
    m_UI->databaseTreeWidget->findItems(
      hash,
      Qt::MatchExactly | Qt::MatchRecursive,
      TreeWidgetItem::COLUMN_INDEX_HASH
    )
  );

  assert( items.isEmpty() || items.size() == 1 );

  qDebug()
    << ( items.isEmpty() ? "NONE" : items.first()->text( 0 ) )
    << m_UI->databaseTreeWidget->selectionModel()->selectedIndexes().size();

  m_UI->databaseTreeWidget->setCurrentItem(
    items.isEmpty()
    ? NULL
    : items.first(),
    TreeWidgetItem::COLUMN_INDEX_TEXT,
    QItemSelectionModel::Clear |
    QItemSelectionModel::Select |
    QItemSelectionModel::Current
  );
}

/*****************************************************************************/
void
DatabaseBrowserWidget
::InstallTreeEventFilter( QObject* eventFilter )
{
  m_UI->databaseTreeWidget->installEventFilter( eventFilter );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseBrowserWidget
::on_databaseTreeWidget_currentItemChanged( QTreeWidgetItem* current,
					    QTreeWidgetItem* previous )
{
  //
  // Current

  // TODO: Should be DatabaseModel::DatasetId but widgets should not depend on models!!!
  QString currentHash;

  TreeWidgetItem* currentItem = dynamic_cast< TreeWidgetItem* >( current );

  if( currentItem!=NULL && currentItem->parent()!=NULL )
    // if current is root and not NULL get the Id of the corresponding
    // Dataset.
    currentHash = currentItem->GetHash();

  //
  // Previous

  // TODO: Should be DatabaseModel::DatasetId but widgets should not depend on models!!!
  QString previousHash;

  TreeWidgetItem* previousItem = dynamic_cast< TreeWidgetItem* >( previous );

  if( previousItem!=NULL )
    previousHash = previousItem->GetHash();

  //
  // Emit event.
  emit CurrentDatasetChanged( currentHash, previousHash );
}

/*******************************************************************************/
void
DatabaseBrowserWidget
::on_m_SearchLine_textChanged( const QString& search )
{
  // qDebug() << this << "on_m_SearchLine_textChanged(" << search << ")";

  //
  // get the search text
  m_SearchText = search;

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
}

} // end namespace 'mvd'
