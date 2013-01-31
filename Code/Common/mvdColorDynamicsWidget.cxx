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

const char*
ColorDynamicsWidget::COLOR_BAND_DYNAMICS_WIDGET_NAMES[] = {
  "redWidget",
  "greenWidget",
  "blueWidget"
};

/*******************************************************************************/
ColorDynamicsWidget
::ColorDynamicsWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::ColorDynamicsWidget() )
{
  m_UI->setupUi( this );

  for( int i=0; i<RGBA_CHANNEL_ALPHA; ++i )
    {
    RgbaChannel channel( static_cast< RgbaChannel >( i ) );

    ColorBandDynamicsWidget* widget = GetChannel( channel );

    widget->SetChannelLabel( channel );

    //
    // Concentrate and forward signals of each channels.

    QObject::connect(
      widget,
      SIGNAL( LowQuantileChanged( RgbaChannel, double ) ),
      // TO:
      this,
      SIGNAL( LowQuantileChanged( RgbaChannel, double ) )
    );

    QObject::connect(
      widget,
      SIGNAL( HighQuantileChanged( RgbaChannel, double ) ),
      // TO:
      this,
      SIGNAL( HighQuantileChanged( RgbaChannel, double ) )
    );

    QObject::connect(
      widget,
      SIGNAL( LowIntensityChanged( RgbaChannel, double ) ),
      // TO:
      this,
      SIGNAL( LowIntensityChanged( RgbaChannel, double ) )
    );

    QObject::connect(
      widget,
      SIGNAL( HighIntensityChanged( RgbaChannel, double ) ),
      // TO:
      this,
      SIGNAL( HighIntensityChanged( RgbaChannel, double ) )
    );

    QObject::connect(
      widget,
      SIGNAL( ResetQuantileClicked( RgbaChannel ) ),
      // TO:
      this,
      SIGNAL( ResetQuantileClicked( RgbaChannel ) )
    );

    QObject::connect(
      widget,
      SIGNAL( ResetIntensityClicked( RgbaChannel ) ),
      // TO:
      this,
      SIGNAL( ResetIntensityClicked( RgbaChannel  ) )
    );
    }
}

/*******************************************************************************/
ColorDynamicsWidget
::~ColorDynamicsWidget()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*****************************************************************************/

} // end namespace 'mvd'
