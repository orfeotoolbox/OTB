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


#ifndef mvdGui_h
#define mvdGui_h

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
#include "OTBMonteverdiGUIExport.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"


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
  RESOLUTION_COUNT,
  //
  RESOLUTION_DEFAULT = RESOLUTION_NEAREST,
};

/**
 * Constant naming effect values.
 */
OTBMonteverdiGUI_EXPORT extern char const* const RESOLUTION_NAMES[RESOLUTION_COUNT];

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
OTBMonteverdiGUI_EXPORT extern char const* const TILE_SIZE_NAMES[TILE_SIZE_COUNT];

/**
 */
OTBMonteverdiGUI_EXPORT extern int TILE_SIZE_VALUE[TILE_SIZE_COUNT];

/**
 */
enum ZoomType
{
  ZOOM_TYPE_NONE = -1,
  //
  ZOOM_TYPE_EXTENT,
  ZOOM_TYPE_FULL,
  ZOOM_TYPE_LAYER,
  //
  ZOOM_TYPE_COUNT
};

} // End of namespace 'mvd'.


/*****************************************************************************/
/* COMMON DECLARATION SECTION                                                */

namespace mvd
{

/**
 */
OTBMonteverdiGUI_EXPORT void AddItemsInto(QComboBox* comboBox, const char* context, char const* const names[], size_t count);
/**
 */
QMessageBox& CustomizeMessageBox(QMessageBox& messageBox, QWidget* widget);

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'.

#endif // mvdGui_h
