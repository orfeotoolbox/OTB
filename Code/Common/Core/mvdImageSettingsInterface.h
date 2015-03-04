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
#ifndef __mvdImageSettingsInterface_h
#define __mvdImageSettingsInterface_h

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
 * \brief WIP.
 */
class Monteverdi2_EXPORT ImageSettingsInterface
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Destructor. */
  virtual ~ImageSettingsInterface();

  /** */
  inline const ImageSettings * GetSettings() const;

  /** */
  inline ImageSettings * GetSettings();

  /** */
  inline bool HasSettings() const;

  /** */
#if 0
  inline
    template< typename T >
    const T * GetSettings() const;
#endif

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
  /** */
  void SetSettings( ImageSettings * );

  /** */
  virtual void virtual_SetSettings( ImageSettings * );

//
// Private attributes.
private:

  /** */
  ImageSettings * m_Settings;
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
const ImageSettings *
ImageSettingsInterface
::GetSettings() const
{
  return m_Settings;
}

/*****************************************************************************/
inline
ImageSettings *
ImageSettingsInterface
::GetSettings()
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
inline
bool
ImageSettingsInterface
::HasSettings() const
{
  return m_Settings!=0;
}

} // end namespace 'mvd'

#endif // __mvdImageSettingsInterface_h
