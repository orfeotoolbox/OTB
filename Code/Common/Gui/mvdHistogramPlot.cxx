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
#include "mvdHistogramPlot.h"


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

namespace mvd
{
/*
  TRANSLATOR mvd::HistogramPlot

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramPlot
::HistogramPlot( const QwtText& title ) :
  QwtPlotItem( title ),
  m_XMin( NULL, NULL,  0 ),
  m_XMax( NULL, NULL, 0 ),
  m_Y( NULL, NULL, 0 )
{
}

/*******************************************************************************/
HistogramPlot
::~HistogramPlot()
{
}

/*******************************************************************************/
void
HistogramPlot
::SetData( double * const xMin,
	   double * const xMax,
	   double * const y,
	   size_t size )
{
  m_XMin = QwtCPointerData( xMin, NULL, 2 * size );
  m_XMax = QwtCPointerData( xMax, NULL, 2 * size );
  m_Y = QwtCPointerData( y, NULL, size );
}

/*******************************************************************************/
void
HistogramPlot
::draw( QPainter* painter,
	const QwtScaleMap& xMap,
	const QwtScaleMap& yMap,
	const QRect& canvasRect ) const
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
