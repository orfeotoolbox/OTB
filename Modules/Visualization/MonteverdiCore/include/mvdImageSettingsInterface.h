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

#ifndef mvdImageSettingsInterface_h
#define mvdImageSettingsInterface_h

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
class ImageSettings;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImageSettingsInterface
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
class OTBMonteverdiCore_EXPORT ImageSettingsInterface
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Destructor. */
  virtual ~ImageSettingsInterface();

  /** */
  inline const ImageSettings* GetSettings() const;

  /** */
  inline ImageSettings* GetSettings();

  /** */
  inline bool HasSettings() const;

/** */
#if 0
  inline
    template< typename T >
    const T * GetSettings() const;
#endif

  /**
   */
  void UpdateSettings();

  /** */
  void SetSettings(ImageSettings*);


  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  /** \brief Constructor. */
  ImageSettingsInterface();

  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private methods.
private:
  /**
   */
  virtual void virtual_UpdateSettings();
  /**
   */
  virtual void virtual_SetSettings(ImageSettings*);

  //
  // Private attributes.
private:
  /** */
  ImageSettings* m_Settings;
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
inline const ImageSettings* ImageSettingsInterface::GetSettings() const
{
  return m_Settings;
}

/*****************************************************************************/
inline ImageSettings* ImageSettingsInterface::GetSettings()
{
  return m_Settings;
}

/*****************************************************************************/
#if 0

template< typename T >
inline
const ImageSettings *
ImageSettingsInterface
::GetSettings() const
{
  return dynamic_cast< T >( m_Settings );
}

#endif

/*****************************************************************************/
inline bool ImageSettingsInterface::HasSettings() const
{
  return m_Settings != 0;
}

} // end namespace 'mvd'

#endif // mvdImageSettingsInterface_h
