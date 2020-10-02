/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mvdHistogramWidget.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#endif

#include "ui_mvdHistogramWidget.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// Qwt includes

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#endif

#if QWT_IS_ABOVE_6_1
#include <qwt_plot_canvas.h>
#endif // QWT_IS_ABOVE_6_1
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_panner.h>
// #include <qwt_plot_zoomer.h>
#include <qwt_scale_engine.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdHistogramPlotPicker.h"

namespace mvd
{

/*
  TRANSLATOR mvd::HistogramWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

#define PRECISION_MARGIN 4

/**
 * \brief Array of enhanced band names that OTB can return.
 *
 * It is defined (statically) as a constant for translation purposes.
 */
const char* HistogramWidget::CURVE_NAMES[HistogramWidget::CURVE_COUNT] = {
    QT_TRANSLATE_NOOP("mvd::HistogramWidget", "Red"), QT_TRANSLATE_NOOP("mvd::HistogramWidget", "Green"), QT_TRANSLATE_NOOP("mvd::HistogramWidget", "Blue"),
    QT_TRANSLATE_NOOP("mvd::HistogramWidget", "Gray"),
};

const QColor HistogramWidget::CURVE_COLORS[HistogramWidget::CURVE_COUNT] = {
    QColor(0xFF, 0x44, 0x44 /*, 0x66*/), QColor(0x44, 0xFF, 0x44 /*, 0x66*/), QColor(0x44, 0x44, 0xFF /*, 0x66*/), QColor(0xBB, 0xBB, 0xBB /*, 0x66*/),
};

const QColor HistogramWidget::BAR_COLORS[HistogramWidget::CURVE_COUNT] = {
    QColor(0xFF, 0x44, 0x44, 0x66), QColor(0x44, 0xFF, 0x44, 0x66), QColor(0x44, 0x44, 0xFF, 0x66), QColor(0xBB, 0xBB, 0xBB, 0x66),
};

const QColor HistogramWidget::MARKER_COLORS[HistogramWidget::CURVE_COUNT] = {
    QColor(0xFF, 0x77, 0x77 /*, 0x00*/), QColor(0x77, 0xFF, 0x77 /*, 0x77*/), QColor(0x77, 0x77, 0xFF /*, 0x00*/), QColor(0xFF, 0xFF, 0xFF /*, 0x00*/),
};

const QColor HistogramWidget::RUBBER_BAND_COLORS[HistogramWidget::CURVE_COUNT] = {
    QColor(0xFF, 0x77, 0x77 /*, 0xAA */), QColor(0x77, 0xFF, 0x77 /*, 0xAA */), QColor(0x77, 0x77, 0xFF /*, 0xAA */), QColor(0xFF, 0xFF, 0xFF /*, 0xAA */),
};

namespace
{
const QColor CANVAS_BACKGROUND(0x33, 0x33, 0x33);
const QColor GRID_MAJ_PEN_COLOR(0x66, 0x66, 0x66);
const QColor GRID_MIN_PEN_COLOR(0x44, 0x44, 0x44);
const QColor RUBBER_BAND_COLOR(0xFF, 0xFF, 0x00, 0xAA);
}


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramWidget::HistogramWidget(QWidget* p, Qt::WindowFlags flags)
  : QWidget(p, flags),
    m_UI(new mvd::Ui::HistogramWidget()),
    m_PlotGrid(NULL),
    m_PlotPicker(NULL),
    // m_PlotZoomer( NULL ),
    m_PlotMagnifier(NULL),
    m_PlotPanner(NULL),
    // m_PlotCurves(),
    // m_LowPlotMarkers(),
    // m_HighPlotMarkers(),
    m_Bounds(),
    m_Precision(0.0),
    m_IsGrayscaleActivated(false)
{
  m_UI->setupUi(this);

  m_UI->histogramPlot->setCanvasBackground(CANVAS_BACKGROUND);


  //
  // GRID.

  m_PlotGrid = new QwtPlotGrid();
  m_PlotGrid->attach(m_UI->histogramPlot);

#if QWT_IS_ABOVE_6_1
  m_PlotGrid->setMajorPen(GRID_MAJ_PEN_COLOR);
  m_PlotGrid->setMinorPen(GRID_MIN_PEN_COLOR);

#else  // QWT_IS_ABOVE_6_1
  m_PlotGrid->setMajPen(GRID_MAJ_PEN_COLOR);
  m_PlotGrid->setMinPen(GRID_MIN_PEN_COLOR);
#endif // QWT_IS_ABOVE_6_1

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
  m_PlotMagnifier = new QwtPlotMagnifier(m_UI->histogramPlot->canvas());
  m_PlotMagnifier->setAxisEnabled(QwtPlot::yLeft, false);
  m_PlotMagnifier->setWheelFactor(1.0 / m_PlotMagnifier->wheelFactor());
  m_PlotMagnifier->setMouseButton(Qt::MidButton);
  // m_PlotMagnifier->setZoomInKey( Qt::Key_Plus, Qt::NoModifier );
  // m_PlotMagnifier->setZoomOutKey( Qt::Key_Minus, Qt::NoModifier );

  m_PlotPanner = new QwtPlotPanner(m_UI->histogramPlot->canvas());
  // m_PlotPanner->setAxisEnabled( QwtPlot::yLeft, false );
  m_PlotPanner->setMouseButton(Qt::RightButton);
  m_PlotPanner->setOrientations(Qt::Horizontal);

  //
  // CURVES.

  HistogramPlotPicker::PlotCurveVector curves(HistogramWidget::CURVE_COUNT);

  for (CountType i = 0; i < HistogramWidget::CURVE_COUNT; ++i)
  {
    //
    // Curve

    curves[i] = m_PlotCurves[i] = new QwtPlotCurve(tr(HistogramWidget::CURVE_NAMES[i]));

#if HISTOGRAM_CURVE_TYPE == 0

#elif HISTOGRAM_CURVE_TYPE == 1
    m_PlotCurves[i]->setStyle(QwtPlotCurve::Steps);

#elif HISTOGRAM_CURVE_TYPE == 2

#else
#endif

    m_PlotCurves[i]->setPen(QPen(CURVE_COLORS[i]));
    m_PlotCurves[i]->setBrush(QBrush(BAR_COLORS[i]));

    m_PlotCurves[i]->attach(m_UI->histogramPlot);

    //
    // Markers

    m_LowPlotMarkers[i] = new QwtPlotMarker();
    m_LowPlotMarkers[i]->setLineStyle(QwtPlotMarker::VLine);
    m_LowPlotMarkers[i]->setLinePen(QPen(HistogramWidget::MARKER_COLORS[i]));
    m_LowPlotMarkers[i]->attach(m_UI->histogramPlot);

    m_HighPlotMarkers[i] = new QwtPlotMarker();
    m_HighPlotMarkers[i]->setLineStyle(QwtPlotMarker::VLine);
    m_HighPlotMarkers[i]->setLinePen(QPen(HistogramWidget::MARKER_COLORS[i]));
    m_HighPlotMarkers[i]->attach(m_UI->histogramPlot);
  }

  //
  // PICKER.
  assert(dynamic_cast<QwtPlotCanvas*>(m_UI->histogramPlot->canvas()) != nullptr);

  m_PlotPicker = new HistogramPlotPicker(curves,
#if QWT_IS_ABOVE_6_1
                                         dynamic_cast<QwtPlotCanvas*>(
#endif // QWT_IS_ABOVE_6_1
                                             m_UI->histogramPlot->canvas()
#if QWT_IS_ABOVE_6_1
                                                 )
#endif // QWT_IS_ABOVE_6_1
                                             );

  m_PlotPicker->setTrackerMode(QwtPicker::ActiveOnly);
  m_PlotPicker->setRubberBandPen(RUBBER_BAND_COLOR);
  m_PlotPicker->setTrackerPen(QColor(Qt::yellow));

  for (CountType i = 0; i < HistogramWidget::CURVE_COUNT; ++i)
  {
    m_PlotPicker->SetRubberBandPen(static_cast<RgbwChannel>(i), QPen(HistogramWidget::RUBBER_BAND_COLORS[i]));
  }

  //
  // CONNECTIONS.

  //
  //
  QObject::connect(m_PlotPicker, SIGNAL(appended(const QPointF&)), this, SLOT(OnAppended(const QPointF&)));
  QObject::connect(m_PlotPicker, SIGNAL(appended(const QPoint&)), this, SLOT(OnAppended(const QPoint&)));
  //
  QObject::connect(m_PlotPicker, SIGNAL(changed(const QPolygon&)), this, SLOT(OnChanged(const QPolygon&)));
  //
  QObject::connect(m_PlotPicker, SIGNAL(selected(const QPointF&)), this, SLOT(OnSelected(const QPointF&)));
  QObject::connect(m_PlotPicker, SIGNAL(selected(const QRectF&)), this, SLOT(OnSelected(const QRectF&)));
  QObject::connect(m_PlotPicker, SIGNAL(selected(const QPolygon&)), this, SLOT(OnSelected(const QPolygon&)));
  QObject::connect(m_PlotPicker, SIGNAL(selected(const QVector<QPointF>&)), this, SLOT(OnSelected(const QVector<QPointF>&)));
}

/*******************************************************************************/
HistogramWidget::~HistogramWidget()
{
  delete m_PlotPicker;
  m_PlotPicker = NULL;

  for (CountType i = 0; i < HistogramWidget::CURVE_COUNT; ++i)
  {
    delete m_PlotCurves[i];
    m_PlotCurves[i] = NULL;

    delete m_LowPlotMarkers[i];
    m_LowPlotMarkers[i] = NULL;

    delete m_HighPlotMarkers[i];
    m_HighPlotMarkers[i] = NULL;
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
void HistogramWidget::SetPrecision(double p)
{
  m_Precision = p;
}

/*******************************************************************************/
double HistogramWidget::GetPrecision() const
{
  return m_Precision;
}

/*******************************************************************************/
void HistogramWidget::SetBounds(RgbwChannel channel, double xMin, double xMax, double yMin, double yMax)
{
  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, channel))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    m_Bounds[i].m_XMin = xMin;
    m_Bounds[i].m_XMax = xMax;

    m_Bounds[i].m_YMin = yMin;
    m_Bounds[i].m_YMax = yMax;
  }

