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

  SetupUI();
}

/*******************************************************************************/
DatabaseBrowserWidget
::~DatabaseBrowserWidget()
{
}

/*******************************************************************************/
void
DatabaseBrowserWidget
::SetDatasetList( const QStringList& list )
{
  // qDebug() << this << "::SetDatasetList" << list;

  // A. If there is no previously stored dataset child item:
  if( m_DatasetRootItem->childCount()==0 )
    {
    // Append full dataset list...
    for( QStringList::const_iterator it( list.begin() );
	 it!=list.end();
	 ++it )
      {
      // qDebug() << "+" << *it;

      // ...as child items.
      new QTreeWidgetItem( m_DatasetRootItem, QStringList( *it ) );
      }
    }

  // B. Otherwise, if given dataset list is empty:
  else if( list.isEmpty() )
    {
    // Remove all previously stored dataset child items.
    while( m_DatasetRootItem->childCount()>0 )
      {
      // qDebug() << "-" << m_DatasetRootItem->child( 0 )->text( 0 );

      // Remove dataset child item and reference it.
      QTreeWidgetItem* child = m_DatasetRootItem->takeChild( 0 );

      // Delete it from memory.
      delete child;
      child = NULL;
      }
    }

  // C. Otherwise, merge previously stored dataset list and given dataset
  // list.
  else
    {
    // Sort list of stored dataset items.
    m_DatasetRootItem->sortChildren( 0, Qt::AscendingOrder );

    // Sort given list of dataset names.
    QStringList sortedList( list );
    sortedList.sort();

    // Initialize iterators to beginning of both dataset lists:
    // - previously stored list of dataset items (list #0);
    // - given list of dataset names, sorted (list #1).
    int i0 = 0;
    QStringList::const_iterator it1( sortedList.begin() );

    // Iterate both dataset lists in parallel.
    while( i0<m_DatasetRootItem->childCount() &&
	   it1!=sortedList.end() )
      {
      // Reference previously stored dataset item (list #0).
      QTreeWidgetItem* child0 = m_DatasetRootItem->child( i0 );
    
      // Compare it's name to the name of the given dataset list (list #1)
      int cmp01 = child0->text( 0 ).compare( *it1 );

      // If it's lower (lexicographically)...
      if( cmp01<0 )
	{
	// qDebug() << "-" << child0->text( 0 );

	// ...then, it means it is not contained in the given list and
	// must be removed.
	m_DatasetRootItem->removeChild( child0 );

	delete child0;
	child0 = NULL;
	}

      // Otherwise, if it's greater (lexicographically)...
      else if( cmp01>0 )
	{
	// qDebug() << "+" << *it1;

	// ...then, it means the item of the given list is not
	// contained and must be inserted.
	m_DatasetRootItem->insertChild(
	  i0,
	  new QTreeWidgetItem( QStringList( *it1 ) )
	);

	++ i0;
	++ it1;
	}

      // Finally, if it's equal (lexicographically)...
      else // if( order==0 )
	{
	// qDebug() << "=" << *it1;

	// ...then, it means both items must be kept contained.
	++ i0;
	++ it1;
	}
      }

    // Remove previously stored dataset item in excess.
    while( i0<m_DatasetRootItem->childCount() )
      {
      // qDebug() << "--" << m_DatasetRootItem->child( i0 )->text( 0 );

      // Remove dataset child item and reference it.
      QTreeWidgetItem* child = m_DatasetRootItem->takeChild( i0 );

      // Delete it from memory.
      delete child;
      child = NULL;
      }

    // Add missing dataset items of given list.
    for( ;
	 it1!=sortedList.end();
	 ++it1 )
      {
	// qDebug() << "++" << *it1;

	new QTreeWidgetItem( m_DatasetRootItem, QStringList( *it1 ) );
      }
    }
}

/*******************************************************************************/
void
DatabaseBrowserWidget
::SetupUI()
{
  m_DatasetRootItem = m_UI->databaseTreeWidget->topLevelItem( 0 );
  assert( m_DatasetRootItem!=NULL );
  assert( m_DatasetRootItem->text( 0 )==tr( "Datasets" ) );

#if 0
  m_DatasetRootItem->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
#endif

  m_DatasetRootItem->setExpanded( true );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
DatabaseBrowserWidget
::on_databaseTreeWidget_currentItemChanged( QTreeWidgetItem* current,
					    QTreeWidgetItem* previous )
{
  assert( current!=previous );

  qDebug()
    << this
    << "::on_databaseTreeWidget_currentItemChanged("
    << current << "," << previous
    << ")";

  qDebug()
    << ( previous==NULL ? "" : previous->text( 0 ) )
    << "->"
    << ( current==NULL ? "" : current->text( 0 ) );

  emit CurrentDatasetChanged(
    current->parent()!=m_DatasetRootItem
    ? QString()
    : current->text( 0 )
  );
}

} // end namespace 'mvd'
