/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "mvdColorDynamicsWidget.h"
#include "ui_mvdColorDynamicsWidget.h"

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
  TRANSLATOR mvd::ColorDynamicsWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

const char*
ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[] = {
  "redWidget",
  "greenWidget",
  "blueWidget",
  "whiteWidget"
};

const double GAMMA_FACTOR = -0.25;
const double GAMMA_POWER = 1.1;

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */


/*****************************************************************************/
ColorDynamicsWidget
::ColorDynamicsWidget( QWidget* p, Qt::WindowFlags flags  ):
  QWidget( p, flags ),
  m_UI( new mvd::Ui::ColorDynamicsWidget() ),
  m_NoDataValidator( NULL ),
  m_IsGrayscaleActivated( false )
{
  //
  // Qt setup.
  m_UI->setupUi( this );

  //
  // Colors.
  CountType begin;
  CountType end;

  RgbwBounds( begin, end, RGBW_CHANNEL_ALL );

  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel( static_cast< RgbwChannel >( i ) );

    ConnectChild( GetChannel( channel ), channel );
    }

  SetGrayscaleActivated( false );

  //
  // No-data.
  m_NoDataValidator = new QDoubleValidator( m_UI->noDataLineEdit );

  m_UI->noDataLineEdit->setValidator( m_NoDataValidator );
}

/*******************************************************************************/
ColorDynamicsWidget
::~ColorDynamicsWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
ColorDynamicsWidget
::SetGrayscaleActivated( bool activated )
{
  m_IsGrayscaleActivated = activated;

  //
  // First, force WHITE channel to be invisible.
  //
  // Cause: prevent layout re-calculation to be resized taking RGB+W
  // into account when switching from grayscale-mode activated to
  // non-activated.
  if( !activated )
    {
    GetChannel( RGBW_CHANNEL_WHITE )->setVisible( false );
    }

  //
  // Then, show/hide relevant components.
  CountType begin;
  CountType end;

  RgbwBounds( begin, end, RGBW_CHANNEL_RGB );

  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel( static_cast< RgbwChannel >( i ) );

    GetChannel( channel )->setVisible( !activated );
    }

  GetChannel( RGBW_CHANNEL_WHITE )->setVisible( activated );

  m_UI->bwLine->setVisible( false );
  m_UI->rgLine->setVisible( !activated );
  m_UI->gbLine->setVisible( !activated );
}

