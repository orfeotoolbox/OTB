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
#include "mvdSearchableTreeWidget.h"
#include "ui_mvdSearchableTreeWidget.h"


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


#define ENABLE_DISPLAY_ID 1

namespace mvd
{

/*
  TRANSLATOR mvd::SearchableTreeWidget

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
SearchableTreeWidget
::SearchableTreeWidget( QWidget* parent, Qt::WindowFlags flags ) :
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::SearchableTreeWidget() )
{
  m_UI->setupUi( this );

  SetupUI();
}

/*******************************************************************************/
SearchableTreeWidget
::~SearchableTreeWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
SearchableTreeWidget
::SetupUI()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
/*
void
SearchableTreeWidget
::on_treeWidget_currentItemChanged( QTreeWidgetItem* current,
                                    QTreeWidgetItem* previous )
{
}
*/

/*******************************************************************************/
/*
void
SearchableTreeWidget
::on_searchLine_textChanged( const QString& text )
{
}
*/

} // end namespace 'mvd'
