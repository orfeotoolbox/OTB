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
#include "Gui/mvdDatabaseBrowserWidget.h"
#include "Gui/ui_mvdDatabaseBrowserWidget.h"


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
#include "Gui/mvdDatasetTreeWidgetItem.h"

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
  m_DatasetRootItem( NULL )
{
  m_UI->setupUi( this );

#if defined( _DEBUG ) && ENABLE_DISPLAY_ID
  m_UI->databaseTreeWidget->headerItem()->setText( 1, "Id" );
#endif

  SetupUI();

  //
  // connect search box changed
  QObject::connect(
    m_UI->m_SearchLine,
    SIGNAL( textChanged( const QString& ) ),
    // to:
    this,
    SLOT( OnSearchBoxChanged( const QString& ) )
  );
}

/*******************************************************************************/
DatabaseBrowserWidget
::~DatabaseBrowserWidget()
{
}

/*******************************************************************************/
void
DatabaseBrowserWidget
::SetDatasetList( const StringPairListType& list )
{
  // qDebug() << this << "::SetDatasetList(" << list << ")";

  // remember dataset list
  m_DatasetList = list;

  // Fill Tree
  FillTree();
}

/*******************************************************************************/
void
DatabaseBrowserWidget
::FillTree()
{
  // get the currentItem Id if any selected.
  // since all the TreeWidgetItem are deleted next, need to remember
  // it in order to set it back
  QString currentItemId = QString();

  DatasetTreeWidgetItem* selectedItem = 
    dynamic_cast<DatasetTreeWidgetItem*>( GetDatabaseTreeWidget()->currentItem() );
  if (selectedItem)
    {
    currentItemId = selectedItem->GetId();
    }
  
  // Remove all previously stored dataset child items.
  while( m_DatasetRootItem->childCount()>0 )
    {
    // Remove dataset child item and reference it.
    QTreeWidgetItem* child = m_DatasetRootItem->takeChild( 0 );

    // Delete it from memory.
    delete child;
    child = NULL;
    }

  // Append full dataset list...
  for( StringPairListType::const_iterator it( m_DatasetList.begin() );
       it!=m_DatasetList.end();
       ++it )
    {
    // qDebug() << "+" << *it;

    // current alias
    const StringPairListType::value_type::first_type& alias = it->first;
    const StringPairListType::value_type::second_type& id = it->second;

    // Is the searchText match the current alias or at least a part of
    // it 
    if (m_SearchText.isEmpty() ||
        alias.contains(m_SearchText ,Qt::CaseInsensitive) )
      {
      DatasetTreeWidgetItem* item =
	new DatasetTreeWidgetItem( m_DatasetRootItem, id, alias );

      // was it the selected item ?
      if( currentItemId == alias )
        {
        // ...add this child item as currentItem
        GetDatabaseTreeWidget()->setCurrentItem( item );
        }
      }
    
    }

}

/*****************************************************************************/
void
DatabaseBrowserWidget
::SetupUI()
{
  // 
  setAcceptDrops(true);

  m_DatasetRootItem = m_UI->databaseTreeWidget->topLevelItem( 0 );
  assert( m_DatasetRootItem!=NULL );
  assert( m_DatasetRootItem->text( 0 )==tr( "Datasets" ) );

#if 0
  m_DatasetRootItem->setChildIndicatorPolicy( DatasetTreeWidgetItem::ShowIndicator );
#endif

  m_DatasetRootItem->setExpanded( true );

  // set placeholder text
#if (QT_VERSION >= 0x407000)
  m_UI->m_SearchLine->setPlaceholderText( tr("Search Dataset ...") );
#endif
}

/*******************************************************************************/
DatabaseTreeWidget*
DatabaseBrowserWidget
::GetDatabaseTreeWidget()
{
  return m_UI->databaseTreeWidget;
}

/*****************************************************************************/
void
DatabaseBrowserWidget
::SetCurrentDataset( const QString& id )
{
  qDebug() << this << "::SetCurrentDataset(" << id << ")";

  assert( m_UI!=NULL );
  assert( m_UI->databaseTreeWidget!=NULL );

  QList< QTreeWidgetItem* > items(
    m_UI->databaseTreeWidget->findItems(
      id,
      Qt::MatchExactly | Qt::MatchRecursive
    )
  );

  assert( items.isEmpty() || items.size()==1 );

  if( items.isEmpty() )
    return;

  m_UI->databaseTreeWidget->setCurrentItem( items.first() );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseBrowserWidget
::on_databaseTreeWidget_currentItemChanged( QTreeWidgetItem* current,
					    QTreeWidgetItem* previous )
{
  qDebug() <<
    this <<
    "::on_databaseTreeWidget_currentItemChanged(" <<
    current << ", " << previous <<
    ")";

  assert( current!=previous );

  // if current is root and not NULL get the Id of the
  // corresponding Dataset
  if ( current && current->parent() )
    {
    emit CurrentDatasetChanged( 
      dynamic_cast<DatasetTreeWidgetItem*>(current)->GetId() 
      );
    }
  else
    {
    emit CurrentDatasetChanged( QString() );
    }
}

/*******************************************************************************/
void
DatabaseBrowserWidget
::OnSearchBoxChanged(const QString & search)
{
  // 
  // get the search text
  m_SearchText = search;
  
  // fill the tree with the application
  FillTree();  
}

} // end namespace 'mvd'
