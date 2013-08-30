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
#include "Gui/mvdHistogramWidget.h"
#include "Gui/ui_mvdHistogramWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// Qwt includes
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_scale_engine.h>

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
  TRANSLATOR mvd::HistogramWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/**
 * \brief Array of enhanced band names that OTB can return.
 *
 * It is defined (statically) as a constant for translation purposes.
 */
const char*
HistogramWidget::CURVE_NAMES[ HistogramWidget::CURVE_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::HistogramWidget", "Red" ),
  QT_TRANSLATE_NOOP( "mvd::HistogramWidget", "Green" ),
  QT_TRANSLATE_NOOP( "mvd::HistogramWidget", "Blue" ),
  QT_TRANSLATE_NOOP( "mvd::HistogramWidget", "Gray" ), 
};

const QColor
HistogramWidget::CURVE_COLORS[ HistogramWidget::CURVE_COUNT ] =
{
  QColor( 0xFF, 0x44, 0x44, 0x77 ),
  QColor( 0x44, 0xFF, 0x44, 0x77 ),
  QColor( 0x44, 0x44, 0xFF, 0x77 ),
  QColor( 0xBB, 0xBB, 0xBB, 0x77 ),
};

const QColor
HistogramWidget::MARKER_COLORS[ HistogramWidget::CURVE_COUNT ] =
{
  QColor( 0xFF, 0x77, 0x77/*, 0x00*/ ),
  QColor( 0x77, 0xFF, 0x77/*, 0x77*/ ),
  QColor( 0x77, 0x77, 0xFF/*, 0x00*/ ),
  QColor( 0xFF, 0xFF, 0xFF/*, 0x00*/ ),
};

namespace
{
const QColor CANVAS_BACKGROUND( 0x33, 0x33, 0x33 );
const QColor GRID_MAJ_PEN_COLOR( 0x66, 0x66, 0x66 );
const QColor GRID_MIN_PEN_COLOR( 0x44, 0x44, 0x44 );
}


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramWidget
::HistogramWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::HistogramWidget() ),
  m_PlotGrid( NULL ),
  m_PlotCurves(),
  m_Bounds()
{
  m_UI->setupUi( this );

  m_UI->histogramPlot->setCanvasBackground( CANVAS_BACKGROUND  );

  m_PlotGrid = new QwtPlotGrid();
  m_PlotGrid->attach( m_UI->histogramPlot );

  m_PlotGrid->setMajPen( GRID_MAJ_PEN_COLOR );
  m_PlotGrid->setMinPen( GRID_MIN_PEN_COLOR );

  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    //
    // Curve
    m_PlotCurves[ i ] =
      new QwtPlotCurve( tr( HistogramWidget::CURVE_NAMES[ i ] ) );

#if HISTOGRAM_CURVE_TYPE==0

#elif HISTOGRAM_CURVE_TYPE==1
    m_PlotCurves[ i ]->setStyle( QwtPlotCurve::Steps );

#elif HISTOGRAM_CURVE_TYPE==2

#else
#endif

    m_PlotCurves[ i ]->setPen( QPen( CURVE_COLORS[ i ] ) );
    m_PlotCurves[ i ]->setBrush( QBrush( CURVE_COLORS[ i ] ) );

    m_PlotCurves[ i ]->attach( m_UI->histogramPlot );

    //
    // Markers

    m_LowPlotMarkers[ i ] = new QwtPlotMarker();
    m_LowPlotMarkers[ i ]->setLineStyle( QwtPlotMarker::VLine );
    m_LowPlotMarkers[ i ]->setLinePen(
      QPen( HistogramWidget::MARKER_COLORS[ i ] )
    );
    m_LowPlotMarkers[ i ]->attach( m_UI->histogramPlot );

    m_HighPlotMarkers[ i ] = new QwtPlotMarker();
    m_HighPlotMarkers[ i ]->setLineStyle( QwtPlotMarker::VLine );
    m_HighPlotMarkers[ i ]->setLinePen(
      QPen( HistogramWidget::MARKER_COLORS[ i ] )
    );
    m_HighPlotMarkers[ i ]->attach( m_UI->histogramPlot );
    }
}

