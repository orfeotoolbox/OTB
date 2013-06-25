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
#include "Gui/mvdColorBandDynamicsWidget.h"
#include "Gui/ui_mvdColorBandDynamicsWidget.h"

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
#include "Gui/mvdDoubleValidator.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ColorBandDynamicsWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
ColorBandDynamicsWidget
::ColorBandDynamicsWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ColorBandDynamicsWidget() ),
  m_LowIntensityValidator( NULL ),
  m_HighIntensityValidator( NULL ),
  m_Channel( RGBW_CHANNEL_RGB )
{
  m_UI->setupUi( this );

  m_LowIntensityValidator = new DoubleValidator( m_UI->lowIntensityLineEdit );
  m_UI->lowIntensityLineEdit->setValidator( m_LowIntensityValidator );

  m_HighIntensityValidator = new DoubleValidator( m_UI->highIntensityLineEdit );
  m_UI->highIntensityLineEdit->setValidator( m_HighIntensityValidator );
}

/*******************************************************************************/
ColorBandDynamicsWidget
::~ColorBandDynamicsWidget()
{
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetMinIntensity() const
{
  return m_UI->lowIntensityLineEdit->text().toDouble();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetMinIntensity( double value )
{
  m_LowIntensityValidator->setBottom( value );
  m_HighIntensityValidator->setBottom( value );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetMaxIntensity() const
{
  return m_LowIntensityValidator->bottom();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetMaxIntensity( double value )
{
  m_LowIntensityValidator->setTop( value );
  m_HighIntensityValidator->setTop( value );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetLowIntensity() const
{
  return m_HighIntensityValidator->top();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetLowIntensity( double value )
{
  QString number(
    QString::number( value, 'g', m_LowIntensityValidator->decimals() )
  );

  assert( !number.isEmpty() );

  m_UI->lowIntensityLineEdit->setText( number );

  m_UI->lowIntensityLineEdit->setCursorPosition( 0 );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetHighIntensity() const
{
  return m_UI->highIntensityLineEdit->text().toDouble();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetHighIntensity( double value )
{
  QString number(
    QString::number( value, 'g', m_HighIntensityValidator->decimals() )
  );

  assert( !number.isEmpty() );

  m_UI->highIntensityLineEdit->setText( number );

  m_UI->highIntensityLineEdit->setCursorPosition( 0 );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetLowQuantile() const
{
  return m_UI->lowQuantileSpinBox->value();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetLowQuantile( double value )
{
  m_UI->lowQuantileSpinBox->setValue( value );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetHighQuantile() const
{
  return m_UI->highQuantileSpinBox->value();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetHighQuantile( double value )
{
  m_UI->highQuantileSpinBox->setValue( value );
}

/*****************************************************************************/
bool
ColorBandDynamicsWidget
::IsBounded() const
{
  return m_UI->linkButton->isChecked();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetBounded( bool enabled )
{
  return m_UI->linkButton->setChecked( enabled );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetChannelLabel( RgbwChannel channel )
{
  m_Channel = channel;
  
  m_UI->rLabel->setVisible(
    channel==RGBW_CHANNEL_RED ||
    channel==RGBW_CHANNEL_RGB ||
    channel==RGBW_CHANNEL_ALL
  );

  m_UI->gLabel->setVisible(
    channel==RGBW_CHANNEL_GREEN ||
    channel==RGBW_CHANNEL_RGB ||
    channel==RGBW_CHANNEL_ALL
  );

  m_UI->bLabel->setVisible(
    channel==RGBW_CHANNEL_BLUE ||
    channel==RGBW_CHANNEL_RGB ||
    channel==RGBW_CHANNEL_ALL
  );

  m_UI->wLabel->setVisible( channel==RGBW_CHANNEL_WHITE );

  m_UI->applyAllButton->setEnabled(
    channel==RGBW_CHANNEL_RED ||
    channel==RGBW_CHANNEL_GREEN ||
    channel==RGBW_CHANNEL_BLUE ||
    channel==RGBW_CHANNEL_RGB
  );
}

/*****************************************************************************/
RgbwChannel
ColorBandDynamicsWidget
::GetChannelLabel() const
{
  return m_Channel;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*****************************************************************************/

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_lowIntensityLineEdit_textChanged( const QString& text )
{
  bool isOk = true;
  double value = text.toDouble( &isOk );

  if( !isOk )
    {
#if 0
    throw std::invalid_argument(
      ToStdString( tr( "Invalid argument '%1'." ).arg( text ) )
    );
#else
    return;
#endif
    }

  emit LowIntensityChanged( m_Channel, value );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_highIntensityLineEdit_textChanged( const QString& text )
{
  bool isOk = true;
  double value = text.toDouble( &isOk );

  if( !isOk )
    {
#if 0
    throw std::invalid_argument(
      ToStdString( tr( "Invalid argument '%1'." ).arg( text ) )
    );
#else
    return;
#endif
    }

  emit HighIntensityChanged( m_Channel, value );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_lowQuantileSpinBox_valueChanged( double value )
{
  emit LowQuantileChanged( m_Channel, value );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_highQuantileSpinBox_valueChanged( double value )
{
  emit HighQuantileChanged( m_Channel, value );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_minMaxButton_clicked()
{
  emit ResetIntensityClicked( m_Channel );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_defaultsButton_clicked()
{
  emit ResetQuantileClicked( m_Channel );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_applyAllButton_clicked()
{
  emit ApplyAllClicked(
    m_Channel,
    m_UI->lowQuantileSpinBox->value(),
    m_UI->highQuantileSpinBox->value()
  );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_linkButton_toggled( bool checked )
{
  /*
  if( checked )
    {
    QString text( m_UI->lowIntensityLineEdit->text() );
    m_LowIntensityValidator->fixup( text );
    m_UI->lowIntensityLineEdit->setText( text );

    text = m_UI->lowIntensityLineEdit->text();
    m_HighIntensityValidator->fixup( text );
    m_UI->highIntensityLineEdit->setText( text );
    }
  */

  emit LinkToggled( m_Channel, checked );
}

} // end namespace 'mvd'
