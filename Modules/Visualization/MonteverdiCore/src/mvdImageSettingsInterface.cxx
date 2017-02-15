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
#include "mvdImageSettingsInterface.h"


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
#include "mvdImageSettings.h"


namespace mvd
{
/*
  TRANSLATOR mvd::ImageSettingsInterface

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
ImageSettingsInterface
::ImageSettingsInterface() :
  m_Settings( NULL )
{
}

/*****************************************************************************/
ImageSettingsInterface
::~ImageSettingsInterface()
{
#if 0
  delete m_Settings;
  m_Settings = NULL;
#endif
}

/*****************************************************************************/
void
ImageSettingsInterface
::SetSettings( ImageSettings * settings )
{
#if 0
  delete m_Settings;
#endif

  m_Settings = settings;

  virtual_SetSettings( settings );
}

/*****************************************************************************/
void
ImageSettingsInterface
::UpdateSettings()
{
  virtual_UpdateSettings();
}

/*****************************************************************************/
void
ImageSettingsInterface
::virtual_SetSettings( ImageSettings * )
{
}

/*****************************************************************************/
void
ImageSettingsInterface
::virtual_UpdateSettings()
{
  virtual_SetSettings( m_Settings );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
