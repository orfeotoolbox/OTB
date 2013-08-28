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
#include "Gui/mvdHistogramController.h"


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
#include "Gui/mvdHistogramWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::HistogramController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramController
::HistogramController( HistogramWidget* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
HistogramController
::~HistogramController()
{
}

/*******************************************************************************/
void
HistogramController
::Connect( AbstractModel* model )
{
  // HistogramWidget* widget = GetWidget< HistogramWidget >();

  //
  // Connect GUI to controller.

  //
  // Connect controller to model.
}

/*******************************************************************************/
void
HistogramController
::Disconnect( AbstractModel* model )
{
  // HistogramWidget* widget = GetWidget< HistogramWidget >();

  //
  // Disconnect controller to model.

  //
  // Disconnect GUI from controller.
}

/*******************************************************************************/
void
HistogramController
::ResetWidget()
{
  ResetWidget( RGBW_CHANNEL_RGB );
}

/*******************************************************************************/
void
HistogramController
::ResetWidget( RgbwChannel channel )
{
  assert( GetModel()==GetModel< VectorImageModel >() );
  VectorImageModel* imageModel = GetModel< VectorImageModel >();
  assert( imageModel!=NULL );

  HistogramModel* model = imageModel->GetHistogramModel();
  assert( model!=NULL );

  assert( GetWidget()==GetWidget< HistogramWidget >() );
  HistogramWidget* widget = GetWidget< HistogramWidget >();
  assert( widget!=NULL );

  CountType begin = 0;
  CountType end = 0;

  if( !RgbBounds( begin,
		  end,
		  channel==RGBW_CHANNEL_WHITE ? RGBW_CHANNEL_RGB : channel ) )
    return;

  assert( std::numeric_limits< double >::has_quiet_NaN );

  for( CountType i=begin; i<end; ++i )
    {
    size_t size = model->GetDataCount( i );

    double* x = new double[ size ];
    double* y = new double[ size ];

    double xMin = std::numeric_limits< double >::quiet_NaN();
    double yMin = std::numeric_limits< double >::quiet_NaN();
    double xMax = std::numeric_limits< double >::quiet_NaN();
    double yMax = std::numeric_limits< double >::quiet_NaN();

    const VectorImageModel::Settings& settings = imageModel->GetSettings();

    RgbwChannel outChan = static_cast< RgbwChannel >( i );

    RgbwChannel inChan =
      channel==RGBW_CHANNEL_WHITE
	? RGBW_CHANNEL_WHITE
      : outChan;

    VectorImageModel::Settings::ChannelVector::value_type band =
      settings.GetRgbwChannel( inChan );

    model->GetData( band, x, y, xMin, xMax, yMin, yMax );

    widget->SetData( outChan, x, y, size, xMin, yMin, xMax, yMax );

    widget->SetLowMarker( outChan, settings.GetLowIntensity( inChan ) );
    widget->SetHighMarker( outChan, settings.GetHighIntensity( inChan ) );

    delete x;
    x = NULL;

    delete y;
    y = NULL;
    }

  widget->Replot();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*****************************************************************************/
void
HistogramController
::OnRgbChannelIndexChanged( RgbwChannel channel, int band )
{
  qDebug()
    << this
    << "::OnRgbChannelIndexChanged("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << band <<
    ")";

  ResetWidget( channel );
}

/*****************************************************************************/
void
HistogramController
::OnGrayChannelIndexChanged( int band )
{
  qDebug()
    << this
    << "::OnGrayChannelIndexChanged(" << band << ")";

  ResetWidget( RGBW_CHANNEL_WHITE );
}

/*******************************************************************************/
void
HistogramController
::OnGrayscaleActivated( bool activated )
{
  qDebug()
    << this
    << "::OnGrayscaleActivated(" << activated << ")";

  ResetWidget(
    activated
    ? RGBW_CHANNEL_WHITE
    : RGBW_CHANNEL_RGB
  );
}

/*******************************************************************************/
void
HistogramController
::OnLowIntensityChanged( RgbwChannel channel, double value, bool refresh )
{
  assert( GetWidget()==GetWidget< HistogramWidget >() );
  HistogramWidget* widget = GetWidget< HistogramWidget >();
  assert( widget!=NULL );

  widget->SetLowMarker(
    channel==RGBW_CHANNEL_WHITE
    ? RGBW_CHANNEL_RGB
    : channel,
    value
  );

  if( refresh )
    widget->Replot();
}

/*******************************************************************************/
void
HistogramController
::OnHighIntensityChanged( RgbwChannel channel, double value, bool refresh )
{
  assert( GetWidget()==GetWidget< HistogramWidget >() );
  HistogramWidget* widget = GetWidget< HistogramWidget >();
  assert( widget!=NULL );

  widget->SetHighMarker(
    channel==RGBW_CHANNEL_WHITE
    ? RGBW_CHANNEL_RGB
    : channel,
    value
  );

  if( refresh )
    widget->Replot();
}

/*******************************************************************************/

} // end namespace 'mvd'
