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

  //
  // Connect GUI to controller.
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
ColorDynamicsController
::ResetWidget()
{
  //
  // Setup color-dynamics controller.
  ResetIntensityRanges();
  ResetQuantiles();

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::Disconnect( AbstractModel* model )
{
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  //
  // Disconnect controller to model.
  QObject::connect(
    this, SIGNAL( ModelUpdated() ),
    // to:
    model, SLOT( OnModelUpdated() ) );

  //
  // Disconnect GUI from controller.
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
void
ColorDynamicsController
::ResetIntensityRanges()
{
  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  //
  // Access histogram from generic image-model.
  const HistogramModel* histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  // Get min/max pixels.
  DefaultImageType::PixelType minPx( histogramModel->GetMinPixel() );
  DefaultImageType::PixelType maxPx( histogramModel->GetMaxPixel() );

  // Get image rengering settings.
  const VectorImageModel::Settings& settings = imageModel->GetSettings();

  // Assign values to controlled widget.
  for( CountType i=0; i<RGBA_CHANNEL_ALPHA; ++i )
    {
    RgbaChannel channel = static_cast< RgbaChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    DefaultImageType::PixelType::ValueType min(
      minPx[ settings.RgbChannel( channel ) ]
    );

    DefaultImageType::PixelType::ValueType max(
      maxPx[ settings.RgbChannel( channel ) ]
    );

    colorBandDynWgt->SetMinIntensity( min );
    colorBandDynWgt->SetMaxIntensity( max );
    }
}

/*******************************************************************************/
void
ColorDynamicsController
::ResetQuantiles()
{
  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  // Assign values to controlled widget.
  for( CountType i=0; i<RGBA_CHANNEL_ALPHA; ++i )
    {
    RgbaChannel channel = static_cast< RgbaChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    // Block this controller's signals to prevent display refreshed
    // but let let widget(s) signal their changes so linked values
    // will be correctly updated.
    this->blockSignals( true );
    {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    colorBandDynWgt->blockSignals( true );
    {
    colorBandDynWgt->SetLowQuantile( 2.0 );
    OnLowQuantileChanged( channel, 2.0 );

    colorBandDynWgt->SetHighQuantile( 2.0 );
    OnHighQuantileChanged( channel, 2.0 );
    }
    colorBandDynWgt->blockSignals( false );
    }
    this->blockSignals( false );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ColorDynamicsController
::OnLowQuantileChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "OnLowQuantileChanged(%1, %2)" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Reference settings.
  VectorImageModel::Settings& settings = imageModel->GetSettings();

  // Calculate quantile intensity.
  HistogramModel::MeasurementType intensity =
    imageModel->GetHistogramModel()->Quantile(
      settings.RgbChannel( channel ),
      0.01 * value,
      BOUND_LOWER
    );

  // Update quantile intensity in model.
  settings.DynamicsParam( 2 * channel ) = intensity;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  ColorBandDynamicsWidget* colorBandDynWgt = colorDynWgt->GetChannel( channel );
  assert( colorBandDynWgt!=NULL );

  // Block widget signals to prevent recursive signal/slot loops.
  colorBandDynWgt->blockSignals( true );
  {
  // Refresh low-intensity display.
  colorBandDynWgt->SetLowIntensity( intensity );
  }
  colorBandDynWgt->blockSignals( false );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnHighQuantileChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "OnHighQuantileChanged(%1, %2)" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Reference settings.
  VectorImageModel::Settings& settings = imageModel->GetSettings();

  // Calculate quantile intensity.
  HistogramModel::MeasurementType intensity =
    imageModel->GetHistogramModel()->Quantile(
      settings.RgbChannel( channel ),
      0.01 * value,
      BOUND_UPPER
    );

  // Update quantile intensity in model.
  settings.DynamicsParam( 2 * channel + 1 ) = intensity;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  ColorBandDynamicsWidget* colorBandDynWgt = colorDynWgt->GetChannel( channel );
  assert( colorBandDynWgt!=NULL );

  // Block widget signals to prevent recursive signal/slot loops.
  colorBandDynWgt->blockSignals( true );
  {
  // Refresh low-intensity display.
  colorBandDynWgt->SetHighIntensity( intensity );
  }
  colorBandDynWgt->blockSignals( false );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnLowIntensityChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "OnLowIntensityChanged(%1, %2)" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Update parameter value.
  imageModel->GetSettings().DynamicsParam( 2 * channel ) = value;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt );

  ColorBandDynamicsWidget* colorBandDynWgt = colorDynWgt->GetChannel( channel );

  // Block widget signals to prevent recursive signal/slot loops.
  colorBandDynWgt->blockSignals( true );
  {
  // Refresh quantile display.
  colorBandDynWgt->SetLowQuantile(
    100.0 * imageModel->GetHistogramModel()->Percentile(
      imageModel->GetSettings().RgbChannel( channel ),
      value,
      BOUND_LOWER )
  );
  }
  colorBandDynWgt->blockSignals( false );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnHighIntensityChanged( RgbaChannel channel, double value )
{
  qDebug() << QString( "OnHighIntensityChanged(%1, %2)" ).arg( RGBA_CHANNEL_NAMES[ channel ] ).arg( value );

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Update parameter value in model.
  imageModel->GetSettings().DynamicsParam( 2 * channel + 1 ) = value;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt );

  ColorBandDynamicsWidget* colorBandDynWgt = colorDynWgt->GetChannel( channel );

  // Block widget signals to prevent recursive signal/slot loops.
  colorBandDynWgt->blockSignals( true );
  {
  // Refresh quantile display.
  colorBandDynWgt->SetHighQuantile(
    100.0 * imageModel->GetHistogramModel()->Percentile(
      imageModel->GetSettings().RgbChannel( channel ),
      value,
      BOUND_LOWER )
  );
  }
  colorBandDynWgt->blockSignals( false );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnResetIntensityClicked( RgbaChannel channel )
{
  qDebug() << QString( "OnResetIntensityClicked(%1)" ).arg( RGBA_CHANNEL_NAMES[ channel ] );
  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  //
  // Access histogram from generic image-model.
  const HistogramModel* histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  // Get min/max pixels.
  DefaultImageType::PixelType minPx( histogramModel->GetMinPixel() );
  DefaultImageType::PixelType maxPx( histogramModel->GetMaxPixel() );

  // Get image rengering settings.
  const VectorImageModel::Settings& settings = imageModel->GetSettings();

  // Assign values to controlled widget.
  for( CountType i=0; i<RGBA_CHANNEL_ALPHA; ++i )
    {
    RgbaChannel channel = static_cast< RgbaChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    DefaultImageType::PixelType::ValueType min(
      minPx[ settings.RgbChannel( channel ) ]
    );

    DefaultImageType::PixelType::ValueType max(
      maxPx[ settings.RgbChannel( channel ) ]
    );

    // Block this controller's signals to prevent display refreshes
    // but let let widget(s) signal their changes so linked values
    // will be correctly updated.
    this->blockSignals( true );
    {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    colorBandDynWgt->blockSignals( true );
    {
    // Set low & high intensities.
    colorBandDynWgt->SetLowIntensity( min );
    OnLowIntensityChanged( channel, min );

    colorBandDynWgt->SetHighIntensity( max );
    OnHighIntensityChanged( channel, max );
    }
    }
    this->blockSignals( false );
    }

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnResetQuantileClicked( RgbaChannel channel )
{
  qDebug() << QString( "OnResetQuantileChanged(%1)" ).arg( RGBA_CHANNEL_NAMES[ channel ] );

  ResetQuantiles();

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*******************************************************************************/

} // end namespace 'mvd'
