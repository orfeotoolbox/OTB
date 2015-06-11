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
#ifndef __mvdImageSettings_h
#define __mvdImageSettings_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi2.h"
#endif //tag=QT4-boost-compatibility


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
 * \brief WIP.
 */
class Monteverdi2_EXPORT ImageSettings
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types & constants.
public:

  /** */
  typedef unsigned int SizeType;

  /** */
  typedef double ValueType;

  /**
   */
  enum Effect
  {
    EFFECT_NONE = -1,
    //
    EFFECT_CHESSBOARD = 0,
    EFFECT_GRADIENT,
    EFFECT_LOCAL_CONTRAST,
    EFFECT_LOCAL_TRANSLUCENCY,
    EFFECT_NORMAL,
    EFFECT_SPECTRAL_ANGLE,
    EFFECT_SWIPE_H,
    EFFECT_SWIPE_V,
    //
    EFFECT_COUNT
  };

  /**
   */
  static char const * const  EFFECT_NAME[ EFFECT_COUNT ];

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
  inline void SetEffect( Effect );

  /**
   */
  inline Effect GetEffect() const;

  /**
   */
  inline bool HasSize() const;

  /**
   */
  inline bool HasValue() const;

  /**
   */
  inline void SetSize( unsigned int );

  /**
   */
  inline unsigned int GetSize() const;

  /**
   */
  void SetValue( double );

  /**
   */
  double GetValue() const;

  /**
   */
  inline char const * const GetValueName() const;

  /**
   */
  inline void SetAlpha( double );

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
  static char const * const  EFFECT_VALUE_NAME[ EFFECT_COUNT ];

  /**
   * \brief Flag which notices that rendering settings have been
   * edited.
   */
  // TODO: Optimize using C++ bitset bool foo:1;
  bool m_IsModified: 1;

  /**
   * \brief Flag which notices that rendering settings have been
   * applied to display.
   */
  // TODO: Optimize using C++ bitset bool foo:1;
  bool m_IsApplied: 1;

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
inline
bool
ImageSettings
::IsApplied() const
{
  return m_IsApplied;
}

/*****************************************************************************/
inline
bool
ImageSettings
::IsModified() const
{
  return m_IsModified;
}

/*****************************************************************************/
inline
void
ImageSettings
::SetModified()
{
  // qDebug() << this << "::SetModified()";

  m_IsModified = true;
  m_IsApplied = false;
}

/*****************************************************************************/
inline
void
ImageSettings
::ClearModified()
{
  // qDebug() << this << "::ClearModified()";

  m_IsModified = false;
}

/*****************************************************************************/
inline
void
ImageSettings
::SetApplied()
{
  // qDebug() << this << "::SetApplied()";

  m_IsApplied = true;
}

/*****************************************************************************/
inline
void
ImageSettings
::SetEffect( Effect effect )
{
  m_Effect = effect;

  SetModified();
}

/*****************************************************************************/
inline
ImageSettings::Effect
ImageSettings
::GetEffect() const
{
  return m_Effect;
}

/*****************************************************************************/
inline
char const * const
ImageSettings
::GetValueName() const
{
  return ImageSettings::EFFECT_VALUE_NAME[ m_Effect ];
}

/*****************************************************************************/
inline
bool
ImageSettings
::HasSize() const
{
  return
    m_Effect==EFFECT_CHESSBOARD ||
    m_Effect==EFFECT_LOCAL_CONTRAST ||
    m_Effect==EFFECT_LOCAL_TRANSLUCENCY ||
    m_Effect==EFFECT_SPECTRAL_ANGLE;
}

/*****************************************************************************/
inline
bool
ImageSettings
::HasValue() const
{
  return
    m_Effect==EFFECT_LOCAL_CONTRAST ||
    m_Effect==EFFECT_SPECTRAL_ANGLE;
}

/*****************************************************************************/
inline
void
ImageSettings
::SetSize( unsigned int size )
{
  m_Size = size;

  SetModified();
}

/*****************************************************************************/
inline
unsigned int
ImageSettings
::GetSize() const
{
  return m_Size;
}

/*****************************************************************************/
inline
void
ImageSettings
::SetAlpha( double alpha )
{
  m_Alpha = alpha;

  if( m_Alpha>1.0 )
    m_Alpha = 1.0;

  if( m_Alpha<0.0 )
    m_Alpha = 0.0;

  SetModified();
}

/*****************************************************************************/
inline
double
ImageSettings
::GetAlpha() const
{
  return m_Alpha;
}

} // end namespace 'mvd'

#endif // __mvdImageSettings_h
