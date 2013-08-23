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
};

const QColor
HistogramWidget::CURVE_COLORS[ HistogramWidget::CURVE_COUNT ] =
{
  QColor( 0xFF, 0x44, 0x44/*, 0x88*/ ),
  QColor( 0x44, 0xFF, 0x44/*, 0x88*/ ),
  QColor( 0x44, 0x44, 0xFF/*, 0x88*/ )
};

namespace
{
const QColor CANVAS_BACKGROUND( 0x44, 0x44, 0x44 );
const QColor GRID_MAJ_PEN_COLOR( 0x88, 0x88, 0x88 );
const QColor GRID_MIN_PEN_COLOR( 0x66, 0x66, 0x66 );
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
  m_PlotCurves()
{
  m_UI->setupUi( this );

  m_UI->histogramPlot->setCanvasBackground( CANVAS_BACKGROUND  );

  m_PlotGrid = new QwtPlotGrid();
  m_PlotGrid->attach( m_UI->histogramPlot );

  m_PlotGrid->setMajPen( GRID_MAJ_PEN_COLOR );
  m_PlotGrid->setMinPen( GRID_MIN_PEN_COLOR );

  for( CountType i=0; i<HistogramWidget::CURVE_COUNT; ++i )
    {
    m_PlotCurves[ i ] =
      new QwtPlotCurve( tr( HistogramWidget::CURVE_NAMES[ i ] ) );

#if HISTOGRAM_CURVE_TYPE==0

#elif HISTOGRAM_CURVE_TYPE==1
    m_PlotCurves[ i ]->setStyle( QwtPlotCurve::Steps );

#elif HISTOGRAM_CURVE_TYPE==2

#else
#endif

    m_PlotCurves[ i ]->setPen( QPen( CURVE_COLORS[ i ] ) );

    m_PlotCurves[ i ]->attach( m_UI->histogramPlot );
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
    }

  delete m_PlotGrid;
  m_PlotGrid = NULL;

  delete m_UI;
  m_UI = NULL;
}

/*******************************************************************************/
void
HistogramWidget
::SetData( RgbwChannel channel, double * const x, double * const y, size_t size )
{
  assert( x!=NULL );
  assert( y!=NULL );

  CountType begin = 0;
  CountType end = 0;

  if( !RgbBounds( begin, end, channel ) )
    return;

  for( CountType i=begin; i<end; ++i )
    {
    assert( i<HistogramWidget::CURVE_COUNT );
    assert( m_PlotCurves[ i ]!=NULL );

    m_PlotCurves[ i ]->setData( x, y, size );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
