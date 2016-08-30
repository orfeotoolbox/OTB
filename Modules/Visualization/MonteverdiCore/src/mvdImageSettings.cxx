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
::EFFECT_VALUE_NAME[ EFFECT_COUNT ] =
{
  NULL,
  NULL,
  QT_TRANSLATE_NOOP( "mvd::ImageSettings", "Range" ),
  NULL,
  NULL,
  QT_TRANSLATE_NOOP( "mvd::ImageSettings", "Angle" ),
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
const char *
ImageSettings
::GetEffectName() const
{
  assert( m_Effect>=0 && m_Effect<EFFECT_COUNT );
  assert( qApp!=NULL );

  return
    qApp->translate(
      "mvd", EFFECT_NAMES[ m_Effect ]
    )
    .toLocal8Bit().constData();
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
      break;
    }

  return std::numeric_limits< double >::signaling_NaN();
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
      break;
    }

  SetModified();
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
