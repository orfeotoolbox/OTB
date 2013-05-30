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
#include "Core/mvdVectorImageModel.h"
#include "Gui/mvdColorDynamicsWidget.h"

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
    SIGNAL( LowQuantileChanged( RgbwChannel, double ) ),
    // to:
    this,
    SLOT( OnLowQuantileChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( HighQuantileChanged( RgbwChannel, double ) ),
    // to:
    this,
    SLOT( OnHighQuantileChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( LowIntensityChanged( RgbwChannel, double ) ),
    // to:
    this,
    SLOT( OnLowIntensityChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( HighIntensityChanged( RgbwChannel, double ) ),
    // to:
    this,
    SLOT( OnHighIntensityChanged( RgbwChannel, double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ResetQuantileClicked( RgbwChannel ) ),
    // to:
    this,
    SLOT( OnResetQuantileClicked( RgbwChannel ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ResetIntensityClicked( RgbwChannel ) ),
    // to:
    this,
    SLOT( OnResetIntensityClicked( RgbwChannel  ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( ApplyAllClicked( RgbwChannel, double, double ) ),
    // to:
    this,
    SLOT( OnApplyAllClicked( RgbwChannel, double, double  ) )
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
  // Reset color-dynamics widget.
  ResetIntensityRanges( RGBW_CHANNEL_RGB );
#if 1
  SetIntensities( RGBW_CHANNEL_RGB );
#else
  ResetQuantiles( RGBW_CHANNEL_RGB );
#endif
}

/*******************************************************************************/
void
ColorDynamicsController
::Disconnect( AbstractModel* model )
{
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  //
  // Disconnect controller to model.
  QObject::disconnect(
    this, SIGNAL( ModelUpdated() ),
    // to:
    model, SLOT( OnModelUpdated() )
  );

  //
  // Disconnect GUI from controller.
  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LowQuantileChanged( RgbwChannel, double ) ),
    // from:
    this,
    SLOT( OnLowQuantileChanged( RgbwChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( HighQuantileChanged( RgbwChannel, double ) ),
    // from:
    this,
    SLOT( OnHighQuantileChanged( RgbwChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LowIntensityChanged( RgbwChannel, double ) ),
    // from:
    this,
    SLOT( OnLowIntensityChanged( RgbwChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( HighIntensityChanged( RgbwChannel, double ) ),
    // from:
    this,
    SLOT( OnHighIntensityChanged( RgbwChannel, double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ResetQuantileClicked( RgbwChannel ) ),
    // from:
    this,
    SLOT( OnResetQuantileClicked( RgbwChannel ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ResetIntensityClicked( RgbwChannel ) ),
    // from:
    this,
    SLOT( OnResetIntensityClicked( RgbwChannel  ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( ApplyAllClicked( RgbwChannel, double, double ) ),
    // from:
    this,
    SLOT( OnApplyAllClicked( RgbwChannel, double, double  ) )
  );

}

/*******************************************************************************/
void
ColorDynamicsController
::ResetIntensityRanges( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !mvd::RgbBounds( begin, end, channels ) )
    return;

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

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    DefaultImageType::PixelType::ValueType min(
      minPx[ settings.GetChannel( channel ) ]
    );

    DefaultImageType::PixelType::ValueType max(
      maxPx[ settings.GetChannel( channel ) ]
    );

    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    colorBandDynWgt->blockSignals( true );
    {
    // Set min/max intensity bounds.
    colorBandDynWgt->SetMinIntensity( min );
    colorBandDynWgt->SetMaxIntensity( max );
    }
    colorBandDynWgt->blockSignals( false );
    }
  }
  this->blockSignals( true );
}

/*******************************************************************************/
void
ColorDynamicsController
::ResetIntensities( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !mvd::RgbBounds( begin, end, channels ) )
    return;

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


  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    DefaultImageType::PixelType::ValueType min(
      minPx[ settings.GetChannel( channel ) ]
    );

    DefaultImageType::PixelType::ValueType max(
      maxPx[ settings.GetChannel( channel ) ]
    );
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
    colorBandDynWgt->blockSignals( false );
    }
  }
  this->blockSignals( false );
}

/*******************************************************************************/
void
ColorDynamicsController
::SetIntensities( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !mvd::RgbBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  /*
  //
  // Access histogram from generic image-model.
  const HistogramModel* histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  // Get min/max pixels.
  DefaultImageType::PixelType minPx( histogramModel->GetMinPixel() );
  DefaultImageType::PixelType maxPx( histogramModel->GetMaxPixel() );
  */

  // Get image rengering settings.
  const VectorImageModel::Settings& settings = imageModel->GetSettings();


  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    ParametersType::ValueType low = settings.GetDynamicsParam( 2 * channel );
    ParametersType::ValueType hi = settings.GetDynamicsParam( 2 * channel + 1 );

    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    colorBandDynWgt->blockSignals( true );
    {
    // Set low & high intensities.
    colorBandDynWgt->SetLowIntensity( low );
    OnLowIntensityChanged( channel, low );

    colorBandDynWgt->SetHighIntensity( hi );
    OnHighIntensityChanged( channel, hi );
    }
    colorBandDynWgt->blockSignals( false );
    }
  }
  this->blockSignals( false );
}

/*******************************************************************************/
void
ColorDynamicsController
::ResetQuantiles( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !mvd::RgbBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();


  // Block this controller's signals to prevent display refreshed
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

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
  }
  this->blockSignals( false );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
ColorDynamicsController
::OnRgbChannelIndexChanged( RgbwChannel channel, int band )
{
  qDebug()
    << this
    << "::OnRgbChannelIndexChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << band <<
    ")";

  //
  // Reset color-dynamics widget.
  ResetIntensityRanges( channel );
  ResetQuantiles( channel );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnGrayChannelIndexChanged( int band )
{
  qDebug()
    << this
    << "::OnGrayChannelIndexChanged(" << band << ")";

  //
  // Reset color-dynamics widget.
  ResetIntensityRanges( RGBW_CHANNEL_RGB );
  ResetQuantiles( RGBW_CHANNEL_RGB );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnGrayscaleActivated( bool activated )
{
  qDebug()
    << this
    << "::OnGrayscaleActivated(" << activated << ")";

  //
  // Toggle grayscale-mode activation-state of controlled widget.
  GetWidget< ColorDynamicsWidget >()->SetGrayscaleActivated( activated );

  //
  // Reset color-dynamics widget.
  ResetIntensityRanges( RGBW_CHANNEL_RGB );
  ResetQuantiles( RGBW_CHANNEL_RGB );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnLowQuantileChanged( RgbwChannel channel, double value )
{
  qDebug()
    << this
    << "::OnLowQuantileChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
    << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Reference settings.
  VectorImageModel::Settings& settings = imageModel->GetSettings();

  // Calculate quantile intensity.
  HistogramModel::MeasurementType intensity =
    imageModel->GetHistogramModel()->Quantile(
      settings.GetChannel( channel ),
      0.01 * value,
      BOUND_LOWER
    );

  // Update quantile intensity in model.
  settings.SetDynamicsParam( 2 * channel, intensity );

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
::OnHighQuantileChanged( RgbwChannel channel, double value )
{
  qDebug()
    << this
    << "::OnHighQuantileChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << value <<
    ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Reference settings.
  VectorImageModel::Settings& settings = imageModel->GetSettings();

  // Calculate quantile intensity.
  HistogramModel::MeasurementType intensity =
    imageModel->GetHistogramModel()->Quantile(
      settings.GetChannel( channel ),
      0.01 * value,
      BOUND_UPPER
    );

  // Update quantile intensity in model.
  settings.SetDynamicsParam( 2 * channel + 1, intensity );

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
::OnLowIntensityChanged( RgbwChannel channel, double value )
{
  qDebug()
    << this
    << "::OnLowIntensityChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
    << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Update parameter value.
  imageModel->GetSettings().SetDynamicsParam( 2 * channel, value );

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
      imageModel->GetSettings().GetChannel( channel ),
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
::OnHighIntensityChanged( RgbwChannel channel, double value )
{
  qDebug()
    << this
    << "::OnHighIntensityChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
    << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Update parameter value in model.
  imageModel->GetSettings().SetDynamicsParam( 2 * channel + 1, value );

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
      imageModel->GetSettings().GetChannel( channel ),
      value,
      BOUND_UPPER )
  );
  }
  colorBandDynWgt->blockSignals( false );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnResetIntensityClicked( RgbwChannel channel )
{
  qDebug()
    << this
    << "::OnResetIntensityClicked(" << RGBW_CHANNEL_NAMES[ channel ] << ")";

  ResetIntensities(
    channel==RGBW_CHANNEL_WHITE
    ? RGBW_CHANNEL_RGB
    : channel
  );

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnResetQuantileClicked( RgbwChannel channel )
{
  qDebug()
    << this
    << "::OnResetQuantileChanged(" << RGBW_CHANNEL_NAMES[ channel ] << ")";

  ResetQuantiles(
    channel==RGBW_CHANNEL_WHITE
    ? RGBW_CHANNEL_RGB
    : channel
  );

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnApplyAllClicked( RgbwChannel channel, double low, double high )
{
  qDebug()
    << this
    << "::OnApplyAllChanged(" << RGBW_CHANNEL_NAMES[ channel ] << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );
  assert( imageModel->GetHistogramModel()!=NULL );

  // Reference settings.
  VectorImageModel::Settings& settings = imageModel->GetSettings();

  CountType begin;
  CountType end;

  RgbBounds( begin, end, RGBW_CHANNEL_RGB );

  for( CountType i=begin; i<end; ++i )
    {
    HistogramModel::MeasurementType lintensity =
      imageModel->GetHistogramModel()->Quantile(
        settings.GetChannel( i ),
        0.01 * low,
        BOUND_LOWER
    );

    // Calculate quantile intensity.
    HistogramModel::MeasurementType uintensity =
      imageModel->GetHistogramModel()->Quantile(
        settings.GetChannel( i ),
        0.01 * high,
        BOUND_UPPER
    );

    // Update quantile intensity in model.
    settings.SetDynamicsParam( 2 * i, lintensity );
    settings.SetDynamicsParam( 2 * i + 1, uintensity );

    // Get color-dynamics widgets.
    ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
    assert( colorDynWgt!=NULL );

    ColorBandDynamicsWidget* colorBandDynWgt = colorDynWgt->GetChannel(  static_cast< RgbwChannel >(i) );
    assert( colorBandDynWgt!=NULL );

    // Block widget signals to prevent recursive signal/slot loops.
    colorBandDynWgt->blockSignals( true );

    // Refresh low-intensity display.
    colorBandDynWgt->SetHighIntensity( uintensity );
    colorBandDynWgt->SetLowIntensity( lintensity );
    colorBandDynWgt->SetLowQuantile( low );
    colorBandDynWgt->SetHighQuantile( high );

    colorBandDynWgt->blockSignals( false );
    }

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}



/*******************************************************************************/

} // end namespace 'mvd'