  // RefreshScale();
}

/*******************************************************************************/
void HistogramWidget::SetData(RgbwChannel channel, double* const xVal, double* const yVal, size_t sizeVal, double xMin, double yMin, double xMax, double yMax)
{
  assert((xVal == NULL && yVal == NULL && sizeVal == 0) || (xVal != NULL && yVal != NULL && sizeVal != 0));

  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, channel))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    assert(i < HistogramWidget::CURVE_COUNT);
    assert(m_PlotCurves[i] != NULL);

    m_PlotCurves[i]->setSamples(xVal, yVal, sizeVal);

    if (xVal == NULL && yVal == NULL)
      m_PlotCurves[i]->setVisible(false);

    /*
    qDebug()
      << RGBW_CHANNEL_NAMES[ i ]
      << "[" << xMin << "; " << xMax << "]"
      << "xVal [" << yMin << "; " << yMax << "]";
    */

    m_Bounds[i].m_XMin = xMin;
    m_Bounds[i].m_XMax = xMax;

    m_Bounds[i].m_YMin = yMin;
    m_Bounds[i].m_YMax = yMax;
  }
}

/*******************************************************************************/
void HistogramWidget::SetLowMarker(RgbwChannel channel, double low)
{
  /*
  qDebug()
    << this << "::SetLowMarker("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << low << ")";
  */

  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, channel))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    m_LowPlotMarkers[i]->setXValue(low);

    m_Bounds[i].m_QMin = low;
  }
}

