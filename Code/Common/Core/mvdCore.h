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

#ifndef __mvdCore_h
#define __mvdCore_h

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
/* TYPE DEFINITION SECTION                                                   */

namespace mvd
{

/**
 * Constants identifying the video-channels.
 */
enum RgbwChannel
{
  RGBW_CHANNEL_NONE = -1,

  RGBW_CHANNEL_RED = 0,
  RGBW_CHANNEL_GREEN,
  RGBW_CHANNEL_BLUE,

  RGBW_CHANNEL_WHITE,

  RGBW_CHANNEL_RGB,

  RGBW_CHANNEL_ALL,

  RGBW_CHANNEL_COUNT,
};

/**
 * Constant naming video-channels.
 */
extern
const char*
RGBW_CHANNEL_NAMES[ RGBW_CHANNEL_COUNT ];

} // end of namespace mvd.


/*****************************************************************************/
/* FUNCTION DECLARATION SECTION                                              */

namespace mvd
{

/**
 * \brief Convenience method used to compute integer indices in order
 * to iterate through RGB channels such as, for example:
 * \code
 * for( i=begin; i<end; ++i ) {}
 * \endcode
 *
 * \param begin    The resulting first index where to begin iteration.
 * \param end      The resulting upper-boundary index of the iteration.
 * \param channels The channels to be iterated. Valid values are:
 * _ RGBA_CHANNEL_NONE to select no video-channel at all;
 * - RGBA_CHANNEL_RED to select red video-channel;
 * - RGBA_CHANNEL_GREEN to select green video-channel;
 * - RGBA_CHANNEL_BLUE to select blue video-channel;
 * - RGBA_CHANNEL_WHITE is equivalent to RGBA_CHANNEL_NONE;
 * - RGBA_CHANNEL_RGB to select all RGB video-channels;
 * - RGBA_CHANNEL_ALL to select all RGB (without the alpha) video-channels.
 *
 * \return true if iteration indices have been set and loop can be
 * processed.
 */
bool RgbBounds( CountType& begin, CountType& end, RgbwChannel channels );

/**
 * \brief Convenience method used to compute integer indices in order
 * to iterate through RGBA channels such as, for example:
 * \code
 * for( i=begin; i<end; ++i ) {}
 * \endcode
 *
 * \param begin    The first index where to begin iteration.
 * \param end      The upper-boundary index of the iteration.
 * \param channels The channels to be iterated. Valid values are:
 * _ RGBA_CHANNEL_NONE to select no video-channel at all;
 * - RGBA_CHANNEL_RED to select red video-channel;
 * - RGBA_CHANNEL_GREEN to select green video-channel;
 * - RGBA_CHANNEL_BLUE to select blue video-channel;
 * - RGBA_CHANNEL_WHITE to select the white video-channel;
 * - RGBA_CHANNEL_RGB to select all RGB (without the alpha) video-channels;
 * - RGBA_CHANNEL_ALL to select all RGBA video-channels.
 *
 * \return true if iteration indices have been set and loop can be
 * processed.
 */
bool RgbwBounds( CountType& begin, CountType& end, RgbwChannel channels );

} // end namespace 'mvd'.


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
/*****************************************************************************/
inline
bool
RgbBounds( CountType& begin, CountType& end, RgbwChannel channels )
{
  assert( channels!=RGBW_CHANNEL_COUNT );

  switch( channels )
    {
    case RGBW_CHANNEL_NONE:
    case RGBW_CHANNEL_WHITE:
      begin = RGBW_CHANNEL_NONE;
      end = RGBW_CHANNEL_NONE;
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      begin = channels;
      end = channels + 1;
      break;

    case RGBW_CHANNEL_RGB:
    case RGBW_CHANNEL_ALL:
      begin = RGBW_CHANNEL_RED;
      end = RGBW_CHANNEL_WHITE;
      break;

    default:
      assert( false && "Non-managed RgbwChannel value.!" );
      return false;
      break;
    }

  return true;
}

/*****************************************************************************/
inline
bool
RgbwBounds( CountType& begin, CountType& end, RgbwChannel channels )
{
  assert( channels!=RGBW_CHANNEL_COUNT );

  switch( channels )
    {
    case RGBW_CHANNEL_NONE:
      begin = RGBW_CHANNEL_NONE;
      end = RGBW_CHANNEL_NONE;
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
    case RGBW_CHANNEL_WHITE:
      begin = channels;
      end = channels + 1;
      break;

    case RGBW_CHANNEL_RGB:
      begin = RGBW_CHANNEL_RED;
      end = RGBW_CHANNEL_WHITE;
      break;

    case RGBW_CHANNEL_ALL:
      begin = RGBW_CHANNEL_RED;
      end = RGBW_CHANNEL_RGB;
      break;

    default:
      assert( false && "Non-managed RgbwChannel value.!" );
      return false;
      break;
    }

  return true;
}

} // end namespace 'mvd'.

#endif // __mvdCore_h
