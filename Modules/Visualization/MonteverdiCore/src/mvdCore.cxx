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

#include "mvdCore.h"

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
  TRANSLATOR mvd::Core

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

ITK_EXPORT const char*
RGBW_CHANNEL_NAMES[ RGBW_CHANNEL_COUNT ] = {
  "RGBW_CHANNEL_RED",
  "RGBW_CHANNEL_GREEN",
  "RGBW_CHANNEL_BLUE",
  "RGBW_CHANNEL_WHITE",
  "RGBW_CHANNEL_RGB",
  "RGBW_CHANNEL_ALL"
};

const char*
BOUND_NAMES[ BOUND_COUNT ] = {
  "BOUND_LOWER",
  "BOUND_UPPER"
};

char const * const
EFFECT_NAMES[ EFFECT_COUNT ] =
{
  // QT_TRANSLATE_NOOP( "mvd", "None" ),
  QT_TRANSLATE_NOOP( "mvd", "Chessboard" ),
  QT_TRANSLATE_NOOP( "mvd", "Gradient" ),
  QT_TRANSLATE_NOOP( "mvd", "Local contrast" ),
  QT_TRANSLATE_NOOP( "mvd", "Local translucency" ),
  QT_TRANSLATE_NOOP( "mvd", "Normal" ),
  QT_TRANSLATE_NOOP( "mvd", "Spectral angle" ),
  QT_TRANSLATE_NOOP( "mvd", "Swipe (horizontal)" ),
  QT_TRANSLATE_NOOP( "mvd", "Swipe (vertical)" ),
};

bool
OVERVIEWS_ENABLED_DEFAULT = true;

int
OVERVIEWS_SIZE_DEFAULT = 512;


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*******************************************************************************/

} // end namespace 'mvd'
