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
#ifndef __mvdHistogramWidget_h
#define __mvdHistogramWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// Qwt includes.
#include <qwt_double_rect.h>
#include <qwt_polygon.h>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdCore.h"

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
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi2_EXPORT HistogramWidget :
    public QWidget
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
  HistogramWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~HistogramWidget();

  /**
   * \brief
   */
  void SetBounds( RgbwChannel channel,
		  double xMin, double xMax,
		  double yMin, double yMax );

  /**
   * \brief 
   */
  void SetData( RgbwChannel channel,
		double * const x,
		double * const y,
		size_t size,
		double xMin, double yMin,
		double xMax, double yMax );

  /**
   */
  void SetLowMarker( RgbwChannel channel, double low );

  /**
   */
  void SetHighMarker( RgbwChannel channel, double high );

  /**
   * \brief
   */
  void SetGrayscaleActivated( bool activated );

  /**
   * \brief
   */
  // bool IsGrayscaleActivated() const;

  /**
   * \brief
   */
  void RefreshScale( bool iqr );

  /**
   */
  void Clear();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  /**
   * \brief
   */
  void Replot();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

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
    Bounds( double xMin =0.0, double xMax =1.0,
	    double yMin =0.0, double yMax =1.0,
	    double qMin =0.0, double qMax =1.0 ) :
      m_XMin( xMin ),
      m_XMax( xMax ),
      m_YMin( yMin ),
      m_YMax( yMax ),
      m_QMin( qMin ),
      m_QMax( qMax )
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
  void foo( CountType index );

//
// Private attributes.
private:
  /**
   * \brief 
   */
  static const CountType CURVE_COUNT = 4;

  /**
   */
  static const char* CURVE_NAMES[ HistogramWidget::CURVE_COUNT ];

  /**
   */
  static const QColor CURVE_COLORS[ HistogramWidget::CURVE_COUNT ];

  /**
   */
  static const QColor BAR_COLORS[ HistogramWidget::CURVE_COUNT ];

  /**
   */
  static const QColor MARKER_COLORS[ HistogramWidget::CURVE_COUNT ];

  /**
   */
  static const QColor RUBBER_BAND_COLORS[ HistogramWidget::CURVE_COUNT ];

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
  QwtPlotCurve* m_PlotCurves[ CURVE_COUNT ];

  /**
   */
  QwtPlotMarker* m_LowPlotMarkers[ CURVE_COUNT ];

  /**
   */
  QwtPlotMarker* m_HighPlotMarkers[ CURVE_COUNT ];

  /**
   */
  Bounds m_Bounds[ CURVE_COUNT ];

  /**
   */
  bool m_IsGrayscaleActivated;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   */
  void on_zoom1Button_clicked();

  /**
   */
  void on_zoomQButton_clicked();

  /**
   */
  void on_channelComboBox_currentIndexChanged( int index );

  /**
   */
  void OnAppended( const QwtDoublePoint& pos );
  void OnAppended( const QPoint& pos );

  void OnChanged( const QwtPolygon& pa );

  void OnMoved( const QwtDoublePoint&pos );
  void OnMoved( const QPoint& pos );

  void OnSelected( const QwtDoublePoint& pos );
  void OnSelected( const QwtDoubleRect& rect );
  void OnSelected( const QwtPolygon& pa );
  void OnSelected( const QwtArray< QwtDoublePoint >& pa );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'

#endif // __mvdHistogramWidget_h
