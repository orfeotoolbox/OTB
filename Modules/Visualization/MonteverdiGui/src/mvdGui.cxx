/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdGui.h"

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
  TRANSLATOR mvd::Gui

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

const int MOUSE_WHEEL_STEP_DEGREES = 15;
const int MOUSE_WHEEL_STEP_FACTOR = 8;

char const * const
RESOLUTION_NAMES[ RESOLUTION_COUNT ] =
{
  // QT_TRANSLATE_NOOP( "mvd", "None" ),
  QT_TRANSLATE_NOOP( "mvd", "Nearest" ),
  QT_TRANSLATE_NOOP( "mvd", "Lower bound" ),
  QT_TRANSLATE_NOOP( "mvd", "Upper bound" ),
};

char const * const
TILE_SIZE_NAMES[ TILE_SIZE_COUNT ] =
{
  // QT_TRANSLATE_NOOP( "mvd", "None" ),
  QT_TRANSLATE_NOOP( "mvd", "64" ),
  QT_TRANSLATE_NOOP( "mvd", "128" ),
  QT_TRANSLATE_NOOP( "mvd", "256" ),
  QT_TRANSLATE_NOOP( "mvd", "512" ),
};

int
TILE_SIZE_VALUE[ TILE_SIZE_COUNT ] =
{
  64, 128, 256, 512,
};


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */
/*****************************************************************************/
void
AddItemsInto( QComboBox * comboBox,
	      const char * context,
	      char const * const names[],
	      size_t count )
{
  assert( comboBox!=NULL );
  assert( names!=NULL );

  for( size_t i=0; i<count; ++i )
    comboBox->addItem(
      qApp->translate(
	context,
	names[ i ] )
    );
}

/*****************************************************************************/
QMessageBox &
CustomizeMessageBox( QMessageBox & messageBox,
                     QWidget * widget )
{
  assert( widget!=NULL );

  assert(
    messageBox.layout()==qobject_cast< QGridLayout * >( messageBox.layout() )
  );

  QGridLayout * gridLayout =
    qobject_cast< QGridLayout * >( messageBox.layout() );
  assert( gridLayout!=NULL );

  /*
  qDebug() << layout;
  qDebug() << layout->itemAt( layout->count() - 2 )->widget()->layout();
  qDebug() << layout->itemAt( layout->count() - 1 )->widget()->layout();
  */

  // Add custom widget into message-box layout.
  gridLayout->addWidget( widget, 1, 1 );

  // Return input message-box to chain function calls, if needed.
  return messageBox;
}

/*****************************************************************************/

} // end namespace 'mvd'
