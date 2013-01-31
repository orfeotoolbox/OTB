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

  for( int i=0; i<RGBA_CHANNEL_ALL; ++i )
    {
    RgbaChannel channel( static_cast< RgbaChannel >( i ) );
    GetChannel( channel )->SetChannelLabel( channel );
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
