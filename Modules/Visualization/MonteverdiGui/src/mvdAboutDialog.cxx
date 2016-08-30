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
#include "mvdAboutDialog.h"
#include "ui_mvdAboutDialog.h"

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
  TRANSLATOR mvd::AboutDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
AboutDialog
::AboutDialog( QWidget* p, Qt::WindowFlags flags ) :
  QDialog( p, flags ),
  m_UI( new mvd::Ui::AboutDialog() )
{
  m_UI->setupUi( this );

  m_UI->m_ProjectLabel->setText( qApp->applicationName() );
  m_UI->m_VersionLabel->setText( qApp->applicationVersion() );
}

/*******************************************************************************/
AboutDialog
::~AboutDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