/*******************************************************************************/
void HistogramWidget::SetHighMarker(RgbwChannel channel, double high)
{
  /*
  qDebug()
    << this << "::SetLowMarker("
    << RGBW_CHANNEL_NAMES[ channel ] << ", " << high << ")";
  */

  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, channel))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    m_HighPlotMarkers[i]->setXValue(high);

    m_Bounds[i].m_QMax = high;
  }
}

/*****************************************************************************/
void HistogramWidget::SetGrayscaleActivated(bool activated)
{
  m_IsGrayscaleActivated = activated;

  /*
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
  */

  m_UI->channelComboBox->setEnabled(!activated);

  bool areSignalsBlocked = m_UI->channelComboBox->blockSignals(true);
  {
    m_UI->channelComboBox->setItemText(3, activated ? tr("White") : tr("RGB"));
    m_UI->channelComboBox->setCurrentIndex(3);
    UpdateCurvesVisibility(3);
  }
  m_UI->channelComboBox->blockSignals(areSignalsBlocked);
  m_PlotPicker->SetGrayscaleActivated(activated);
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
void HistogramWidget::Replot()
{
  // RefreshScale( true );

  m_UI->histogramPlot->replot();
}

/*******************************************************************************/
void HistogramWidget::UpdateCurvesVisibility(CountType index)
{
  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, RGBW_CHANNEL_ALL))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    bool isCurveVisible = m_IsGrayscaleActivated ? i == RGBW_CHANNEL_WHITE : (index == RGBW_CHANNEL_WHITE ? i < RGBW_CHANNEL_WHITE : i == index);

    assert(i < HistogramWidget::CURVE_COUNT);

    m_PlotCurves[i]->setVisible(isCurveVisible);
    m_LowPlotMarkers[i]->setVisible(isCurveVisible);
    m_HighPlotMarkers[i]->setVisible(isCurveVisible);
  }
}

