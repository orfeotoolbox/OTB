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
  //
  // Access color-dynamics widget.
  ColorSetupWidget* colorSetupWidget = GetWidget< ColorSetupWidget >();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Setup color-setup controller.
  colorSetupWidget->SetComponents( imageModel->GetBandNames() );

  for( int i=0; i<RGBA_CHANNEL_ALPHA; ++i )
    {
    colorSetupWidget->SetCurrentIndex(
      static_cast< RgbaChannel >( i ),
      imageModel->GetSettings().RgbChannel( i )
    );
    }

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
