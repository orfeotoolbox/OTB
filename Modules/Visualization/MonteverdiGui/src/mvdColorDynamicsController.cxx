/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
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
#include "mvdVectorImageModel.h"
#include "mvdColorDynamicsWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ColorDynamicsController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
ColorDynamicsController
::ColorDynamicsController( ColorDynamicsWidget* widget, QObject* p ) :
  AbstractModelController( widget, p )
{
}

/*****************************************************************************/
ColorDynamicsController
::~ColorDynamicsController()
{
}

/*****************************************************************************/
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

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( LinkToggled( RgbwChannel, bool ) ),
    // to:
    this,
    SLOT( OnLinkToggled( RgbwChannel, bool  ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( NoDataFlagToggled( bool ) ),
    // to:
    this,
    SLOT( OnNoDataFlagToggled( bool ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( NoDataValueChanged( double ) ),
    // to:
    this,
    SLOT( OnNoDataValueChanged( double ) )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL( NoDataButtonPressed() ),
    // to:
    this,
    SLOT( RefreshHistogram() )
  );

  QObject::connect(
    colorDynamicsWidget,
    SIGNAL(GammaValueChanged( double ) ),
    // to:
    this,
    SLOT( OnGammaValueChanged( double ))
    );

  //
  // Connect controller to model.
  QObject::connect(
    this, SIGNAL( ModelUpdated() ),
    // to:
    model, SLOT( OnModelUpdated() )
  );
}

/*****************************************************************************/
void
ColorDynamicsController
::virtual_ResetWidget( bool isSettingModel )
{
  //
  // Reset color-dynamics widget mode (grayscale/RGB).
  assert( GetModel()!=NULL && GetModel()==GetModel< VectorImageModel >() );
  const VectorImageModel* model = GetModel< VectorImageModel >();

  assert( GetWidget()!=NULL &&
	  GetWidget()==GetWidget< ColorDynamicsWidget >() );
  ColorDynamicsWidget* widget = GetWidget< ColorDynamicsWidget >();

  widget->SetGrayscaleActivated( model->GetSettings().IsGrayscaleActivated() );

  RgbwChannel channels = RGBW_CHANNEL_ALL;
    /*
    model->GetSettings().IsGrayscaleActivated()
    ? RGBW_CHANNEL_WHITE
    : RGBW_CHANNEL_RGB;
    */

  //
  // Reset color-dynamics widget.

  // Set constrained editing ON/OFF.
  // (must be called before settings intensity/quantiles)
  SetBoundsEnabled( channels, isSettingModel );

  // Set ranges.
  ResetIntensityRanges( channels );

  // Set intensities based on applied quantiles (restoring
  // dataset-descriptor settings).
  SetIntensities( channels );

  // Setup no-data state.
  SetNoData();
  SetGamma();
}

/*****************************************************************************/
void
ColorDynamicsController
::ClearWidget()
{
  // qDebug() << this << "::ClearWidget()";

  assert( GetWidget()!=NULL &&
	  GetWidget()==GetWidget< ColorDynamicsWidget >() );
  ColorDynamicsWidget* widget = GetWidget< ColorDynamicsWidget >();

  widget->SetGrayscaleActivated( true );

  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, RGBW_CHANNEL_ALL ) )
    return;

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals( true );
  {
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      widget->GetChannel( channel );


    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    colorBandDynWgt->SetBounded( true );
    colorBandDynWgt->SetLinkButtonEnabled( true );

    colorBandDynWgt->SetMinIntensity( 0.0 );
    colorBandDynWgt->SetMaxIntensity( 1.0 );

    colorBandDynWgt->SetLowIntensity( 0.0 );
    colorBandDynWgt->SetHighIntensity( 1.0 );

    colorBandDynWgt->SetLowQuantile( 2.0 );
    colorBandDynWgt->SetHighQuantile( 2.0 );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
  }
  this->blockSignals( thisSignalsBlocked );
}

