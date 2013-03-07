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
#include "mvdColorBandDynamicsWidget.h"
#include "ui_mvdColorBandDynamicsWidget.h"

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
  TRANSLATOR mvd::ColorBandDynamicsWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
ColorBandDynamicsWidget
::ColorBandDynamicsWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ColorBandDynamicsWidget() ),
  m_Channel( RGBA_CHANNEL_RGB )
{
  m_UI->setupUi( this );
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
  return m_UI->lowIntensitySpinBox->minimum();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetMinIntensity( double value )
{
  m_UI->lowIntensitySpinBox->setMinimum( value );
  m_UI->highIntensitySpinBox->setMinimum( value );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetMaxIntensity() const
{
  return m_UI->highIntensitySpinBox->maximum();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetMaxIntensity( double value )
{
  m_UI->lowIntensitySpinBox->setMaximum( value );
  m_UI->highIntensitySpinBox->setMaximum( value );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetLowIntensity() const
{
  return m_UI->lowIntensitySpinBox->value();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetLowIntensity( double value )
{
  m_UI->lowIntensitySpinBox->setValue( value );
}

/*****************************************************************************/
double
ColorBandDynamicsWidget
::GetHighIntensity() const
{
  return m_UI->highIntensitySpinBox->value();
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::SetHighIntensity( double value )
{
  m_UI->highIntensitySpinBox->setValue( value );
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
void
ColorBandDynamicsWidget
::SetChannelLabel( RgbaChannel channel )
{
  m_Channel = channel;
  
  m_UI->rLabel->setVisible(
    channel==RGBA_CHANNEL_RED || channel==RGBA_CHANNEL_ALL );
  
  m_UI->gLabel->setVisible(
    channel==RGBA_CHANNEL_GREEN || channel==RGBA_CHANNEL_ALL );
  
  m_UI->bLabel->setVisible(
    channel==RGBA_CHANNEL_BLUE || channel==RGBA_CHANNEL_ALL );
}

/*****************************************************************************/
RgbaChannel
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
::on_lowIntensitySpinBox_valueChanged( double value )
{
  emit LowIntensityChanged( m_Channel, value );
}

/*****************************************************************************/
void
ColorBandDynamicsWidget
::on_highIntensitySpinBox_valueChanged( double value )
{
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
  emit ApplyAllClicked( m_Channel,
                        m_UI->lowQuantileSpinBox->value(),
                        m_UI->highQuantileSpinBox->value());
}

} // end namespace 'mvd'
