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
#include "mvdLayerStackWidget.h"
#include "ui_mvdLayerStackWidget.h"


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
#include "Gui/mvdLayerStackItemModel.h"

namespace mvd
{

/*
  TRANSLATOR mvd::LayerStackWidget

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
LayerStackWidget
::LayerStackWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::LayerStackWidget() )
{
  m_UI->setupUi( this );
}

/*******************************************************************************/
LayerStackWidget
::~LayerStackWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
// void
// LayerStackWidget
// ::SetModel( LayerStackItemModel * itemModel )
// {
//   // See http://qt-project.org/doc/qt-4.8/qabstractitemview.html#setModel .
//   QItemSelectionModel * itemSelectionModel = m_UI->treeView->selectionModel();

//   m_UI->treeView->setModel( itemModel );

//   itemModel->setParent( m_UI->treeView );

//   delete itemSelectionModel;
//   itemSelectionModel = NULL;
// }

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
