/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
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
#include "ConfigureMonteverdi2.h"


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
/* COMMON DECLARATION SECTION                                                */

namespace mvd
{

  /**
   * Constants identifying the video-channels.
   */
  enum RgbaChannel
  {
    RGBA_CHANNEL_NONE = -1,

    RGBA_CHANNEL_RED = 0,
    RGBA_CHANNEL_GREEN = 1,
    RGBA_CHANNEL_BLUE = 2,
    RGBA_CHANNEL_ALPHA = 3,

    RGBA_CHANNEL_COUNT,

    RGBA_CHANNEL_ALL = RGBA_CHANNEL_COUNT
  };


} // end namespace 'mvd'.

#endif // __mvdGui_h