/*****************************************************************************/
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

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( LinkToggled( RgbwChannel, bool ) ),
    // to:
    this,
    SLOT( OnLinkToggled( RgbwChannel, bool  ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( NoDataFlagToggled( bool ) ),
    // to:
    this,
    SLOT( OnNoDataFlagToggled( bool ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( NoDataValueChanged( double ) ),
    // to:
    this,
    SLOT( OnNoDataValueChanged( double ) )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( NoDataButtonPressed() ),
    // to:
    this,
    SLOT( RefreshHistogram() )
  );

  QObject::disconnect(
    colorDynamicsWidget,
    SIGNAL( GammaValueChanged( double ) ),
    // to:
    this,
    SLOT( OnGammaValueChanged( double ) )
    );
}

/*****************************************************************************/
void
ColorDynamicsController
::ResetIntensityRanges( RgbwChannel channels )
{
  // Trace.
  // qDebug()
  //   << this
  //   << "::ResetIntensityRanges(" << RGBW_CHANNEL_NAMES[ channels ] << ")";

  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channels ) )
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
  const HistogramModel * histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  // if( !histogramModel->IsValid() )
  //   return;

  // Get min/max pixels.
  DefaultImageType::PixelType minPx( histogramModel->GetMinPixel() );
  DefaultImageType::PixelType maxPx( histogramModel->GetMaxPixel() );

  // Get image rengering settings.
  const VectorImageSettings & settings = imageModel->GetSettings();

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals( true );
  {
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    assert(
      std::numeric_limits< DefaultImageType::PixelType::ValueType >::has_infinity
    );

    DefaultImageType::PixelType::ValueType min(
      -std::numeric_limits< DefaultImageType::PixelType::ValueType >::infinity()
    );
    DefaultImageType::PixelType::ValueType max(
      +std::numeric_limits< DefaultImageType::PixelType::ValueType >::infinity()
    );

    if( colorBandDynWgt->IsBounded() )
      {
      min = minPx[ settings.GetRgbwChannel( channel ) ];
      max = maxPx[ settings.GetRgbwChannel( channel ) ];
      }

    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // Set min/max intensity bounds.
    colorBandDynWgt->SetMinIntensity( min );
    colorBandDynWgt->SetMaxIntensity( max );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
  }
  this->blockSignals( thisSignalsBlocked );
}

/*****************************************************************************/
void
ColorDynamicsController
::ResetIntensities( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channels ) )
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
  const VectorImageSettings & settings = imageModel->GetSettings();

  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    DefaultImageType::PixelType::ValueType min(
      minPx[ settings.GetRgbwChannel( channel ) ]
    );

    DefaultImageType::PixelType::ValueType max(
      maxPx[ settings.GetRgbwChannel( channel ) ]
    );

    // Block this controller's signals to prevent display refreshes
    // but let let widget(s) signal their changes so linked values
    // will be correctly updated.
    bool thisSignalsBlocks = this->blockSignals( true );
    {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // Set low & high intensities.
    colorBandDynWgt->SetLowIntensity( min );
    OnLowIntensityChanged( channel, min );

    colorBandDynWgt->SetHighIntensity( max );
    OnHighIntensityChanged( channel, max );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
    this->blockSignals( thisSignalsBlocks );

    emit LowIntensityChanged( channel, min, false );
    emit HighIntensityChanged( channel, max, false );
    }
}

/*****************************************************************************/
void
ColorDynamicsController
::SetIntensities( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();

  //
  // Access image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Get image rengering settings.
  const VectorImageSettings & settings = imageModel->GetSettings();


  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    ParametersType::ValueType low = settings.GetLowIntensity( channel );
    ParametersType::ValueType hi = settings.GetHighIntensity( channel );

    // Block this controller's signals to prevent display refreshes
    // but let let widget(s) signal their changes so linked values
    // will be correctly updated.
    bool thisSignalsBlocked = this->blockSignals( true );
    {
    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // Set low & high intensities.
    colorBandDynWgt->SetLowIntensity( low );
    OnLowIntensityChanged( channel, low );

    colorBandDynWgt->SetHighIntensity( hi );
    OnHighIntensityChanged( channel, hi );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
    this->blockSignals( thisSignalsBlocked );

    emit LowIntensityChanged( channel, low, true );
    emit HighIntensityChanged( channel, hi, true );
    }
}