/*******************************************************************************/
HistogramWidget
::~HistogramWidget()
{
  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    delete m_PlotCurves[ i ];
    m_PlotCurves[ i ] = NULL;

    delete m_LowPlotMarkers[ i ];
    m_LowPlotMarkers[ i ] = NULL;

    delete m_HighPlotMarkers[ i ];
    m_HighPlotMarkers[ i ] = NULL;
    }

  delete m_PlotGrid;
  m_PlotGrid = NULL;

  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
void
HistogramWidget
::SetBounds( RgbwChannel channel,
	     double xMin, double xMax,
	     double yMin, double yMax )
{
  CountType begin = 0;
  CountType end = 0;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    m_Bounds[ i ].m_XMin = xMin;
    m_Bounds[ i ].m_XMax = xMax;

    m_Bounds[ i ].m_YMin = yMin;
    m_Bounds[ i ].m_YMax = yMax;
    }

  RefreshScale();
}

/*******************************************************************************/
void
HistogramWidget
::SetData( RgbwChannel channel,
	   double * const x, double * const y, size_t size,
	   double xMin, double yMin,
	   double xMax, double yMax )
{
  assert( x!=NULL );
  assert( y!=NULL );

  CountType begin = 0;
  CountType end = 0;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    assert( i<HistogramWidget::CURVE_COUNT );
    assert( m_PlotCurves[ i ]!=NULL );

    m_PlotCurves[ i ]->setData( x, y, size );

    qDebug()
      << RGBW_CHANNEL_NAMES[ i ]
      << "[" << xMin << "; " << xMax << "]"
      << "x [" << yMin << "; " << yMax << "]";

    m_Bounds[ i ].m_XMin = xMin;
    m_Bounds[ i ].m_XMax = xMax;

    m_Bounds[ i ].m_YMin = yMin;
    m_Bounds[ i ].m_YMax = yMax;
    }
}

/*******************************************************************************/
void
HistogramWidget
::SetLowMarker( RgbwChannel channel,
		double low )
{
  qDebug()
    << this << "::SetLowMarker("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << low << ")";

  CountType begin = 0;
  CountType end = 0;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    m_LowPlotMarkers[ i ]->setXValue( low );
    }
}

/*******************************************************************************/
void
HistogramWidget
::SetHighMarker( RgbwChannel channel,
		double high )
{
  qDebug()
    << this << "::SetLowMarker("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << high << ")";

  CountType begin = 0;
  CountType end = 0;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    m_HighPlotMarkers[ i ]->setXValue( high );
    }
}

/*****************************************************************************/
void
HistogramWidget
::SetGrayscaleActivated( bool activated )
{
  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    bool isVisible = 
      i<RGBW_CHANNEL_WHITE
      ? !activated
      : activated;

    m_PlotCurves[ i ]->setVisible( isVisible );

    m_LowPlotMarkers[ i ]->setVisible( isVisible );
    m_HighPlotMarkers[ i ]->setVisible( isVisible );
    }
}

/*******************************************************************************/
void
HistogramWidget
::Replot()
{
  RefreshScale();

  m_UI->histogramPlot->replot();
}

/*******************************************************************************/
void
HistogramWidget
::RefreshScale()
{
  assert( std::numeric_limits< double >::has_infinity );

  double xMin = +std::numeric_limits< double >::infinity();
  double xMax = -std::numeric_limits< double >::infinity();

  double yMin = +std::numeric_limits< double >::infinity();
  double yMax = -std::numeric_limits< double >::infinity();

  CountType begin = 0;
  CountType end = 0;

  if( !RgbBounds( begin, end, RGBW_CHANNEL_RGB ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    if( m_Bounds[ i ].m_XMin<xMin )
      xMin = m_Bounds[ i ].m_XMin;

    if( m_Bounds[ i ].m_XMax>xMax )
      xMax = m_Bounds[ i ].m_XMax;

    if( m_Bounds[ i ].m_YMin<yMin )
      yMin = m_Bounds[ i ].m_YMin;

    if( m_Bounds[ i ].m_YMax>yMax )
      yMax = m_Bounds[ i ].m_YMax;
    }

  qDebug()
    << "[" << xMin << "; " << xMax << "]"
    << "x [" << yMin << "; " << yMax << "]";

  m_UI->histogramPlot
    ->axisScaleDiv( QwtPlot::xBottom )
    ->setInterval( xMin, xMax );

  m_UI->histogramPlot
    ->axisScaleDiv( QwtPlot::yLeft )
    ->setInterval( yMin, yMax );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
