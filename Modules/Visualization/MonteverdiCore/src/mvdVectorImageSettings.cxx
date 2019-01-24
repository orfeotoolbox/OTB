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

#include "mvdVectorImageSettings.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTextStream.h"

namespace mvd
{
/*
  TRANSLATOR mvd::VectorImageSettings

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
/*****************************************************************************/
VectorImageSettings
::VectorImageSettings() :
  m_RgbChannels(),
  m_RgbDynamicsParams( 6 ),
  m_IsGrayscaleActivated( false ),
  m_GrayChannel( 0 ),
  m_GrayDynamicsParams( 6 ),
  m_Gamma( 1.0 )
{
  m_RgbDynamicsParams.Fill( 0 );
  m_GrayDynamicsParams.Fill( 0 );
}

/***************************************************************************/
VectorImageSettings
::VectorImageSettings( const VectorImageSettings & other ) :
  ImageSettings(other),
  m_RgbChannels( other.m_RgbChannels ),
  m_RgbDynamicsParams( other.m_RgbDynamicsParams ),
  m_IsGrayscaleActivated( other.m_IsGrayscaleActivated ),
  m_GrayChannel( other.m_GrayChannel ),
  m_GrayDynamicsParams( 6 ),
  m_Gamma( other.m_Gamma )
{
}

/*****************************************************************************/
VectorImageSettings
::~VectorImageSettings()
{
}

/*****************************************************************************/
VectorImageSettings::ChannelVector::value_type
VectorImageSettings
::GetRgbwChannel( RgbwChannel channel ) const
{
  switch( channel )
    {
    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      return m_RgbChannels[ channel ];
      break;

    case RGBW_CHANNEL_WHITE:
      return m_GrayChannel;
      break;

    default:
      throw std::invalid_argument(
        ToStdString(
          QCoreApplication::translate(
            "mvd::VectorImageSettings",
            "Invalid argument: %1 ('%2')"
          )
          .arg( channel )
          .arg( RGBW_CHANNEL_NAMES[ channel ] )
        )
      );
      break;
    }
}

/*****************************************************************************/
void
VectorImageSettings
::SetLowIntensity( RgbwChannel channel, ParametersType::ValueType intensity )
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      SetRgbDynamicsParam( 2 * RGBW_CHANNEL_RED, intensity );
      SetRgbDynamicsParam( 2 * RGBW_CHANNEL_GREEN, intensity );
      SetRgbDynamicsParam( 2 * RGBW_CHANNEL_BLUE, intensity );
      break;

    case RGBW_CHANNEL_WHITE:
      SetGrayDynamicsParam( 0, intensity );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      SetRgbDynamicsParam( 2 * channel, intensity );
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  QCoreApplication::translate(
            "mvd::VectorImageSettings",
            "Invalid argument: %1 ('%2')"
          )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

/*****************************************************************************/
ParametersType::ValueType
VectorImageSettings
::GetLowIntensity( RgbwChannel channel ) const
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      {
      ParametersType::ValueType r =
	GetRgbDynamicsParam( 2 * RGBW_CHANNEL_RED );

      ParametersType::ValueType g =
	GetRgbDynamicsParam( 2 * RGBW_CHANNEL_GREEN );

      ParametersType::ValueType b =
	GetRgbDynamicsParam( 2 * RGBW_CHANNEL_BLUE );

      return (r + g + b) / 3;
      }
      break;

    case RGBW_CHANNEL_WHITE:
      return GetGrayDynamicsParam( false );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      return GetRgbDynamicsParam( 2 * channel );
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  QCoreApplication::translate(
            "mvd::VectorImageSettings",
            "Invalid argument: %1 ('%2')"
          )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

/*****************************************************************************/
void
VectorImageSettings
::SetHighIntensity( RgbwChannel channel, ParametersType::ValueType intensity )
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      SetRgbDynamicsParam( 1 + 2 * RGBW_CHANNEL_RED, intensity );
      SetRgbDynamicsParam( 1 + 2 * RGBW_CHANNEL_GREEN, intensity );
      SetRgbDynamicsParam( 1 + 2 * RGBW_CHANNEL_BLUE, intensity );
      break;

    case RGBW_CHANNEL_WHITE:
      SetGrayDynamicsParam( 1, intensity );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      SetRgbDynamicsParam( 1 + 2 * channel, intensity );
      break;

    default:
      assert( false );
      break;
    }
}

/*****************************************************************************/
ParametersType::ValueType
VectorImageSettings
::GetHighIntensity( RgbwChannel channel ) const
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      {
      ParametersType::ValueType r =
	GetRgbDynamicsParam( 1+ 2*RGBW_CHANNEL_RED );

      ParametersType::ValueType g =
	GetRgbDynamicsParam( 1+ 2*RGBW_CHANNEL_GREEN );

      ParametersType::ValueType b =
	GetRgbDynamicsParam( 1+ 2*RGBW_CHANNEL_BLUE );

      return (r + g + b) / 3;
      }
      break;

    case RGBW_CHANNEL_WHITE:
      return GetGrayDynamicsParam( 1 );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      return GetRgbDynamicsParam( 1 + 2 * channel );
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  QCoreApplication::translate(
            "mvd::VectorImageSettings",
            "Invalid argument: %1 ('%2')"
          )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

} // end namespace 'mvd'
