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
ImageSettingsInterface::ImageSettingsInterface() : m_Settings(NULL)
{
}

/*****************************************************************************/
ImageSettingsInterface::~ImageSettingsInterface()
{
#if 0
  delete m_Settings;
  m_Settings = NULL;
#endif
}

/*****************************************************************************/
void ImageSettingsInterface::SetSettings(ImageSettings* settings)
{
#if 0
  delete m_Settings;
#endif

  m_Settings = settings;

  virtual_SetSettings(settings);
}

/*****************************************************************************/
void ImageSettingsInterface::UpdateSettings()
{
  virtual_UpdateSettings();
}

/*****************************************************************************/
void ImageSettingsInterface::virtual_SetSettings(ImageSettings*)
{
}

/*****************************************************************************/
void ImageSettingsInterface::virtual_UpdateSettings()
{
  virtual_SetSettings(m_Settings);
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
