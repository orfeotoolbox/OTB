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
#include <qwt_plot_magnifier.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_panner.h>
// #include <qwt_plot_zoomer.h>
#include <qwt_scale_engine.h>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdHistogramPlotPicker.h"

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
  QColor( 0xFF, 0x44, 0x44, 0x66 ),
  QColor( 0x44, 0xFF, 0x44, 0x66 ),
  QColor( 0x44, 0x44, 0xFF, 0x66 ),
  QColor( 0xBB, 0xBB, 0xBB, 0x66 ),
};

const QColor
HistogramWidget::MARKER_COLORS[ HistogramWidget::CURVE_COUNT ] =
{
  QColor( 0xFF, 0x77, 0x77/*, 0x00*/ ),
  QColor( 0x77, 0xFF, 0x77/*, 0x77*/ ),
  QColor( 0x77, 0x77, 0xFF/*, 0x00*/ ),
  QColor( 0xFF, 0xFF, 0xFF/*, 0x00*/ ),
};

const QColor
HistogramWidget::RUBBER_BAND_COLORS[ HistogramWidget::CURVE_COUNT ] =
{
  QColor( 0xFF, 0x77, 0x77/*, 0xAA */),
  QColor( 0x77, 0xFF, 0x77/*, 0xAA */ ),
  QColor( 0x77, 0x77, 0xFF/*, 0xAA */ ),
  QColor( 0xFF, 0xFF, 0xFF/*, 0xAA */ ),
};

namespace
{
const QColor CANVAS_BACKGROUND( 0x33, 0x33, 0x33 );
const QColor GRID_MAJ_PEN_COLOR( 0x66, 0x66, 0x66 );
const QColor GRID_MIN_PEN_COLOR( 0x44, 0x44, 0x44 );
const QColor RUBBER_BAND_COLOR( 0xFF, 0xFF, 0x00, 0xAA );
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
  m_PlotPicker( NULL ),
  // m_PlotZoomer( NULL ),
  m_PlotMagnifier( NULL ),
  m_PlotPanner( NULL ),
  m_PlotCurves(),
  m_LowPlotMarkers(),
  m_HighPlotMarkers(),
  m_Bounds()
{
  m_UI->setupUi( this );

  m_UI->histogramPlot->setCanvasBackground( CANVAS_BACKGROUND  );


  //
  // GRID.

  m_PlotGrid = new QwtPlotGrid();
  m_PlotGrid->attach( m_UI->histogramPlot );

  m_PlotGrid->setMajPen( GRID_MAJ_PEN_COLOR );
  m_PlotGrid->setMinPen( GRID_MIN_PEN_COLOR );

  //
  // ZOOMER.
  /*
  m_PlotZoomer = new QwtPlotZoomer( m_UI->histogramPlot->canvas() );
  m_PlotZoomer->setTrackerMode( QwtPicker::AlwaysOff );
  m_PlotZoomer->setSelectionFlags(
    QwtPicker::RectSelection |
    QwtPicker::DragSelection );
  m_PlotZoomer->setRubberBandPen( QPen( QColor( 0xFF, 0x44, 0xFF ) ) );
  m_PlotZoomer->setTrackerPen( QPen( QColor( 0xFF, 0x44, 0xFF ) ) );
  */
  m_PlotMagnifier = new QwtPlotMagnifier( m_UI->histogramPlot->canvas() );
  m_PlotMagnifier->setAxisEnabled( QwtPlot::yLeft, false );
  m_PlotMagnifier->setWheelFactor( 1.0 / m_PlotMagnifier->wheelFactor() );

  m_PlotPanner = new QwtPlotPanner( m_UI->histogramPlot->canvas() );
  // m_PlotPanner->setAxisEnabled( QwtPlot::yLeft, false );
  m_PlotPanner->setMouseButton( Qt::RightButton );
  m_PlotPanner->setOrientations( Qt::Horizontal );

  //
  // CURVES.

  HistogramPlotPicker::PlotCurveVector curves( HistogramWidget::CURVE_COUNT );

  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    //
    // Curve

    curves[ i ] =
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

  //
  // PICKER.

  m_PlotPicker =
    new HistogramPlotPicker( curves, m_UI->histogramPlot->canvas() );
  m_PlotPicker->setTrackerMode( QwtPicker::AlwaysOn );
  m_PlotPicker->setRubberBandPen( RUBBER_BAND_COLOR );
  m_PlotPicker->setTrackerPen( QColor( Qt::yellow ) );

  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    m_PlotPicker->SetRubberBandPen(
      static_cast< RgbwChannel >( i ),
      QPen( HistogramWidget::RUBBER_BAND_COLORS[ i ] )
    );
    }

  //
  // CONNECTIONS.