/*****************************************************************************/
void
ColorDynamicsController
::SetNoData()
{
  assert( GetModel()==GetModel< VectorImageModel >() );
  const VectorImageModel* imageModel = GetModel< VectorImageModel >();

  if( imageModel==NULL )
    return;

  const ImageProperties* imageProperties = imageModel->GetProperties();
  assert( imageProperties!=NULL );

  assert( GetWidget()==GetWidget< ColorDynamicsWidget >() );
  ColorDynamicsWidget* widget = GetWidget< ColorDynamicsWidget >();
  assert( widget!=NULL );

  bool thisSignalsBlocked = this->blockSignals( true );
  {
  bool widgetSignalsBlocked = widget->blockSignals( true );
  {
  widget->SetNoDataButtonChecked( true );
  widget->SetNoDataChecked( imageProperties->IsNoDataEnabled() );
  widget->SetNoDataValue( imageProperties->GetNoData() );
  }
  widget->blockSignals( widgetSignalsBlocked );
  }
  this->blockSignals( thisSignalsBlocked );
}

/*****************************************************************************/
void
ColorDynamicsController
::SetGamma()
{
  assert( GetModel()==GetModel< VectorImageModel >() );
  const VectorImageModel* imageModel = GetModel< VectorImageModel >();

  if( imageModel==NULL )
    return;

  assert( GetWidget()==GetWidget< ColorDynamicsWidget >() );
  ColorDynamicsWidget* widget = GetWidget< ColorDynamicsWidget >();
  assert( widget!=NULL );

  bool thisSignalsBlocked = this->blockSignals( true );
  {
  bool widgetSignalsBlocked = widget->blockSignals( true );
  {
  widget->SetGamma( imageModel->GetSettings().GetGamma() );
  }
  widget->blockSignals( widgetSignalsBlocked );
  }
  this->blockSignals( thisSignalsBlocked );  
}

/*****************************************************************************/
void
ColorDynamicsController
::SetBoundsEnabled( RgbwChannel channels, bool isSettingModel )
{
  // qDebug()
  //   << this
  //   << "::SetBoundsEnabled(" << channels << "," << isSettingModel << ")";

  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorDynamicsWidget * colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();
  assert( colorDynamicsWidget!=NULL );

  //
  // Access image-model.
  const VectorImageModel * imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  //
  // Access histogram-model.
  assert( imageModel->GetHistogramModel()!=NULL );
  bool hasValidHistogram = imageModel->GetHistogramModel()->IsValid();

  // Block this controller's signals to prevent display refreshed
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals( true );
  {
  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget * colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // MANTIS-1144:
    //
    // SetBoundsEnabled() must keep track of previous state because it
    // is also called (circular loop) when editing quantile and/or
    // intensity is finished.
    //
    // {
    colorBandDynWgt->SetBounded(
      ( !isSettingModel
	? colorBandDynWgt->IsBounded()
	: true
      ) &&
      hasValidHistogram
    );
    // }
    colorBandDynWgt->SetLinkButtonEnabled( hasValidHistogram );
    colorBandDynWgt->SetDefaultsButtonEnabled( hasValidHistogram );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
  }
  this->blockSignals( thisSignalsBlocked );
}

/*****************************************************************************/
void
ColorDynamicsController
::ResetQuantiles( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();
  assert( colorDynamicsWidget!=NULL );

  // Block this controller's signals to prevent display refreshed
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals( true );
  {
  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget * colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    colorBandDynWgt->SetLowQuantile( 2.0 );
    OnLowQuantileChanged( channel, 2.0 );

    colorBandDynWgt->SetHighQuantile( 2.0 );
    OnHighQuantileChanged( channel, 2.0 );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
  }
  this->blockSignals( thisSignalsBlocked );
}

/*****************************************************************************/
void
ColorDynamicsController
::RestoreQuantiles( RgbwChannel channels )
{
  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channels ) )
    return;

  //
  // Access color-dynamics widget.
  ColorDynamicsWidget* colorDynamicsWidget = GetWidget< ColorDynamicsWidget >();


  // Block this controller's signals to prevent display refreshed
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  bool thisSignalsBlocked = this->blockSignals( true );
  {
  // Assign values to controlled widget.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel channel = static_cast< RgbwChannel >( i );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynamicsWidget->GetChannel( channel );

    // Block widget's signals...
    //...but force call to valueChanged() slot to force refresh.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    OnLowQuantileChanged( channel, colorBandDynWgt->GetLowQuantile() );
    OnHighQuantileChanged( channel, colorBandDynWgt->GetHighQuantile() );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );
    }
  }
  this->blockSignals( thisSignalsBlocked );
}

