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
#include "mvdApplicationsToolBox.h"
#include "ui_mvdApplicationsToolBox.h"


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
  TRANSLATOR mvd::ApplicationsToolBox

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

enum COLUMN
{
  COLUMN_NAME = 0,
  COLUMN_TITLE,
  //
  COLUMN_COUNT,
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ApplicationsToolBox
::ApplicationsToolBox( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ApplicationsToolBox() ),
  m_AppTags(),
  m_AppsDocNameToNameMap(),
  m_SearchText()
{
  m_UI->setupUi( this );

  //
  // connect search box changed
  QObject::connect(m_UI->m_SearchLine,
                   SIGNAL( textChanged(const QString &) ),
                   this,
                   SLOT( OnSearchBoxChanged(const QString &) )
    );

  //
  // connect double click on the tree Widget
  QObject::connect( m_UI->m_AlgorithmsTree,
                   SIGNAL( itemDoubleClicked(QTreeWidgetItem* ,int) ),
                   this,
                   SLOT( OnAlgorithmTreeDoubleClick(QTreeWidgetItem* ,int) )
    );

  // put a place holder text in search area
#if (QT_VERSION >= 0x407000)
  m_UI->m_SearchLine->setPlaceholderText( tr("Search Application ...") );
#endif
}

/*******************************************************************************/
ApplicationsToolBox
::~ApplicationsToolBox()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
// QTreeWidget *
// ApplicationsToolBox
// ::GetAlgorithmsTree()
// {
//   return m_UI->m_AlgorithmsTree;
// }

/*******************************************************************************/
void
ApplicationsToolBox
::FillTreeUsingTags()
{
  assert( m_UI->m_AlgorithmsTree!=NULL );

  //
  // clear algorithms tree
  m_UI->m_AlgorithmsTree->clear();

  if( m_AppTags.empty() )
    return;

  // 
  // main item (title)
  QTreeWidgetItem * mainItem = new QTreeWidgetItem( m_UI->m_AlgorithmsTree );

  mainItem->setText( COLUMN_NAME, tr( "Orfeo Toolbox Algorithms" ) );
  mainItem->setExpanded( !m_SearchText.isEmpty() );

  QString search( m_SearchText.simplified() );

  // 
  // iterate on map: key as high-level item / algorithms as lower-level items
  for( ApplicationsTagContainer::const_iterator itTag( m_AppTags.begin() );
       itTag!=m_AppTags.end();
       ++itTag )
    {
    //
    // current Doctag name
    QString  qcurrentTag( (*itTag).first.c_str() );

    // If a current tag applicaton name match the searchText, add the
    // tag as an item to the tree
    if (m_SearchText.isEmpty() ||  
	IsSearchTextMatchAnyAlgorithm( qcurrentTag, search ) || 
	qcurrentTag.contains( search, Qt::CaseInsensitive ) )
      {
      //
      // step #1 ->  DocTag is a main item
      QTreeWidgetItem * cmainItem = new QTreeWidgetItem( mainItem );
      cmainItem->setText( COLUMN_NAME,  qcurrentTag );
      cmainItem->setExpanded( !m_SearchText.isEmpty() );

      //
      // add category icon
      cmainItem->setIcon( COLUMN_NAME, QIcon( ":/icons/otb" ) );

      //
      // step #2 -> Add algorithms name if matching the search label
      for( StringVector::const_iterator itApps( itTag->second.begin() );
	   itApps != itTag->second.end();
	   ++itApps )
	{
	// get current app name
	QString  name( itApps->c_str() );

	//  get current app DocName
	QString title(
	  GetApplicationDocNameByApplicationName( name )
	);

	// does the current algorithm DocName match the search text
	if ( m_SearchText.isEmpty() ||
	     title.contains( search, Qt::CaseInsensitive ) ||
	     name.contains( search, Qt::CaseInsensitive ) )
	  {
	  // 
	  // set current application name as secondary item
	  QTreeWidgetItem * secItem = new QTreeWidgetItem( cmainItem );

	  secItem->setText( COLUMN_NAME, name );
	  secItem->setText( COLUMN_TITLE, title );

	  secItem->setToolTip( COLUMN_NAME, title );

	  secItem->setIcon( COLUMN_NAME, QIcon( ":/icons/process" ) );
	  }
	}
      }
    }

  // Expand algorithms tree by default
  m_UI->m_AlgorithmsTree->expandAll();

  m_UI->m_AlgorithmsTree->resizeColumnToContents( COLUMN_NAME );
  m_UI->m_AlgorithmsTree->resizeColumnToContents( COLUMN_TITLE );
}

/*******************************************************************************/
bool
ApplicationsToolBox
::IsSearchTextMatchAnyAlgorithm( const QString & tagName, const QString & search )
{
  // find the pair corresponding to the tagName
  ApplicationsTagContainer::const_iterator itTag(
    m_AppTags.find(
      ToStdString( tagName )
    )
  );

  if( itTag==m_AppTags.end() )
    return false;

  for( StringVector::const_iterator itApps( itTag->second.begin() );
       itApps != itTag->second.end();
       ++itApps )
    {
    QString name( FromStdString( *itApps ) );

    if( name.contains( search, Qt::CaseInsensitive ) ||
	GetApplicationDocNameByApplicationName( name )
	.contains( search, Qt::CaseInsensitive ) )
      return true;
    }

  return false;
}

/*******************************************************************************/
void
ApplicationsToolBox
::LaunchApplication( const QString& appName )
{
  emit ApplicationToLaunchSelected(
    appName,
    GetApplicationDocNameByApplicationName( appName )
  );
}

/*******************************************************************************/
QString
ApplicationsToolBox
::GetApplicationDocNameByApplicationName( const QString & appName )
{
  QString docName("");

  // find the pair corresponding to the tagName
  ApplicationDocNameToNameMap::const_iterator itDocNames = m_AppsDocNameToNameMap.begin();

  while( itDocNames != m_AppsDocNameToNameMap.end() )
    {
    
    // retrieve the appName in the map
    if ( appName == QString( (*itDocNames).second.c_str() ) )
      {
      //
      // return the relative docName
      return QString ( (*itDocNames).first.c_str() );
      }

    ++itDocNames;
    }

  return docName;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ApplicationsToolBox
::OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer& appsTags, 
                                     const ApplicationDocNameToNameMap& docNameToNameMap)
{
  // rememeber the map
  m_AppTags = appsTags;

  // remember the OTB applications  docName <-> Name association
  m_AppsDocNameToNameMap = docNameToNameMap;
  
  // fill the tree with the application
  FillTreeUsingTags();  
}

/*******************************************************************************/
void
ApplicationsToolBox
::OnSearchBoxChanged(const QString & search)
{
  // 
  // get the search text
  m_SearchText = search;
  
  // fill the tree with the application
  FillTreeUsingTags();
}

/*******************************************************************************/
void
ApplicationsToolBox
::OnAlgorithmTreeDoubleClick( QTreeWidgetItem * item , int column )
{
  // qDebug() << item << column;

  if( item->childCount() > 0 )
    return;

  QString text( item->text( column ) );

  if( text.isEmpty() )
    return;

  switch( column )
    {
    case COLUMN_NAME:
      break;

    case COLUMN_TITLE:
      {
      ApplicationDocNameToNameMap::const_iterator it(
	m_AppsDocNameToNameMap.find(
	  ToStdString( text )
	)
      );

      assert( it!=m_AppsDocNameToNameMap.end() );

      assert( it->second.empty() );

      text = FromStdString( it->second );
      }
      break;

    default:
      assert( false && "Unexpected enum value." );
      break;
    }

  LaunchApplication( text );
}

/*******************************************************************************/

} // end namespace 'mvd'
