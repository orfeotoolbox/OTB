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

ITK_EXPORT const char* RGBW_CHANNEL_NAMES[RGBW_CHANNEL_COUNT] = {"RGBW_CHANNEL_RED",   "RGBW_CHANNEL_GREEN", "RGBW_CHANNEL_BLUE",
                                                                 "RGBW_CHANNEL_WHITE", "RGBW_CHANNEL_RGB",   "RGBW_CHANNEL_ALL"};

const char* BOUND_NAMES[BOUND_COUNT] = {"BOUND_LOWER", "BOUND_UPPER"};

char const* const EFFECT_NAMES[EFFECT_COUNT] = {
    // QT_TRANSLATE_NOOP( "mvd", "None" ),
    QT_TRANSLATE_NOOP("mvd", "Chessboard"),
    QT_TRANSLATE_NOOP("mvd", "Gradient"),
    QT_TRANSLATE_NOOP("mvd", "Local contrast"),
    QT_TRANSLATE_NOOP("mvd", "Local translucency"),
    QT_TRANSLATE_NOOP("mvd", "Normal"),
    QT_TRANSLATE_NOOP("mvd", "Swipe (horizontal)"),
    QT_TRANSLATE_NOOP("mvd", "Swipe (vertical)"),
    QT_TRANSLATE_NOOP("mvd", "Spectral angle"),
    QT_TRANSLATE_NOOP("mvd", "Lookup table (jet)"),
    QT_TRANSLATE_NOOP("mvd", "Local lookup table (jet)"),
    QT_TRANSLATE_NOOP("mvd", "Lookup table (hot)"),
    QT_TRANSLATE_NOOP("mvd", "Local lookup table (hot)"),
    QT_TRANSLATE_NOOP("mvd", "Lookup table (winter)"),
    QT_TRANSLATE_NOOP("mvd", "Local lookup table (winter)"),
    QT_TRANSLATE_NOOP("mvd", "Lookup table (summer)"),
    QT_TRANSLATE_NOOP("mvd", "Local lookup table (summer)"),
    QT_TRANSLATE_NOOP("mvd", "Lookup table (cool)"),
    QT_TRANSLATE_NOOP("mvd", "Local lookup table (cool)")};

bool OVERVIEWS_ENABLED_DEFAULT = true;

int OVERVIEWS_SIZE_DEFAULT = 4096;


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*******************************************************************************/

} // end namespace 'mvd'
