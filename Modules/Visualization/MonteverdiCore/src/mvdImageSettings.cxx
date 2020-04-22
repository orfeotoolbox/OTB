/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


char const* const ImageSettings::EFFECT_VALUE_NAME[EFFECT_COUNT] = {
    NULL, NULL, QT_TRANSLATE_NOOP("mvd::ImageSettings", "Range"), NULL, NULL, QT_TRANSLATE_NOOP("mvd::ImageSettings", "Angle"), NULL, NULL,
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
ImageSettings::ImageSettings() : m_IsModified(false), m_IsApplied(false), m_Effect(EFFECT_NORMAL), m_Size(256), m_Range(50.0), m_Angle(10.0), m_Alpha(1.0)
{
}

/*****************************************************************************/
ImageSettings::~ImageSettings()
{
}

/*****************************************************************************/
const char* ImageSettings::GetEffectName() const
{
  assert(m_Effect >= 0 && m_Effect < EFFECT_COUNT);
  assert(qApp != NULL);

  return qApp->translate("mvd", EFFECT_NAMES[m_Effect]).toLatin1().constData();
}

/*****************************************************************************/
double ImageSettings::GetValue() const
{
  switch (m_Effect)
  {
  case EFFECT_LOCAL_CONTRAST:
    // qDebug() << "Range:" << m_Range;
    return m_Range;
    break;

  case EFFECT_SPECTRAL_ANGLE:
    // qDebug() << "Angle:" << m_Angle;
    return m_Angle;
    break;

  case EFFECT_LUT_LOCAL_JET:
    // qDebug() << "Range:" << m_Range;
    return m_Range;
    break;

  case EFFECT_LUT_LOCAL_HOT:
    // qDebug() << "Range:" << m_Range;
    return m_Range;
    break;

  case EFFECT_LUT_LOCAL_WINTER:
    // qDebug() << "Range:" << m_Range;
    return m_Range;
    break;

  case EFFECT_LUT_LOCAL_SUMMER:
    // qDebug() << "Range:" << m_Range;
    return m_Range;
    break;

  case EFFECT_LUT_LOCAL_COOL:
    // qDebug() << "Range:" << m_Range;
    return m_Range;
    break;

  default:
    break;
  }

  return std::numeric_limits<double>::signaling_NaN();
}

/*****************************************************************************/
void ImageSettings::SetValue(double value)
{
  switch (m_Effect)
  {
  case EFFECT_LOCAL_CONTRAST:
    // qDebug() << "Range = " << value;
    m_Range = value;
    break;

  case EFFECT_SPECTRAL_ANGLE:
    // qDebug() << "Angle = " << value;
    m_Angle = value;
    break;

  case EFFECT_LUT_LOCAL_JET:
    // qDebug() << "Range = " << value;
    m_Range = value;
    break;

  case EFFECT_LUT_LOCAL_HOT:
    // qDebug() << "Range = " << value;
    m_Range = value;
    break;

  case EFFECT_LUT_LOCAL_WINTER:
    // qDebug() << "Range = " << value;
    m_Range = value;
    break;

  case EFFECT_LUT_LOCAL_SUMMER:
    // qDebug() << "Range = " << value;
    m_Range = value;
    break;

  case EFFECT_LUT_LOCAL_COOL:
    // qDebug() << "Range = " << value;
    m_Range = value;
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