/*******************************************************************************/
void HistogramWidget::RefreshScale(bool iqr)
{
  assert(std::numeric_limits<double>::has_infinity);

  double xMin = +std::numeric_limits<double>::infinity();
  double xMax = -std::numeric_limits<double>::infinity();

  double yMin = +std::numeric_limits<double>::infinity();
  double yMax = -std::numeric_limits<double>::infinity();

  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, RGBW_CHANNEL_ALL))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    if (iqr)
    {
      if (m_Bounds[i].m_QMin < xMin)
        xMin = m_Bounds[i].m_QMin;

      if (m_Bounds[i].m_QMax > xMax)
        xMax = m_Bounds[i].m_QMax;
    }
    else
    {
      if (m_Bounds[i].m_XMin < xMin)
        xMin = m_Bounds[i].m_XMin;

      if (m_Bounds[i].m_XMax > xMax)
        xMax = m_Bounds[i].m_XMax;
    }

    if (m_Bounds[i].m_YMin < yMin)
      yMin = m_Bounds[i].m_YMin;

    if (m_Bounds[i].m_YMax > yMax)
      yMax = m_Bounds[i].m_YMax;
  }

  if (xMin == xMax)
  {
    double epsilon = PRECISION_MARGIN * m_Precision;

    if (xMin >= std::numeric_limits<double>::min() + epsilon)
      xMin -= epsilon;

    if (xMax <= std::numeric_limits<double>::max() - epsilon)
      xMax += epsilon;
  }

  // qDebug()
  //   << "[" << xMin << "; " << xMax << "]"
  //   << "x [" << yMin << "; " << yMax << "]";

  m_UI->histogramPlot->setAxisScale(QwtPlot::xBottom, xMin, xMax);
  m_UI->histogramPlot->setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

/*******************************************************************************/
void HistogramWidget::Clear()
{
  CountType begin = 0;
  CountType end   = 0;

  if (!RgbwBounds(begin, end, RGBW_CHANNEL_ALL))
    return;

  for (CountType i = begin; i < end; ++i)
  {
    m_PlotCurves[i]->setVisible(false);
    m_LowPlotMarkers[i]->setVisible(false);
    m_HighPlotMarkers[i]->setVisible(false);
    /*
    qDebug()
      << RGBW_CHANNEL_NAMES[ i ]
      << "[" << xMin << "; " << xMax << "]"
      << "x [" << yMin << "; " << yMax << "]";
    */

    m_Bounds[i].m_XMin = 0.0;
    m_Bounds[i].m_XMax = 1000.0;

    m_Bounds[i].m_YMin = 0.0;
    m_Bounds[i].m_YMax = 1000.0;

    m_Bounds[i].m_QMin = 0.0;
    m_Bounds[i].m_QMax = 1000.0;
  }

  RefreshScale(false);

  Replot();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void HistogramWidget::on_zoom1Button_clicked()
{
  RefreshScale(false);
  Replot();
}

/*******************************************************************************/
void HistogramWidget::on_zoomQButton_clicked()
{
  RefreshScale(true);
  Replot();
}

/*******************************************************************************/
void HistogramWidget::on_channelComboBox_currentIndexChanged(int /*index*/)
{
  UpdateCurvesVisibility(m_UI->channelComboBox->currentIndex());

  Replot();
}

/*******************************************************************************/
void HistogramWidget::OnAppended(const QPointF&)
{
  // qDebug() << this << "::OnAppended(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnAppended(const QPoint&)
{
  // qDebug() << this << "::OnAppended(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnChanged(const QPolygon&)
{
  // qDebug() << this << "::OnChanged(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnMoved(const QPointF&)
{
  // qDebug() << this << "::OnMoved(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnMoved(const QPoint&)
{
  // qDebug() << this << "::OnMoved(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnSelected(const QPointF&)
{
  // qDebug() << this << "::OnSelected(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnSelected(const QRectF&)
{
  // qDebug() << this << "::OnSelected(" << r << ")";
}

/*******************************************************************************/
void HistogramWidget::OnSelected(const QPolygon&)
{
  // qDebug() << this << "::OnSelected(" << p << ")";
}

/*******************************************************************************/
void HistogramWidget::OnSelected(const QVector<QPointF>&)
{
  // qDebug() << this << "::OnSelected(" << v << ")";
}

} // end namespace 'mvd'
