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

#ifndef mvdImageSettings_h
#define mvdImageSettings_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"

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
#include "mvdCore.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImageSettings
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
class OTBMonteverdiCore_EXPORT ImageSettings
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types & constants.
public:
  /** */
  typedef unsigned int SizeType;

  /** */
  typedef double ValueType;

  //
  // Public methods.
public:
  /** \brief Constructor. */
  ImageSettings();

  /** \brief Destructor. */
  virtual ~ImageSettings();

  /**
   */
  inline bool IsApplied() const;

  /**
   */
  inline bool IsModified() const;

  /**
   */
  inline void ClearModified();

  /**
   */
  inline void SetApplied();

  /**
   */
  inline void SetEffect(Effect);

  /**
   */
  inline Effect GetEffect() const;

  /**
   */
  const char* GetEffectName() const;

  /**
   */
  inline bool HasSize() const;

  /**
   */
  inline bool HasValue() const;

  /**
   */
  inline void SetSize(unsigned int);

  /**
   */
  inline unsigned int GetSize() const;

  /**
   */
  void SetValue(double);

  /**
   */
  double GetValue() const;

  /**
   */
  inline const char* GetValueName() const;

  /**
   */
  inline void SetAlpha(double);

  /**
   */
  inline double GetAlpha() const;

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /**
   */
  inline void SetModified();

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  //
  // Private attributes.
private:
  /**
   */
  static char const* const EFFECT_VALUE_NAME[EFFECT_COUNT];

  /**
   * \brief Flag which notices that rendering settings have been
   * edited.
   */
  // TODO: Optimize using C++ bitset bool foo:1;
  bool m_IsModified : 1;

  /**
   * \brief Flag which notices that rendering settings have been
   * applied to display.
   */
  // TODO: Optimize using C++ bitset bool foo:1;
  bool m_IsApplied : 1;

  /**
   */
  Effect m_Effect;

  /**
   */
  unsigned int m_Size;

  /**
   */
  double m_Range;
  double m_Angle;

  /**
   */
  double m_Alpha;
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

/*****************************************************************************/
inline bool ImageSettings::IsApplied() const
{
  return m_IsApplied;
}

/*****************************************************************************/
inline bool ImageSettings::IsModified() const
{
  return m_IsModified;
}

/*****************************************************************************/
inline void ImageSettings::SetModified()
{
  // qDebug() << this << "::SetModified()";

  m_IsModified = true;
  m_IsApplied  = false;
}

/*****************************************************************************/
inline void ImageSettings::ClearModified()
{
  // qDebug() << this << "::ClearModified()";

  m_IsModified = false;
}

/*****************************************************************************/
inline void ImageSettings::SetApplied()
{
  // qDebug() << this << "::SetApplied()";

  m_IsApplied = true;
}

/*****************************************************************************/
inline void ImageSettings::SetEffect(Effect effect)
{
  m_Effect = effect;

  SetModified();
}

/*****************************************************************************/
inline Effect ImageSettings::GetEffect() const
{
  return m_Effect;
}

/*****************************************************************************/
inline const char* ImageSettings::GetValueName() const
{
  assert(qApp != NULL);

  return qApp->translate("mvd::ImageSettings", ImageSettings::EFFECT_VALUE_NAME[m_Effect]).toLocal8Bit().constData();
}

/*****************************************************************************/
inline bool ImageSettings::HasSize() const
{
  return m_Effect == EFFECT_CHESSBOARD || m_Effect == EFFECT_GRADIENT || m_Effect == EFFECT_LOCAL_CONTRAST || m_Effect == EFFECT_LOCAL_TRANSLUCENCY ||
         m_Effect == EFFECT_SPECTRAL_ANGLE || m_Effect == EFFECT_LUT_LOCAL_JET || m_Effect == EFFECT_LUT_LOCAL_HOT || m_Effect == EFFECT_LUT_LOCAL_WINTER ||
         m_Effect == EFFECT_LUT_LOCAL_SUMMER || m_Effect == EFFECT_LUT_LOCAL_COOL;
}

/*****************************************************************************/
inline bool ImageSettings::HasValue() const
{
  return m_Effect == EFFECT_LOCAL_CONTRAST || m_Effect == EFFECT_SPECTRAL_ANGLE || m_Effect == EFFECT_LUT_LOCAL_JET || m_Effect == EFFECT_LUT_LOCAL_HOT ||
         m_Effect == EFFECT_LUT_LOCAL_WINTER || m_Effect == EFFECT_LUT_LOCAL_SUMMER || m_Effect == EFFECT_LUT_LOCAL_COOL;
}

/*****************************************************************************/
inline void ImageSettings::SetSize(unsigned int size)
{
  m_Size = size;

  SetModified();
}

/*****************************************************************************/
inline unsigned int ImageSettings::GetSize() const
{
  return m_Size;
}

/*****************************************************************************/
inline void ImageSettings::SetAlpha(double alpha)
{
  m_Alpha = alpha;

  if (m_Alpha > 1.0)
    m_Alpha = 1.0;

  if (m_Alpha < 0.0)
    m_Alpha = 0.0;

  SetModified();
}

/*****************************************************************************/
inline double ImageSettings::GetAlpha() const
{
  return m_Alpha;
}

} // end namespace 'mvd'

#endif // mvdImageSettings_h
