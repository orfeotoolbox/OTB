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

#ifndef mvdImageProperties_h
#define mvdImageProperties_h

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
#include "mvdTypes.h"


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
 * \class ImageProperties
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
#define IMAGE_PROPERTIES_IS_QOBJECT 0
class OTBMonteverdiCore_EXPORT ImageProperties
#if IMAGE_PROPERTIES_IS_QOBJECT
    : public QObject
#endif
{

/*-[ QOBJECT SECTION ]-----------------------------------------------------*/

#if IMAGE_PROPERTIES_IS_QOBJECT
  Q_OBJECT;

  Q_PROPERTY(bool isNoDataEnabled READ IsNoDataEnabled WRITE SetNoDataEnabled);

  Q_PROPERTY(ComponentType NoData READ GetNoData WRITE SetNoData);
#endif

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
/** \brief Constructor. */
#if IMAGE_PROPERTIES_IS_QOBJECT
  ImageProperties(QObject* p = NULL);
#else
  ImageProperties();
#endif

  /** \brief Destructor. */
  virtual ~ImageProperties();

  /**
   */
  inline bool IsModified() const;

  /**
   */
  inline void ClearModified();

  /*
   */
  bool IsNoDataEnabled() const;

  /**
   */
  inline void SetNoDataEnabled(bool enabled);

  /**
   */
  void SetNoData(ComponentType value = ComponentType(0));

  /**
   */
  inline ComponentType GetNoData() const;

  /**
   */
  bool operator==(const ImageProperties&) const;

  /**
   */
  bool operator!=(const ImageProperties&) const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   */
  inline void SetModified();

  //
  // Private attributes.
private:
  //
  // Group bitfield bool flags together.
  struct Flags
  {
    Flags() : m_IsModified(false), m_NoData(false)
    {
    }

  public:
    bool m_IsModified : 1;
    bool m_NoData : 1;
  };

  /**
   */
  Flags m_Flags;

  /**
   */
  ComponentType m_NoData;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
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
inline bool ImageProperties::IsModified() const
{
  return m_Flags.m_IsModified;
}

/*****************************************************************************/
inline void ImageProperties::SetModified()
{
  m_Flags.m_IsModified = true;
}

/*****************************************************************************/
inline void ImageProperties::ClearModified()
{
  m_Flags.m_IsModified = false;
}

/*****************************************************************************/
inline void ImageProperties::SetNoDataEnabled(bool enabled)
{
  m_Flags.m_NoData = enabled;

  SetModified();
}

/*****************************************************************************/
inline void ImageProperties::SetNoData(ComponentType value)
{
  m_NoData = value;

  SetModified();
}

/*****************************************************************************/
inline bool ImageProperties::IsNoDataEnabled() const
{
  return m_Flags.m_NoData;
}

/*****************************************************************************/
inline ComponentType ImageProperties::GetNoData() const
{
  return m_NoData;
}

} // end namespace 'mvd'

#endif // mvdImageProperties_h
