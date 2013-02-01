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
#include "mvdColorDynamicsController.h"


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
#include "mvdColorDynamicsWidget.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ColorDynamicsController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ColorDynamicsController
::ColorDynamicsController( ColorDynamicsWidget* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
ColorDynamicsController
::~ColorDynamicsController()
{
}

/*******************************************************************************/
void
ColorDynamicsController
::Connect( AbstractModel* model )
{
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  /*
  VectorImageModel* vectorImageModel =
    qobject_cast< VectorImageModel* >( model );
  */

  // Connect newly selected model to UI controller.
  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( LowQuantileChanged( RgbaChannel, double ) ),
    // to:
    this,
    SLOT( OnLowQuantileChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( HighQuantileChanged( RgbaChannel, double ) ),
    // to:
    this,
    SLOT( OnHighQuantileChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( LowIntensityChanged( RgbaChannel, double ) ),
    // to:
    this,
    SLOT( OnLowIntensityChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( HighIntensityChanged( RgbaChannel, double ) ),
    // to:
    this,
    SLOT( OnHighIntensityChanged( RgbaChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ResetQuantileClicked( RgbaChannel ) ),
    // to:
    this,
    SLOT( OnResetQuantileClicked( RgbaChannel ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ResetIntensityClicked( RgbaChannel ) ),
    // to:
    this,
    SLOT( OnResetIntensityClicked( RgbaChannel  ) )
  );

  // Setup color-dynamics controller.
  colorDynamicsWidget->blockSignals( true );
  colorDynamicsWidget->blockSignals( false );
}

/*******************************************************************************/
void
ColorDynamicsController
::Disconnect( AbstractModel* model )
{
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  /*
  VectorImageModel* vectorImageModel =
    qobject_cast< VectorImageModel* >( model );
  */

  // Disconnect previously selected model to UI controller.
  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LowQuantileChanged( RgbaChannel, double ) ),
    // from:
    this,
    SLOT( OnLowQuantileChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( HighQuantileChanged( RgbaChannel, double ) ),
    // from:
    this,
    SLOT( OnHighQuantileChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LowIntensityChanged( RgbaChannel, double ) ),
    // from:
    this,
    SLOT( OnLowIntensityChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( HighIntensityChanged( RgbaChannel, double ) ),
    // from:
    this,
    SLOT( OnHighIntensityChanged( RgbaChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ResetQuantileClicked( RgbaChannel ) ),
    // from:
    this,
    SLOT( OnResetQuantileClicked( RgbaChannel ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ResetIntensityClicked( RgbaChannel ) ),
    // from:
    this,
    SLOT( OnResetIntensityClicked( RgbaChannel  ) )
  );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ColorDynamicsController
::OnLowQuantileChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "%1; %2" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );
}

/*******************************************************************************/
void
ColorDynamicsController
::OnHighQuantileChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "%1; %2" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );
}

/*******************************************************************************/
void
ColorDynamicsController
::OnLowIntensityChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "%1; %2" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );
}

/*******************************************************************************/
void
ColorDynamicsController
::OnHighIntensityChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "%1; %2" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );
}

/*******************************************************************************/
void
ColorDynamicsController
::OnResetIntensityClicked( RgbaChannel channel )
{
  qDebug() << QString( "%1" ).arg( RGBA_CHANNEL_NAMES[ channel ] );
}

/*******************************************************************************/
void
ColorDynamicsController
::OnResetQuantileClicked( RgbaChannel channel )
{
  qDebug() << QString( "%1" ).arg( RGBA_CHANNEL_NAMES[ channel ] );
}

/*******************************************************************************/

} // end namespace 'mvd'
