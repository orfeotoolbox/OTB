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
// #include <cassert>

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
/*****************************************************************************/
StatusBarWidget
::StatusBarWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::StatusBarWidget() )
{
  m_UI->setupUi( this );
}

/*****************************************************************************/
StatusBarWidget
::~StatusBarWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
StatusBarWidget
::SetPixelIndex( const IndexType& pixel, bool isInsideRegion )
{
  m_UI->pixelIndexLineEdit->setText(
    isInsideRegion
    ? QString( "%1, %2" ).arg( pixel[ 0 ] ).arg( pixel[ 1 ] )
    : QString()
  );
}

/*****************************************************************************/
void
StatusBarWidget
::SetScale( double sx, double sy )
{
  // qDebug() << this << "::SetScale(" << sx << "," << sy << ")";

  QString zoomLevel( StatusBarWidget::ZoomLevel( sx ) );

  /*
  if( sx!=sy )
    zoomLevel.append( "/" + StatusBarWidget::ZoomLevel( sy ) );
  */

  m_UI->scaleLineEdit->setText( zoomLevel );
}

/*****************************************************************************/
/* PRIVATE SLOTS                                                             */
/*****************************************************************************/
/*
void
StatusBarWidget
::SetPixelIndexText( const QString& text )
{
  m_UI->pixelIndexLineEdit->setText( text );
}
*/

/*****************************************************************************/
void
StatusBarWidget
::SetPixelRadiometryText( const QString& text )
{
  m_UI->pixelRadiometryLabel->setText( text );
}

/*****************************************************************************/
void
StatusBarWidget
::on_pixelIndexLineEdit_returnPressed()
{
  //
  // Cancel if pixel-index coordinates text is empty.
  if( m_UI->pixelIndexLineEdit->text().isEmpty() )
    return;

  //
  // Split coordinates.
  QStringList coordinates( m_UI->pixelIndexLineEdit->text().split( ',' ) );

  //
  // Check splitted coordinates format.
  assert( coordinates.size()==1 || coordinates.size()==2 );

  if( coordinates.size()!=1 && coordinates.size()!=2 )
    return;

  //
  // Construct resulting pixel-index.
  IndexType index;

  index.Fill( 0 );

  //
  // Convert first pixel-index coordinate.
  bool isOk = true;
  index[ 0 ] = coordinates.front().toUInt( &isOk );

  // assert( isOk );

  if( !isOk )
    return;

  //
  // Convert second pixel-index coordinate.
  if( coordinates.size()>1 )
    {
    index[ 1 ] = coordinates.back().toUInt( &isOk );

    // assert( isOk );

    if( !isOk )
      return;
    }

  //
  // If both pixel-index coordinates have correctly been converted,
  // emit pixel-index changed signal.
  emit PixelIndexChanged( index );
}

/*****************************************************************************/
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
  // assert( scale.size()==1 || scale.size()==2 );

  if( scale.size()!=1 && scale.size()!=2 )
    return;

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

  // assert( isOk );
  // assert( numerator!=0.0 );

  if( !isOk || numerator==0.0 )
    return;

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

    // assert( isOk );

    if( !isOk )
      return;

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

} // end namespace 'mvd'