/*******************************************************************************/
bool
ColorDynamicsWidget
::IsNoDataChecked() const
{
  return m_UI->noDataCheckBox->isChecked();
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetNoDataChecked( bool checked )
{
  m_UI->noDataCheckBox->setChecked( checked );
}

/*******************************************************************************/
double
ColorDynamicsWidget
::GetNoDataValue() const
{
  bool isOk = true;
  double value = m_UI->noDataLineEdit->text().toDouble( &isOk );

  if( !isOk )
    {
    }

  return value;
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetNoDataValue( double value )
{
  QString number(
    QString::number( value, 'g', m_NoDataValidator->decimals() ) );

  m_UI->noDataLineEdit->setText( number );
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetGamma( double value )
{
  int gamma =
    itk::Math::Round< int, double >(
      std::log( value ) / (GAMMA_FACTOR * std::log( GAMMA_POWER ) )
    );

  int min = GetMinGamma();

  if( gamma<min )
    gamma = min;

  int max = GetMaxGamma();

  if( gamma>max )
    gamma = max;

  // qDebug() << "::SetGamma(" << value << "): " << gamma;

  SetGammaCursorPosition( gamma );
}

/*******************************************************************************/
double
ColorDynamicsWidget
::GetGamma() const
{
  // qDebug() <<
  //   "::GetGamma(" << GetGammaCursorPosition() << "): " <<
  //   std::pow(
  //     GAMMA_POWER,
  //     GAMMA_FACTOR * static_cast< double >( GetGammaCursorPosition() )
  //   );

  return
    std::pow(
      GAMMA_POWER,
      GAMMA_FACTOR * static_cast< double >( GetGammaCursorPosition() )
    );
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetGammaCursorPosition( int value )
{
  m_UI->gammaSlider->setValue( value );
}

/*******************************************************************************/
int
ColorDynamicsWidget
::GetGammaCursorPosition() const
{
  return m_UI->gammaSlider->value();
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetMinGamma( int value )
{
  m_UI->gammaSlider->setMinimum( value );
}

/*******************************************************************************/
int 
ColorDynamicsWidget
::GetMinGamma() const
{
  return m_UI->gammaSlider->minimum();
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetMaxGamma( int value )
{
  m_UI->gammaSlider->setMaximum( value );
}

/*******************************************************************************/
int 
ColorDynamicsWidget
::GetMaxGamma() const
{
  return m_UI->gammaSlider->maximum();
}

/*******************************************************************************/
void
ColorDynamicsWidget
::SetGammaStep( int value )
{
  m_UI->gammaSlider->setPageStep( value );
}

/*******************************************************************************/
int 
ColorDynamicsWidget
::GetGammaStep() const
{
  return m_UI->gammaSlider->pageStep();
}

/*******************************************************************************/
void
ColorDynamicsWidget
::ConnectChild( ColorBandDynamicsWidget* child, RgbwChannel channel )
{
  child->SetChannelLabel( channel );

  //
  // Concentrate and forward signals of each channels.
  QObject::connect(
    child,
    SIGNAL( LowQuantileChanged( RgbwChannel, double ) ),
    // TO:
    this,
    SIGNAL( LowQuantileChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    child,
    SIGNAL( HighQuantileChanged( RgbwChannel, double ) ),
    // TO:
    this,
    SIGNAL( HighQuantileChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    child,
    SIGNAL( LowIntensityChanged( RgbwChannel, double ) ),
    // TO:
    this,
    SIGNAL( LowIntensityChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    child,
    SIGNAL( HighIntensityChanged( RgbwChannel, double ) ),
    // TO:
    this,
    SIGNAL( HighIntensityChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    child,
    SIGNAL( ResetQuantileClicked( RgbwChannel ) ),
    // TO:
    this,
    SIGNAL( ResetQuantileClicked( RgbwChannel ) )
  );

  QObject::connect(
    child,
    SIGNAL( ResetIntensityClicked( RgbwChannel ) ),
    // TO:
    this,
    SIGNAL( ResetIntensityClicked( RgbwChannel  ) )
  );

  QObject::connect(
    child,
    SIGNAL( ApplyAllClicked( RgbwChannel, double, double ) ),
    // TO:
    this,
    SIGNAL( ApplyAllClicked( RgbwChannel, double, double ) )
  );

  QObject::connect(
    child,
    SIGNAL( LinkToggled( RgbwChannel, bool ) ),
    // TO:
    this,
    SIGNAL( LinkToggled( RgbwChannel, bool ) )
  );
}

/*****************************************************************************/
/* PUBLIC SLOTS                                                              */
/*****************************************************************************/
void
ColorDynamicsWidget
::SetNoDataButtonChecked( bool checked )
{
  if( checked )
    {
    m_UI->noDataButton->setEnabled( false );
    m_UI->noDataButton->setChecked( true );
    // m_UI->noDataButton->setText( tr( "Done" ) );
    }
  else
    {
    m_UI->noDataButton->setEnabled( true );
    m_UI->noDataButton->setChecked( false );
    // m_UI->noDataButton->setText( tr( "GO" ) );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*****************************************************************************/
void
ColorDynamicsWidget
::on_noDataCheckBox_toggled( bool enabled )
{
  emit NoDataFlagToggled( enabled );
}

/*****************************************************************************/
void
ColorDynamicsWidget
::on_noDataLineEdit_textChanged( const QString& text )
{
  bool isOk = true;
  double value = text.toDouble( &isOk );

  if( !isOk )
    {
    }

  emit NoDataValueChanged( value );
}

/*****************************************************************************/
void
ColorDynamicsWidget
::on_noDataButton_toggled( bool checked )
{
  bool thisSB = this->blockSignals( true );
  {
  bool uiSB = m_UI->noDataButton->blockSignals( true );
  {
  SetNoDataButtonChecked( checked ); 
  }
  m_UI->noDataButton->blockSignals( uiSB );
  }
  this->blockSignals( thisSB );

  if( checked )
    {
    emit NoDataButtonPressed();
    }
}

/*****************************************************************************/
void
ColorDynamicsWidget
::on_gammaSlider_valueChanged( int value )
{
  emit GammaCursorPositionChanged( value );

  emit GammaValueChanged( GetGamma() );

  //Display Gamma value as a tooltip when value changed 
  QToolTip::showText(mapToGlobal(m_UI->gammaSlider->pos()),tr("Gamma: ") % QString::number(GetGamma()) );
}

void
ColorDynamicsWidget
::on_gammaResetButton_clicked()
{
  this->SetGamma(1.0);
}

} // end namespace 'mvd'
