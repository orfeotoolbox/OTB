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
::SetupUI()
{
  m_DatasetRootItem = m_UI->databaseTreeWidget->topLevelItem( 0 );
  assert( m_DatasetRootItem!=NULL );
  assert( m_DatasetRootItem->text( 0 )==tr( "Datasets" ) );

  m_DatasetRootItem->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
