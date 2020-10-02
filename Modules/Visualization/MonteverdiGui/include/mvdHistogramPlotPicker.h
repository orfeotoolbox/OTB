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

#ifndef mvdHistogramPlotPicker_h
#define mvdHistogramPlotPicker_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
//
#include <qwt_plot_picker.h>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
#include "mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

class QwtPlotCanvas;
class QwtPlotCurve;

namespace mvd
{

//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramPlotPicker
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT HistogramPlotPicker : public QwtPlotPicker
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types and constants.
public:
  /**
   * \brief
   */
  typedef std::vector<QwtPlotCurve*> PlotCurveVector;

  //
  // Public methods.
public:
  /** \brief Constructor. */
  HistogramPlotPicker(const PlotCurveVector& curves, QwtPlotCanvas* canvas);

  /** \brief Constructor. */
  HistogramPlotPicker(const PlotCurveVector& curves, int xAxis, int yAxis, QwtPlotCanvas* canvas);

  /** \brief Constructor. */
  HistogramPlotPicker(const PlotCurveVector& curves, int xAxis, int yAxis, DisplayMode trackerMode, QwtPlotCanvas* canvas);

  /** \brief Destructor. */
  ~HistogramPlotPicker() override;

  /**
   */
  void SetRubberBandPen(RgbwChannel channel, const QPen& pen);

  /**
   */
  void SetGrayscaleActivated(bool isGrayscale);

  //
  // QwtPlotPicker methods.

  void drawRubberBand(QPainter* painter) const override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // QwtPlotPicker methods.

  using QwtPlotPicker::trackerText;

  QwtText trackerTextF(const QPointF&) const override;

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   */
  double Find(const QwtPlotCurve* curve, double x) const;

  /**
   */
  CountType Find(const QwtPlotCurve* curve, double x, double& xmin, double& xmax, double& y) const;

  //
  // Private attributes.
private:
  /**
   * \brief
   */
  static const CountType CURVE_COUNT = 4;

  /**
   * \brief
   */
  PlotCurveVector m_PlotCurves;

  /**
   */
  QPen m_RubberBandPens[CURVE_COUNT];

  /**
   */
  bool m_IsGrayscaleActivated : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'

#endif // mvdHistogramPlotPicker_h
