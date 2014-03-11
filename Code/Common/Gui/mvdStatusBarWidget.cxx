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
#include "mvdStatusBarWidget.h"
#include "ui_mvdStatusBarWidget.h"


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
#if USE_OLD_IMAGE_VIEW

QLineEdit*
StatusBarWidget
::GetCurrentPixelIndexWidget()
{
  return m_UI->pixelIndexLineEdit;
}

#endif // USE_OLD_IMAGE_VIEW

/*******************************************************************************/
#if USE_OLD_IMAGE_VIEW

QLabel*
StatusBarWidget
::GetCurrentPixelRadioWidget()
{
  return m_UI->pixelRadiometryLabel;
}

#endif // USE_OLD_IMAGE_VIEW

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
void
StatusBarWidget
::SetPixelIndex( const IndexType& pixel, bool isInsideRegion )
{
  SetPixelIndexText(
    isInsideRegion
    ? QString( "%1, %2" ).arg( pixel[ 0 ] ).arg( pixel[ 1 ] )
    : QString()
  );
}

/*******************************************************************************/
void
StatusBarWidget
::SetPixelIndexText( const QString& text )
{
  m_UI->pixelIndexLineEdit->setText( text );
}

/*******************************************************************************/
void
StatusBarWidget
::SetPixelRadiometryText( const QString& text )
{
  m_UI->pixelRadiometryLabel->setText( text );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
StatusBarWidget
::on_scaleLineEdit_editingFinished()
{
  //
  // Cancel if scale text is empty.
  if( m_UI->scaleLineEdit->text().isEmpty() )
    return;

  //
  // Split scale text.
  QStringList scale( m_UI->scaleLineEdit->text().split( ':' ) );

  //
  // Check scale text format.
  assert( scale.size()==1 || scale.size()==2 );

  /*
  if( scale.size()!=1 && scale.size()!=2 )
    {
    throw std::invalid_argument(
      ToStdString(
        tr( "Invalid argument: '%1' should be scale of the form <numerator>[:<denominator>] with numerator and denominator being real numbers." )
      )
    );
    }
  */

  //
  // Convert scale numerator.
  bool isOk = true;
  double numerator = scale.front().toDouble( &isOk );

  assert( isOk );
  assert( numerator!=0.0 );

  /*
  if( !isOk )
    {
    throw std::invalid_argument(
      ToStdString(
        tr( "Invalid argument: '%1' should be scale of the form <numerator>[:<denominator>] with numerator and denominator being real numbers." )
      )
    );
    }
  */

  //
  // Convert scale denominator.
  double denominator = 1.0;

  if( scale.size()>1 )
    {
    denominator = scale.back().toDouble( &isOk );

    assert( isOk );

    /*
    if( !isOk )
      {
      throw std::invalid_argument(
        ToStdString(
          tr( "Invalid argument: '%1' should be scale of the form <numerator>[:<denominator>] with numerator and denominator being real numbers." )
        )
      );
      }
    */
    }

  //
  // Emit scale changed.
  emit ScaleChanged( numerator / denominator );
}

/*******************************************************************************/
void
StatusBarWidget
::OnScaleChanged( double sx, double sy )
{
  // qDebug() << this << "::OnScaleChanged(" << sx << "," << sy << ")";

  QString zoomLevel( StatusBarWidget::ZoomLevel( sx ) );

  /*
  if( sx!=sy )
    zoomLevel.append( "/" + StatusBarWidget::ZoomLevel( sy ) );
  */

  m_UI->scaleLineEdit->setText( zoomLevel );
}

} // end namespace 'mvd'
