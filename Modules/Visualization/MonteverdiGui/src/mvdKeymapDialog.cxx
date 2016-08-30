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
#include "mvdKeymapDialog.h"
#include "ui_mvdKeymapDialog.h"

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

namespace mvd
{

/*
  TRANSLATOR mvd::KeymapDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
KeymapDialog
::KeymapDialog( QWidget* p, Qt::WindowFlags flags ) :
  QDialog( p, flags ),
  m_UI( new mvd::Ui::KeymapDialog() )
{
  m_UI->setupUi( this );

  m_UI->m_ProjectLabel->setText( PROJECT_NAME );
  m_UI->m_VersionLabel->setText( Monteverdi_VERSION_STRING );
}

/*******************************************************************************/
KeymapDialog
::~KeymapDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