/*****************************************************************************/
void
ColorDynamicsController
::RefreshHistogram()
{  
  // Trace.
  // qDebug()
  //   << this
  //   << "::RefreshHistogram()";

  // Get image-model.
  assert( GetModel()==GetModel< VectorImageModel>() );
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Refresh histogram-model.
  imageModel->RefreshHistogram();

  // Refresh color-dynamics.
  {
    assert( imageModel->GetHistogramModel()!=NULL );

    SetBoundsEnabled( RGBW_CHANNEL_ALL, true );

    if( imageModel->GetHistogramModel()->IsValid() )
      ResetQuantiles( RGBW_CHANNEL_ALL );
    else
      ResetIntensities( RGBW_CHANNEL_ALL );
  }

  // Signal histogram has been refreshed.
  emit HistogramRefreshed();

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
ColorDynamicsController
::OnRgbChannelIndexChanged( RgbwChannel channel, int )
{  
  // qDebug()
  //   << this
  //   << "::OnRgbChannelIndexChanged("
  //   << RGBW_CHANNEL_NAMES[ channel ] << ", " << band <<
  //   ")";

  //
  // Reset color-dynamics widget.
  ResetQuantiles( channel );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnGrayChannelIndexChanged( int )
{
  // qDebug()
  //   << this
  //   << "::OnGrayChannelIndexChanged(" << band << ")";

  //
  // Reset color-dynamics widget.
  ResetQuantiles( RGBW_CHANNEL_WHITE );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
void
ColorDynamicsController
::OnGrayscaleActivated( bool activated )
{  
  // qDebug()
  //   << this
  //   << "::OnGrayscaleActivated(" << activated << ")";

  //
  // Toggle grayscale-mode activation-state of controlled widget.
  GetWidget< ColorDynamicsWidget >()->SetGrayscaleActivated( activated );

  //
  // Reset color-dynamics widget.
  ResetQuantiles(
    activated
    ? RGBW_CHANNEL_WHITE
    : RGBW_CHANNEL_RGB
  );

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnLowQuantileChanged( RgbwChannel channel, double value )
{  
  // qDebug()
  //   << this
  //   << "::OnLowQuantileChanged("
  //   << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
  //   << ")";

  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  const HistogramModel * histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  if( !histogramModel->IsValid() )
    return;

  // Reference settings.
  VectorImageSettings & settings = imageModel->GetSettings();

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  // Update quantile intensity in model.
  for( CountType i=begin; i<end; ++i )
    {
    // RGB/W Channel.
    RgbwChannel chan = static_cast< RgbwChannel >( i );

    // Calculate quantile intensity.
    HistogramModel::MeasurementType intensity =
      histogramModel->Quantile(
	settings.GetRgbwChannel( chan ),
	0.01 * value,
	BOUND_LOWER
      );

    // Set intensity.
    settings.SetLowIntensity( chan, intensity );

    // Access color-band widget.
    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynWgt->GetChannel( chan );

    // check pointer.
    assert( colorBandDynWgt!=NULL );

    // Block widget signals to prevent recursive signal/slot loops.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // Refresh low-intensity display.
    colorBandDynWgt->SetLowIntensity( intensity );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );

    emit LowIntensityChanged( chan, intensity, true );
    }

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnHighQuantileChanged( RgbwChannel channel, double value )
{  
  // qDebug()
  //   << this
  //   << "::OnHighQuantileChanged("
  //   << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
  //   << ")";

  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  const HistogramModel * histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  if( !histogramModel->IsValid() )
    return;

  // Reference settings.
  VectorImageSettings & settings = imageModel->GetSettings();

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  // Update quantile intensity in model.
  for( CountType i=begin; i<end; ++i )
    {
    // RGB/W Channel.
    RgbwChannel chan = static_cast< RgbwChannel >( i );

    // Calculate quantile intensity.
    HistogramModel::MeasurementType intensity =
      histogramModel->Quantile(
	settings.GetRgbwChannel( chan ),
	0.01 * value,
	BOUND_UPPER
      );

    // Set intensity.
    settings.SetHighIntensity( chan, intensity );

    // Access color-band widget.
    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynWgt->GetChannel( chan );

    // check pointer.
    assert( colorBandDynWgt!=NULL );

    // Block widget signals to prevent recursive signal/slot loops.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // Refresh high-intensity display.
    colorBandDynWgt->SetHighIntensity( intensity );
    }
    widgetSignalsBlocked = colorBandDynWgt->blockSignals( widgetSignalsBlocked );

    emit HighIntensityChanged( chan, intensity, true );
    }

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnLowIntensityChanged( RgbwChannel channel, double value )
{  
  // qDebug()
  //   << this
  //   << "::OnLowIntensityChanged("
  //   << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
  //   << ")";

  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  const HistogramModel * histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  // if( !histogramModel->IsValid() )
  //   return;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt );

  // Update parameter value.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel chan = static_cast< RgbwChannel >( i );

    imageModel->GetSettings().SetLowIntensity( chan, value );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynWgt->GetChannel( chan );

    if( colorBandDynWgt->IsBounded() && histogramModel->IsValid() )
      {
      // Block widget signals to prevent recursive signal/slot loops.
      bool wgtSignalsBlocked = colorBandDynWgt->blockSignals( true );
      {
      // Refresh quantile display.
      colorBandDynWgt->SetLowQuantile(
	100.0 * histogramModel->Percentile(
	  imageModel->GetSettings().GetRgbwChannel( chan ),
	  value,
	  BOUND_LOWER )
      );
      }
      wgtSignalsBlocked = colorBandDynWgt->blockSignals( wgtSignalsBlocked );
      }

    emit LowIntensityChanged( chan, value, true );
    }

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnHighIntensityChanged( RgbwChannel channel, double value )
{
  // qDebug()
  //   << this
  //   << "::OnHighIntensityChanged("
  //   << RGBW_CHANNEL_NAMES[ channel ] << ", " << value
  //   << ")";

  //
  // Calculate loop bounds. Return if nothing to do.
  CountType begin = -1;
  CountType end = -1;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  const HistogramModel * histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  // if( !histogramModel->IsValid() )
  //   return;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt );

  // Update parameter value.
  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel chan = static_cast< RgbwChannel >( i );

    imageModel->GetSettings().SetHighIntensity( chan, value );

    ColorBandDynamicsWidget* colorBandDynWgt =
      colorDynWgt->GetChannel( chan );

    if( colorBandDynWgt->IsBounded() && histogramModel->IsValid() )
      {
      // Block widget signals to prevent recursive signal/slot loops.
      bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
      {
      // Refresh quantile display.
      colorBandDynWgt->SetHighQuantile(
	100.0 * imageModel->GetHistogramModel()->Percentile(
	  imageModel->GetSettings().GetRgbwChannel( chan ),
	  value,
	  BOUND_UPPER
	)
      );
      }
      colorBandDynWgt->blockSignals( widgetSignalsBlocked );
      }

    emit HighIntensityChanged( chan, value, true );
    }

  // Signal model has been updated.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnResetIntensityClicked( RgbwChannel channel )
{  
  // qDebug()
  //   << this
  //   << "::OnResetIntensityClicked(" << RGBW_CHANNEL_NAMES[ channel ] << ")";

  ResetIntensities( channel );

  // Emit signal to force histogram refresh.
  emit HistogramRefreshed();

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnResetQuantileClicked( RgbwChannel channel )
{  
  // qDebug()
  //   << this
  //   << "::OnResetQuantileChanged(" << RGBW_CHANNEL_NAMES[ channel ] << ")";

  ResetQuantiles( channel );

  // Emit signal to force histogram refresh.
  emit HistogramRefreshed();

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnApplyAllClicked( RgbwChannel, double low, double high )
{  
  // qDebug()
  //   << this
  //   << "::OnApplyAllChanged("
  //   << RGBW_CHANNEL_NAMES[ channel ] << "," << low << "," << high << ")";

  // Get image-model.
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  const HistogramModel * histogramModel = imageModel->GetHistogramModel();
  assert( histogramModel!=NULL );

  if( !histogramModel->IsValid() )
    return;

  // Reference settings.
  VectorImageSettings & settings = imageModel->GetSettings();

  CountType begin;
  CountType end;

  if( !RgbBounds(
	begin, end,
	settings.IsGrayscaleActivated()
	? RGBW_CHANNEL_WHITE
	: RGBW_CHANNEL_RGB ) )
    return;

  // Get color-dynamics widgets.
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  for( CountType i=begin; i<end; ++i )
    {
    RgbwChannel chan = static_cast< RgbwChannel >( i );

    VectorImageSettings::ChannelVector::value_type band(
      settings.GetRgbwChannel( chan )
    );

    HistogramModel::MeasurementType lintensity =
      histogramModel->Quantile(
        band,
        0.01 * low,
        BOUND_LOWER
    );

    // Calculate quantile intensity.
    HistogramModel::MeasurementType uintensity =
      histogramModel->Quantile(
        band,
        0.01 * high,
        BOUND_UPPER
    );

    // Update quantile intensity in model.
    settings.SetLowIntensity( chan, lintensity );
    settings.SetHighIntensity( chan, uintensity );

    ColorBandDynamicsWidget* colorBandDynWgt = colorDynWgt->GetChannel( chan );
    assert( colorBandDynWgt!=NULL );

    // Block widget signals to prevent recursive signal/slot loops.
    bool widgetSignalsBlocked = colorBandDynWgt->blockSignals( true );
    {
    // Refresh intensities display.
    // TODO: Remove SetLow/HighIntensity(): overwrite by SetLow/HighQuantiles().
    colorBandDynWgt->SetHighIntensity( uintensity );
    colorBandDynWgt->SetLowIntensity( lintensity );

    // Regresh quantile display.
    colorBandDynWgt->SetLowQuantile( low );
    colorBandDynWgt->SetHighQuantile( high );
    }
    colorBandDynWgt->blockSignals( widgetSignalsBlocked );

    emit LowIntensityChanged( chan, lintensity, false );
    emit HighIntensityChanged( chan, uintensity, false );
    }

  // Force histogram refresh.
  emit HistogramRefreshed();

  // Now, emit this controller's signal to cause display refresh.
  emit ModelUpdated();
}

/*****************************************************************************/
void
ColorDynamicsController
::OnNoDataFlagToggled( bool enabled )
{  
  // Trace.
  // qDebug()
  //   << this
  //   << "::OnNoDataFlagToggled(" << enabled << ")";

  // Get image-model.
  assert( GetModel()==GetModel< VectorImageModel>() );
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Reference settings.
  ImageProperties* properties = imageModel->GetProperties();

  // Get color-dynamics widgets.
  assert( GetWidget()==GetWidget< ColorDynamicsWidget >() );
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  // Store property.
  // properties->SetNoDataEnabled( colorDynWgt->IsNoDataChecked() );
  properties->SetNoDataEnabled( enabled );

  //
  colorDynWgt->SetNoDataButtonChecked( false );
}

/*****************************************************************************/
void
ColorDynamicsController
::OnNoDataValueChanged( double value )
{  
  // Trace.
  // qDebug()
  //   << this
  //   << "::OnNoDataValueChanged(" << value << ")";

  // Get image-model.
  assert( GetModel()==GetModel< VectorImageModel>() );
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Reference settings.
  ImageProperties* properties = imageModel->GetProperties();

  // Get color-dynamics widgets.
  assert( GetWidget()==GetWidget< ColorDynamicsWidget >() );
  ColorDynamicsWidget* colorDynWgt = GetWidget< ColorDynamicsWidget >();
  assert( colorDynWgt!=NULL );

  // Store property.
  // properties->SetNoData( colorDynWgt->GetNoDataValue() );
  properties->SetNoData( value );

  //
  colorDynWgt->SetNoDataButtonChecked( false );
}

/*****************************************************************************/
void
ColorDynamicsController
::OnLinkToggled( RgbwChannel channel, bool checked )
{
  // Trace.
  // qDebug()
  //   << this
  //   << "::OnLinkToggled("
  //   << RGBW_CHANNEL_NAMES[ channel ] << ", " << checked
  //   << ")";

  ResetIntensityRanges( channel );

  if( checked )
    {
    RestoreQuantiles( channel );

    emit ModelUpdated();
    }
}

/*****************************************************************************/
void
ColorDynamicsController
::OnGammaValueChanged( double value )
{
  // qDebug()
  //   << this
  //   << "::OnGammaValueChanged("
  //   << value
  //   <<")";

  // Get image-model.
  assert( GetModel()==GetModel< VectorImageModel>() );
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  // Get widget.
  // assert( GetWidget()==GetWidget< ColorDynamicsWidget >() );
  // ColorDynamicsWidget* widget = GetWidget< ColorDynamicsWidget >();
  // assert( widget!=NULL );

  // Store gamma into settings.
  // imageModel->GetSettings().SetGamma( widget->GetGamma() );
  imageModel->GetSettings().SetGamma( value );

  // Emit refresh signal.
  emit ModelUpdated();
}

/*****************************************************************************/

} // end namespace 'mvd'
