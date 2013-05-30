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
#include "Gui/mvdColorDynamicsWidget.h"
#include "Gui/ui_mvdColorDynamicsWidget.h"

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

const char*
ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[] = {
  "redWidget",
  "greenWidget",
  "blueWidget",
  "whiteWidget"
};

/*******************************************************************************/
ColorDynamicsWidget
::ColorDynamicsWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ColorDynamicsWidget() ),
  m_IsGrayscaleActivated( false )
{
  m_UI->setupUi( this );

  CountType begin;
  CountType end;

  RgbwBounds( begin, end, RGBW_CHANNEL_ALL );

  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel( static_cast< RgbwChannel >( i ) );

    ConnectChild( GetChannel( channel ), channel );
    }

  SetGrayscaleActivated( false );
}

/*******************************************************************************/
ColorDynamicsWidget
::~ColorDynamicsWidget()
{
}

/*****************************************************************************/
void
ColorDynamicsWidget
::SetGrayscaleActivated( bool activated )
{
  m_IsGrayscaleActivated = activated;

  CountType begin;
  CountType end;

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
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*****************************************************************************/

} // end namespace 'mvd'
