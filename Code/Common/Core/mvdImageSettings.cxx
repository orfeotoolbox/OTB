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
#include <cassert>
#include <limits>

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

char const * const
ImageSettings
::EFFECT_VALUE_NAME[ ImageSettings::EFFECT_COUNT ] =
{
  NULL,
  NULL,
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Range" ),
  NULL,
  NULL,
  QT_TRANSLATE_NOOP( "mvd::VectorImageSettings", "Angle" ),
  NULL,
  NULL,
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
ImageSettings
::ImageSettings() :
  m_IsModified( false ),
  m_IsApplied( false ),
  m_Effect( EFFECT_NORMAL ),
  m_Size( 256 ),
  m_Range( 50.0 ),
  m_Angle( 10.0 ),
  m_Alpha( 1.0 )
{
}

/*****************************************************************************/
ImageSettings
::~ImageSettings()
{
}

/*****************************************************************************/
void
ImageSettings
::SetValue( double value )
{
  switch( m_Effect )
    {
    case EFFECT_LOCAL_CONTRAST:
      // qDebug() << "Range = " << value;
      m_Range = value;
      break;

    case EFFECT_SPECTRAL_ANGLE :
      // qDebug() << "Angle = " << value;
      m_Angle = value;
      break;

    default:
      assert( false && "No value for this shader effect." );
      break;
    }

  SetModified();
}

/*****************************************************************************/
double
ImageSettings
::GetValue() const
{
  switch( m_Effect )
    {
    case EFFECT_LOCAL_CONTRAST:
      // qDebug() << "Range:" << m_Range;
      return m_Range;
      break;

    case EFFECT_SPECTRAL_ANGLE :
      // qDebug() << "Angle:" << m_Angle;
      return m_Angle;
      break;

    default:
      return std::numeric_limits< double >::signaling_NaN();
      break;
    }

  return std::numeric_limits< double >::signaling_NaN();
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
