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
#include "mvdColorSetupController.h"


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
#include "mvdColorSetupWidget.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ColorSetupController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ColorSetupController
::ColorSetupController( ColorSetupWidget* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
ColorSetupController
::~ColorSetupController()
{
}

/*******************************************************************************/
void
ColorSetupController
::Connect( AbstractModel* model )
{
  ColorSetupWidget* colorSetupWidget = GetWidget< ColorSetupWidget >();

  //
  // Connect GUI to controller.
  QObject::connect(
    colorSetupWidget,
    SIGNAL( CurrentIndexChanged( RgbaChannel, int ) ),
    // to:
    this,
    SLOT( OnCurrentIndexChanged( RgbaChannel, int ) )
  );

  //
  // Connect controller to model.
  QObject::connect(
    this, SIGNAL( ModelUpdated() ),
    // to:
    model, SLOT( OnModelUpdated() )
  );
}

/*******************************************************************************/
void
ColorSetupController
::ResetWidget()
{
  // Reset color-setup widget.
  ResetIndices( RGBA_CHANNEL_RGB );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorSetupController
::Disconnect( AbstractModel* model )
{
  ColorSetupWidget* colorSetupWidget = GetWidget< ColorSetupWidget >();

  //
  // Disconnect controller to model.
  QObject::disconnect(
    this, SIGNAL( ModelUpdated() ),
    // from:
    model, SLOT( OnModelUpdated() )
  );

  //
  // Disconnect GUI from controller.
  QObject::disconnect(
    colorSetupWidget,
    SIGNAL( CurrentIndexChanged( RgbaChannel, int ) ),
    // to:
    this,
    SLOT( OnCurrentIndexChanged( RgbaChannel, int ) )
  );
}

/*******************************************************************************/
void
ColorSetupController
::ResetIndices( RgbaChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = 0;
  CountType end = 0;

  if( !mvd::RgbBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorSetupWidget* colorSetupWidget = GetWidget< ColorSetupWidget >();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
  colorSetupWidget->blockSignals( true );
  {
  // Reset list of component names.
  colorSetupWidget->SetComponents( imageModel->GetBandNames() );

  // Reset current-indices of RGB channels widgets.
  for( CountType i=begin; i<end; ++i )
    {
    RgbaChannel channel = static_cast< RgbaChannel >( i );
    
    VectorImageModel::Settings::ChannelVector::value_type band =
      imageModel->GetSettings().RgbChannel( i );

    // Set current-index of channel.
    colorSetupWidget->SetCurrentIndex( channel, band );
    OnCurrentIndexChanged( channel, band );
    }
  }
  colorSetupWidget->blockSignals( false );
  }
  this->blockSignals( false );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ColorSetupController
::OnCurrentIndexChanged( RgbaChannel channel, int index )
{
  qDebug() << QString( "OnCurrentIndexChanged(%1, %2)" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( index );

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Update channel index.
  imageModel->GetSettings().RgbChannel( channel ) = index;

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/

} // end namespace 'mvd'
