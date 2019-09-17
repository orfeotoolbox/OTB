/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "mvdHistogramPlotPicker.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// Qwt includes.

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#endif

#include <qwt_plot_curve.h>
#include <qwt_painter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot.h>
#if QWT_IS_ABOVE_6_1
#include <qwt_plot_canvas.h>
#endif // QWT_ABOVE_6_1
#include <qwt_text.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdHistogramWidget.h"

namespace mvd
{

/*
  TRANSLATOR mvd::HistogramPlotPicker

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramPlotPicker::HistogramPlotPicker(const PlotCurveVector& curves, QwtPlotCanvas* can)
  : QwtPlotPicker(can), m_PlotCurves(curves), m_RubberBandPens(), m_IsGrayscaleActivated(false)
{
  assert(m_PlotCurves.size() == HistogramPlotPicker::CURVE_COUNT);

  setStateMachine(new QwtPickerDragPointMachine());

  setRubberBand(QwtPicker::UserRubberBand);
}

/*******************************************************************************/
HistogramPlotPicker::HistogramPlotPicker(const PlotCurveVector& curves, int xA, int yA, QwtPlotCanvas* can)
  : QwtPlotPicker(xA, yA, can), m_PlotCurves(curves), m_RubberBandPens(), m_IsGrayscaleActivated(false)

{
  assert(m_PlotCurves.size() == HistogramPlotPicker::CURVE_COUNT);

  setStateMachine(new QwtPickerDragPointMachine());

  setRubberBand(QwtPicker::UserRubberBand);
}

/*******************************************************************************/
HistogramPlotPicker::HistogramPlotPicker(const PlotCurveVector& curves, int xA, int yA, DisplayMode tracker, QwtPlotCanvas* can)
  : QwtPlotPicker(xA, yA, QwtPicker::UserRubberBand, tracker, can), m_PlotCurves(curves), m_RubberBandPens(), m_IsGrayscaleActivated(false)
{
  assert(m_PlotCurves.size() == HistogramPlotPicker::CURVE_COUNT);

  setStateMachine(new QwtPickerDragPointMachine());
}

/*******************************************************************************/
HistogramPlotPicker::~HistogramPlotPicker()
{
}

/*******************************************************************************/
void HistogramPlotPicker::SetRubberBandPen(RgbwChannel channel, const QPen& pen)
{
  CountType start = 0;
  CountType stop  = 0;

  if (!RgbwBounds(start, stop, channel))
    return;

  for (CountType i = start; i < stop; ++i)
  {
    m_RubberBandPens[i] = pen;
  }
}

/*******************************************************************************/
void HistogramPlotPicker::SetGrayscaleActivated(bool isGrayscale)
{
  m_IsGrayscaleActivated = isGrayscale;
}

/*******************************************************************************/
void HistogramPlotPicker::drawRubberBand(QPainter* painter) const
{
  assert(painter != NULL);

  if (rubberBand() != QwtPicker::UserRubberBand)
  {
    QwtPlotPicker::drawRubberBand(painter);
    return;
  }

  if (!isActive() || rubberBand() == QwtPicker::NoRubberBand || rubberBandPen().style() == Qt::NoPen)
    return;

  const QPolygon& pa = selection();

  if (rubberBand() == QwtPicker::UserRubberBand &&
      // ( selectionType()==QwtPicker::PointSelection ) &&
      selection().count() >= 1)
  {
#if QWT_IS_ABOVE_6_1
    assert(parentWidget() != nullptr);

    const QRect& rect = parentWidget()->contentsRect();

#else // QWT_IS_ABOVE_6_1
    const QRect& rect = pickRect();

#endif // QWT_IS_ABOVE_6_1

    const QPoint& pos = pa[0];

    QwtPainter::drawLine(painter, pos.x(), rect.bottom(), pos.x(), rect.top());

    CountType start = 0;
    CountType stop  = 0;

    if (!RgbwBounds(start, stop, RGBW_CHANNEL_ALL))
      return;

    QPointF p(invTransform(pos));

    for (CountType i = start; i < stop; ++i)
      if (m_PlotCurves[i]->isVisible())
      {
        // RgbwChannel channel = static_cast< RgbwChannel >( i );

        p.setY(Find(m_PlotCurves[i], p.x()));
        QPoint pos2(transform(p));

        /*
          QPen pen( m_RubberBandPens[ i ] );
          painter->setPen( rubberBandPen() );
        */

        QwtPainter::drawLine(painter, rect.left(), pos2.y(), rect.right(), pos2.y());

        // painter->setPen( pen );
      }
  }
}

/*******************************************************************************/
QwtText HistogramPlotPicker::trackerTextF(const QPointF& point) const
{
  if (rubberBand() != QwtPicker::UserRubberBand)
    return QwtPlotPicker::trackerTextF(point);

  if (!isActive())
    return QwtText(QString().sprintf("%.4f, %.4f", point.x(), point.y()));

  QString text;
  text.sprintf("%.4f", point.x());

  CountType start = 0;
  CountType stop  = 0;

  if (RgbwBounds(start, stop, RGBW_CHANNEL_ALL))
    for (CountType i = start; i < stop; ++i)
      if (m_PlotCurves[i]->isVisible())
      {
        double c0 = 0.0;
        double c1 = 0.0;
        double cf = 0.0;

        Find(m_PlotCurves[i], point.x(), c0, c1, cf);

        text.append(QString().sprintf("\n[%.4f; %.4f[, %.0f", c0, c1, cf));
      }

  return text;
}

/*******************************************************************************/
double HistogramPlotPicker::Find(const QwtPlotCurve* curve, double x) const
{
  typedef QwtSeriesData<QPointF> SeriesData;

  const SeriesData* data = curve->data();

  assert(data != nullptr);

  /*
  #if HISTOGRAM_CURVE_TYPE==0
    assert( false && "Not yet implemented!" );

  #elif HISTOGRAM_CURVE_TYPE==1
    assert( false && "Not yet implemented!" );

  #elif HISTOGRAM_CURVE_TYPE==2
  */

  assert(data->size() % 4 == 0);

  CountType steps = 0;

  if (data->size() == 0)
    return -1.0;

  CountType i0 = 0;
  CountType i1 = data->size() / 4 - 1;

  // assert( x>=data->sample( 4 * i0 ).x() && x<=data->sample( 4 * i1 + 3 ).x() );
  if (x < data->sample(4 * i0).x() || x > data->sample(4 * i1 + 3).x())
    return -1.0;

  while (i0 != i1)
  {
    assert(data->sample(4 * i0).x() == data->sample(4 * i0 + 1).x());
    assert(data->sample(4 * i0 + 2).x() == data->sample(4 * i0 + 3).x());
    assert(data->sample(4 * i0 + 1).y() == data->sample(4 * i0 + 2).y());
    assert(data->sample(4 * i0).y() == data->sample(4 * i0 + 3).y());

    assert(data->sample(4 * i1).x() == data->sample(4 * i1 + 1).x());
    assert(data->sample(4 * i1 + 2).x() == data->sample(4 * i1 + 3).x());
    assert(data->sample(4 * i1 + 1).y() == data->sample(4 * i1 + 2).y());
    assert(data->sample(4 * i1).y() == data->sample(4 * i1 + 3).y());

    CountType i = (i0 + i1 + 1) / 2;

#if 0
    qDebug()
      << i0
      << " (" << data->sample( 4*i0 ).x()
      << ", " << data->sample( 4*i0+2 ).x()
      << ") "
      << i
      << " (" << data->sample( 4*i ).x()
      << ", " << data->sample( 4*i+2 ).x()
      << ") "
      << i1
      << " (" << data->sample( 4*i1 ).x()
      << ", " << data->sample( 4*i1+2 ).x()
      << ")";
#endif

    if (x < data->sample(4 * i).x())
      i1 = i - 1;
    else
      i0 = i;

    ++steps;
  }

  assert(x >= data->sample(4 * i0).x() && x <= data->sample(4 * i0 + 2).x());
  assert(x >= data->sample(4 * i0 + 1).x() && x <= data->sample(4 * i0 + 3).x());

#if 0
  qDebug()
    << steps << ":"
    << x << "in [" << data->sample( 4 * i0 ).x() << "; " << data->sample( 4 * i0 + 2 ).x() << "] ->"
    << data->sample( 4 * i0 + 1 ).y();
#endif

  return data->sample(4 * i0 + 1).y();

  /*
  #else
    assert( false && "Unknown HISTOGRAM_CURVE_TYPE value" );

  #endif
  */
}

/*******************************************************************************/
CountType HistogramPlotPicker::Find(const QwtPlotCurve* curve, double x, double& xmin, double& xmax, double& y) const
{
  typedef QwtSeriesData<QPointF> SeriesData;

  const SeriesData* data = curve->data();

  assert(data != nullptr);

  /*
  #if HISTOGRAM_CURVE_TYPE==0
    assert( false && "Not yet implemented!" );

  #elif HISTOGRAM_CURVE_TYPE==1
    assert( false && "Not yet implemented!" );

  #elif HISTOGRAM_CURVE_TYPE==2
  */

  assert(data->size() % 4 == 0);

  CountType steps = 0;

  if (data->size() == 0)
    return 0;

  CountType i0 = 0;
  CountType i1 = data->size() / 4 - 1;

  if (x < data->sample(4 * i0).x() || x > data->sample(4 * i1 + 3).x())
    return 0;

  while (i0 != i1)
  {
    assert(data->sample(4 * i0).x() == data->sample(4 * i0 + 1).x());
    assert(data->sample(4 * i0 + 2).x() == data->sample(4 * i0 + 3).x());
    assert(data->sample(4 * i0 + 1).y() == data->sample(4 * i0 + 2).y());
    assert(data->sample(4 * i0).y() == data->sample(4 * i0 + 3).y());

    assert(data->sample(4 * i1).x() == data->sample(4 * i1 + 1).x());
    assert(data->sample(4 * i1 + 2).x() == data->sample(4 * i1 + 3).x());
    assert(data->sample(4 * i1 + 1).y() == data->sample(4 * i1 + 2).y());
    assert(data->sample(4 * i1).y() == data->sample(4 * i1 + 3).y());

    CountType i = (i0 + i1 + 1) / 2;

#if 0
    qDebug()
      << i0
      << " (" << data->sample( 4*i0 ).x()
      << ", " << data->sample( 4*i0+2 ).x()
      << ") "
      << i
      << " (" << data->sample( 4*i ).x()
      << ", " << data->sample( 4*i+2 ).x()
      << ") "
      << i1
      << " (" << data->sample( 4*i1 ).x()
      << ", " << data->sample( 4*i1+2 ).x()
      << ")";
#endif

    if (x < data->sample(4 * i).x())
      i1 = i - 1;
    else
      i0 = i;

    ++steps;
  }

  assert(x >= data->sample(4 * i0).x() && x <= data->sample(4 * i0 + 2).x());
  assert(x >= data->sample(4 * i0 + 1).x() && x <= data->sample(4 * i0 + 3).x());

#if 0
  qDebug()
    << steps << ":"
    << x << "in [" << data->sample( 4 * i0 ).x() << "; " << data->sample( 4 * i0 + 2 ).x() << "] ->"
    << data->sample( 4 * i0 + 1 ).y();
#endif

  xmin = data->sample(4 * i0).x();
  xmax = data->sample(4 * i0 + 2).x();
  y    = data->sample(4 * i0 + 1).y();

  return steps;

  /*
  #else
    assert( false && "Unknown HISTOGRAM_CURVE_TYPE value" );

  #endif
  */
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
