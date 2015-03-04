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
#include "mvdImageSettings.h"


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
  TRANSLATOR mvd::ImageSettings

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


char const * const
ImageSettings
::EFFECT_NAME[ ImageSettings::EFFECT_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Chessboard" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Gradient" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Local contrast" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Local translucency" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Normal" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Spectral angle" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Swipe (horizontal)" ),
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Swipe (vertical)" ),
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ImageSettings
::ImageSettings() :
  m_IsModified( false ),
  m_IsApplied( false )
{
}

/*******************************************************************************/
ImageSettings
::~ImageSettings()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
