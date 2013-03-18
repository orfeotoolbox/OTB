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
#include "Gui/mvdApplicationsToolBox.h"
#include "Gui/ui_mvdApplicationsToolBox.h"


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


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ApplicationsToolBox
::ApplicationsToolBox( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ApplicationsToolBox() )
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
  QObject::connect(GetAlgorithmsTree(),
                   SIGNAL( itemDoubleClicked(QTreeWidgetItem* ,int) ),
                   this,
                   SLOT( OnAlgorithmTreeDoubleClick(QTreeWidgetItem* ,int) )
    );
}

/*******************************************************************************/
ApplicationsToolBox
::~ApplicationsToolBox()
{
}

/*******************************************************************************/
QTreeWidget *
ApplicationsToolBox
::GetAlgorithmsTree()
{
  return m_UI->m_AlgorithmsTree;
}

/*******************************************************************************/
void
ApplicationsToolBox
::FillTreeUsingTags()
{
  //
  // clear algorithms tree
  GetAlgorithmsTree()->clear();

  if ( m_AppTags.size() > 0 )
    {
    // 
    // main item (title)
    QTreeWidgetItem * mainItem = new QTreeWidgetItem( GetAlgorithmsTree() );
    mainItem->setText(0, tr("Orfeo Toolbox Algorithms"));
    mainItem->setExpanded( !m_SearchText.isEmpty() );

    // 
    // iterate on map: key as high-level item / algorithms as lower-level items
    ApplicationsTagContainer::const_iterator itTag = m_AppTags.begin();
    while( itTag != m_AppTags.end() )
      {
      //
      // current Doctag name
      QString  qcurrentTag( (*itTag).first.c_str() );

      // If a current tag applicaton name match the searcText, add the
      // tag as an item to the tree
      if (m_SearchText.isEmpty() ||  IsSearchTextMatchAnyAlgorithm( qcurrentTag ) )
        {
        //
        // step #1 ->  DocTag is a main item
        QTreeWidgetItem * cmainItem = new QTreeWidgetItem( mainItem );
        cmainItem->setText(0,  qcurrentTag );
        cmainItem->setExpanded( !m_SearchText.isEmpty() );

        //
        // TODO : add category icon

        //
        // step #2 -> Add algorithms name if matching the sear
        StringVector::const_iterator itApps = (*itTag).second.begin();
        while( itApps != (*itTag).second.end() )
          {
          // get current app name
          QString  qcurrentAlg( (*itApps).c_str() );        
        
          // does the current algorithm match the search text
          if ( m_SearchText.isEmpty() ||
               qcurrentAlg.contains(m_SearchText ,Qt::CaseInsensitive) )
            {
            // 
            // set current application name as secondary item
            QTreeWidgetItem * secItem = new QTreeWidgetItem( cmainItem );
            secItem->setText(0, qcurrentAlg);
          
            //
            // TODO : add algorithm icon
            //secItem->setIcon(0, QIcon( ":" ));
            }

          ++itApps;
          }
        }

      ++itTag;
      }

    }
}

/*******************************************************************************/
bool
ApplicationsToolBox
::IsSearchTextMatchAnyAlgorithm(const QString & tagName )
{
  bool res = false;
  
  // find the pair corresponding to the tagName
  ApplicationsTagContainer::const_iterator itTag = m_AppTags.find( ToStdString( tagName ) );
  if( itTag != m_AppTags.end() )
    {
    // iterate on the alg names relative to this tag
    StringVector::const_iterator itApps = (*itTag).second.begin();
    while( itApps != (*itTag).second.end() )
      {

      // get current app name
      QString  qcurrentAlgItem( (*itApps).c_str() );        
      
      if ( qcurrentAlgItem.contains(m_SearchText ,Qt::CaseInsensitive) )
        {
        return true;
        }
      ++itApps;
      }

    }
  
  return res;
}

/*******************************************************************************/
void
ApplicationsToolBox
::ExecuteAlgorithm( const QString& appName )
{
  emit ApplicationToLaunchSelected(appName);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ApplicationsToolBox
::OnAvailableApplicationsTagsChanged(const ApplicationsTagContainer& appsTags )
{
  // rememeber the map
  m_AppTags = appsTags;
  
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
  //
  // Execute algorithm : check  if the
  // item is low-level ->does not any child
  if ( item->childCount() == 0 )
    {
    QString appName = item->text( column );
  
    // 
    // Execute the algorithm
    if ( !appName.isEmpty() )
      {
      ExecuteAlgorithm( appName );
      }
    }
}

/*******************************************************************************/

} // end namespace 'mvd'
