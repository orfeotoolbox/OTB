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
#include "Gui/mvdHistogramController.h"


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
#include "Gui/mvdHistogramWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::HistogramController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramController
::HistogramController( HistogramWidget* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
HistogramController
::~HistogramController()
{
}

/*******************************************************************************/
void
HistogramController
::Connect( AbstractModel* model )
{
  HistogramWidget* widget = GetWidget< HistogramWidget >();

  //
  // Connect GUI to controller.

  //
  // Connect controller to model.
}

/*******************************************************************************/
void
HistogramController
::Disconnect( AbstractModel* model )
{
  HistogramWidget* widget = GetWidget< HistogramWidget >();

  //
  // Disconnect controller to model.

  //
  // Disconnect GUI from controller.
}

/*******************************************************************************/
void
HistogramController
::ResetWidget()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
