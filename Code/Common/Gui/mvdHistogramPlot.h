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
#ifndef __mvdHistogramPlot_h
#define __mvdHistogramPlot_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// Qwt includes (sorted by alphabetical order)
#include <qwt_data.h>
#include <qwt_plot_item.h>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class HistogramPlot
 *
 * \brief WIP.
 */
class Monteverdi2_EXPORT HistogramPlot :
    public QwtPlotItem
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  // Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   * \brief
   */
  typedef QwtArray< double > QwtDoubleArray;

//
// Public methods.
public:

  /** \brief Constructor. */
  HistogramPlot( const QwtText& title =QwtText() );

  /** \brief Destructor. */
  virtual ~HistogramPlot();

  /**
   */
  void draw( QPainter* painter,
	     const QwtScaleMap& xMap,
	     const QwtScaleMap& yMap,
	     const QRect& canvasRect ) const;

  /**
   */
  void SetData( double * const xMin,
		double * const xMax,
		double * const y,
		size_t size );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
// public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
// signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:


//
// Private attributes.
private:
  /**
   */
  QwtCPointerData m_XMin;

  /**
   */
  QwtCPointerData m_XMax;

  /**
   */
  QwtCPointerData m_Y;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
// private slots:
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdHistogramPlot_h
