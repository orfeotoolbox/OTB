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

#ifndef mvdHistogramWidget_h
#define mvdHistogramWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// Qwt includes.

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

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

class QwtPlot;
class QwtPlotGrid;
class QwtPlotCurve;
class QwtPlotMagnifier;
class QwtPlotMarker;
class QwtPlotPanner;
// class QwtPlotZoomer;

namespace mvd
{

class HistogramPlotPicker;

//
// Internal classes pre-declaration.
namespace Ui
{
class HistogramWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramWidget
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT HistogramWidget : public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public constants

  //
  // Public methods.
public:
  /** \brief Constructor. */
  HistogramWidget(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~HistogramWidget() override;

  /**
   * \brief
   */
  void SetBounds(RgbwChannel channel, double xMin, double xMax, double yMin, double yMax);

  /**
   * \brief
   */
  void SetData(RgbwChannel channel, double* const x, double* const y, size_t size, double xMin, double yMin, double xMax, double yMax);

  /**
   */
  void SetLowMarker(RgbwChannel channel, double low);

  /**
   */
  void SetHighMarker(RgbwChannel channel, double high);

  /**
   * \brief
   */
  void SetGrayscaleActivated(bool activated);

  /**
   * \brief
   */
  // bool IsGrayscaleActivated() const;

  /**
   * \brief
   */
  void RefreshScale(bool iqr);

  /**
   */
  void Clear();

  /**
   */
  void SetPrecision(double);

  /**
   */
  double GetPrecision() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:
  /**
   * \brief
   */
  void Replot();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private types.
private:
  struct Bounds
  {
    Bounds(double xMin = 0.0, double xMax = 1.0, double yMin = 0.0, double yMax = 1.0, double qMin = 0.0, double qMax = 1.0)
      : m_XMin(xMin), m_XMax(xMax), m_YMin(yMin), m_YMax(yMax), m_QMin(qMin), m_QMax(qMax)
    {
    }

    double m_XMin;
    double m_XMax;

    double m_YMin;
    double m_YMax;

    double m_QMin;
    double m_QMax;
  };

  //
  // Private methods.
private:
  /**
   */
  void UpdateCurvesVisibility(CountType index);

  //
  // Private attributes.
private:
  /**
   * \brief
   */
  static const CountType CURVE_COUNT = 4;

  /**
   */
  static const char* CURVE_NAMES[HistogramWidget::CURVE_COUNT];

  /**
   */
  static const QColor CURVE_COLORS[HistogramWidget::CURVE_COUNT];

  /**
   */
  static const QColor BAR_COLORS[HistogramWidget::CURVE_COUNT];

  /**
   */
  static const QColor MARKER_COLORS[HistogramWidget::CURVE_COUNT];

  /**
   */
  static const QColor RUBBER_BAND_COLORS[HistogramWidget::CURVE_COUNT];

  /**
   * \brief uic generated.
   */
  Ui::HistogramWidget* m_UI;

  /**
   */
  QwtPlotGrid* m_PlotGrid;

  /**
   */
  HistogramPlotPicker* m_PlotPicker;

  /**
   */
  // QwtPlotZoomer* m_PlotZoomer;

  /**
   */
  QwtPlotMagnifier* m_PlotMagnifier;

  /**
   */
  QwtPlotPanner* m_PlotPanner;

  /**
   * \brief
   */
  QwtPlotCurve* m_PlotCurves[CURVE_COUNT];

  /**
   */
  QwtPlotMarker* m_LowPlotMarkers[CURVE_COUNT];

  /**
   */
  QwtPlotMarker* m_HighPlotMarkers[CURVE_COUNT];

  /**
   */
  Bounds m_Bounds[CURVE_COUNT];

  /**
   */
  double m_Precision;

  /**
   */
  bool m_IsGrayscaleActivated;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
  /**
   */
  void on_zoom1Button_clicked();

  /**
   */
  void on_zoomQButton_clicked();

  /**
   */
  void on_channelComboBox_currentIndexChanged(int index);

  /**
   */
  void OnAppended(const QPointF&);
  void OnAppended(const QPoint&);

  void OnChanged(const QPolygon&);

  void OnMoved(const QPointF&);
  void OnMoved(const QPoint&);

  void OnSelected(const QPointF&);
  void OnSelected(const QRectF&);
  void OnSelected(const QPolygon&);
  void OnSelected(const QVector<QPointF>&);
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'

#endif // mvdHistogramWidget_h
