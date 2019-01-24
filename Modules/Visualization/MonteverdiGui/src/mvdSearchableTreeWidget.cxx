/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
::SearchableTreeWidget( QWidget* p, Qt::WindowFlags flags ) :
  QWidget( p, flags ),
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
