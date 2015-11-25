/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdGui_h
#define __mvdGui_h

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
#include "Core/mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

//
// Internal classes pre-declaration.
namespace mvd
{
} // end namespace 'mvd'


/*****************************************************************************/
/* CONSTANTS SECTION                                                         */

/**
 */
namespace mvd
{

/**
 */
extern const int MOUSE_WHEEL_STEP_DEGREES;

/**
 */
extern const int MOUSE_WHEEL_STEP_FACTOR;

} // end of namespace 'mvd'.

/*****************************************************************************/
/* TYPE DEFINITION SECTION                                                   */

namespace mvd
{

/**
 */
enum Resolution
{
  RESOLUTION_NONE = -1,
  //
  RESOLUTION_NEAREST = 0,
  RESOLUTION_LOWER,
  RESOLUTION_UPPER,
  //
  RESOLUTION_COUNT
};

/**
 * Constant naming effect values.
 */
Monteverdi_EXPORT
extern
char const * const
RESOLUTION_NAME[ RESOLUTION_COUNT ];

/**
 */
enum TileSize
{
  TILE_SIZE_NONE = -1,
  //
  TILE_SIZE_64 = 0,
  TILE_SIZE_128,
  TILE_SIZE_256,
  TILE_SIZE_512,
  //
  TILE_SIZE_COUNT,
  //
  TILE_SIZE_DEFAULT = TILE_SIZE_256,
};

/**
 * Constant naming effect values.
 */
Monteverdi_EXPORT
extern
char const * const
TILE_SIZE_NAME[ TILE_SIZE_COUNT ];

/**
 */
Monteverdi_EXPORT
extern
int
TILE_SIZE_VALUE[ TILE_SIZE_COUNT ];

} // End of namespace 'mvd'.


/*****************************************************************************/
/* COMMON DECLARATION SECTION                                                */

namespace mvd
{

/**
 */
QMessageBox & CustomizeMessageBox( QMessageBox & messageBox,
                                   QWidget * widget );

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'.

#endif // __mvdGui_h