  //
  //
  QObject::connect(
    m_PlotPicker, SIGNAL( appended( const QwtDoublePoint& ) ),
    this, SLOT( OnAppended( const QwtDoublePoint& ) )
  );
  QObject::connect(
    m_PlotPicker, SIGNAL( appended( const QPoint& ) ),
    this, SLOT( OnAppended( const QPoint& ) )
  );
  //
  QObject::connect(
    m_PlotPicker, SIGNAL( changed( const QwtPolygon& ) ),
    this, SLOT( OnChanged( const QwtPolygon& ) )
  );
  //
  QObject::connect(
    m_PlotPicker, SIGNAL( selected( const QwtDoublePoint& ) ),
    this, SLOT( OnSelected( const QwtDoublePoint& ) )
  );
  QObject::connect(
    m_PlotPicker, SIGNAL( selected( const QwtDoubleRect& ) ),
    this, SLOT( OnSelected( const QwtDoubleRect& ) )
  );
  QObject::connect(
    m_PlotPicker, SIGNAL( selected( const QwtPolygon& ) ),
    this, SLOT( OnSelected( const QwtPolygon& ) )
  );
  QObject::connect(
    m_PlotPicker, SIGNAL( selected( const QwtArray< QwtDoublePoint >& ) ),
    this, SLOT( OnSelected( const QwtArray< QwtDoublePolygon >& ) )
  );
}

/*******************************************************************************/
HistogramWidget
::~HistogramWidget()
{
  delete m_PlotPicker;
  m_PlotPicker = NULL;

  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    delete m_PlotCurves[ i ];
    m_PlotCurves[ i ] = NULL;

    delete m_LowPlotMarkers[ i ];
    m_LowPlotMarkers[ i ] = NULL;

    delete m_HighPlotMarkers[ i ];
    m_HighPlotMarkers[ i ] = NULL;
    }

  delete m_PlotPanner;
  m_PlotPanner = NULL;

  delete m_PlotMagnifier;
  m_PlotMagnifier = NULL;

  /*
  delete m_PlotZoomer;
  m_PlotZoomer = NULL;
  */

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
  assert( ( x==NULL && y==NULL && size==0 ) ||
	  ( x!=NULL && y!=NULL && size!=0 ) );

  CountType begin = 0;
  CountType end = 0;

  if( !RgbwBounds( begin, end, channel ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    assert( i<HistogramWidget::CURVE_COUNT );
    assert( m_PlotCurves[ i ]!=NULL );

    m_PlotCurves[ i ]->setData( x, y, size );

    if( x==NULL && y==NULL )
      m_PlotCurves[ i ]->setVisible( false );

    /*
    qDebug()
      << RGBW_CHANNEL_NAMES[ i ]
      << "[" << xMin << "; " << xMax << "]"
      << "x [" << yMin << "; " << yMax << "]";
    */

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

  m_PlotPicker->SetGrayscaleActivated( activated );
}

/*****************************************************************************/
/*
bool
HistogramWidget
::IsGrayscaleActivated() const
{
  assert( m_PlotCurves[ RGBW_CHANNEL_WHITE ]!=NULL );

  return m_PlotCurves[ RGBW_CHANNEL_WHITE ]->isVisible();
}
*/

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

  if( !RgbwBounds( begin, end, RGBW_CHANNEL_ALL ) )
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

#if 0
  m_UI->histogramPlot
    ->axisScaleDiv( QwtPlot::xBottom )
    ->setInterval( xMin, xMax );

  m_UI->histogramPlot
    ->axisScaleDiv( QwtPlot::yLeft )
    ->setInterval( yMin, yMax );
#else
  m_UI->histogramPlot->setAxisScale( QwtPlot::xBottom, xMin, xMax );
  m_UI->histogramPlot->setAxisScale( QwtPlot::yLeft, yMin, yMax );
#endif
}

/*******************************************************************************/
void
HistogramWidget
::Clear()
{
  CountType begin = 0;
  CountType end = 0;

  if( !RgbwBounds( begin, end, RGBW_CHANNEL_ALL ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    m_PlotCurves[ i ]->setVisible( false );
    m_LowPlotMarkers[ i ]->setVisible( false );
    m_HighPlotMarkers[ i ]->setVisible( false );
    /*
    qDebug()
      << RGBW_CHANNEL_NAMES[ i ]
      << "[" << xMin << "; " << xMax << "]"
      << "x [" << yMin << "; " << yMax << "]";
    */

    m_Bounds[ i ].m_XMin = 0.0;
    m_Bounds[ i ].m_XMax = 1000.0;

    m_Bounds[ i ].m_YMin = 0.0;
    m_Bounds[ i ].m_YMax = 1000.0;
    }

  Replot();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
HistogramWidget
::OnAppended( const QwtDoublePoint& pos )
{
  qDebug() << this << "::OnAppended(" << pos.x() << ", " << pos.y() << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnAppended( const QPoint& pos )
{
  qDebug() << this << "::OnAppended(" << pos << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnChanged( const QwtPolygon& pa )
{
  qDebug() << this << "::OnChanged(" << pa << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnMoved( const QwtDoublePoint& pos )
{
  qDebug() << this << "::OnMoved(" << pos.x() << ", " << pos.y() << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnMoved( const QPoint& pos )
{
  qDebug() << this << "::OnMoved(" << pos << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnSelected( const QwtDoublePoint& pos )
{
  qDebug() << this << "::OnSelected(" << pos.x() << ", " << pos.y() << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnSelected( const QwtDoubleRect& rect )
{
  qDebug() << this << "::OnSelected(" << rect.x() << ", " << rect.y() << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnSelected( const QwtPolygon& pa )
{
  qDebug() << this << "::OnSelected(" << pa << ")";
}

/*******************************************************************************/
void
HistogramWidget
::OnSelected( const QwtArray< QwtDoublePoint >& pa )
{
  qDebug() << this << "::OnSelected(" << pa << ")";
}

} // end namespace 'mvd'
