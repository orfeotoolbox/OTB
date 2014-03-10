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
#include "Gui/mvdStatusBarWidget.h"
#include "Gui/ui_mvdStatusBarWidget.h"


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

namespace mvd
{
/*
  TRANSLATOR mvd::StatusBarWidget

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
StatusBarWidget
::StatusBarWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::StatusBarWidget() )
{
  m_UI->setupUi( this );
}

/*******************************************************************************/
StatusBarWidget
::~StatusBarWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
QLineEdit*
StatusBarWidget
::GetCurrentPixelIndexWidget()
{
  return m_UI->pixelIndexLineEdit;
}

/*******************************************************************************/
QLabel*
StatusBarWidget
::GetCurrentPixelRadioWidget()
{
  return m_UI->pixelRadiometryLabel;
}

/*******************************************************************************/
#if USE_OLD_IMAGE_VIEW

QLineEdit * 
StatusBarWidget
::GetCurrentScaleWidget()
{
    return m_UI->scaleLineEdit;
}

#endif // USE_OLD_IMAGE_VIEW

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
StatusBarWidget
::OnScaleChanged( double sx, double sy )
{
  // qDebug() << this << "::OnScaleChanged(" << sx << "," << sy << ")";

  QString zoomLevel( StatusBarWidget::ZoomLevel( sx ) );

  if( sx!=sy )
    zoomLevel.append( StatusBarWidget::ZoomLevel( sy ) );

  m_UI->scaleLineEdit->setText( zoomLevel );
}

} // end namespace 'mvd'
