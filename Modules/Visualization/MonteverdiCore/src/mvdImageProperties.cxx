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

#include "mvdImageProperties.h"


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

namespace mvd
{
/*
  TRANSLATOR mvd::ImageProperties

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

/*******************************************************************************/
ImageProperties
#if IMAGE_PROPERTIES_IS_QOBJECT
::ImageProperties( QObject* p ) :
  QObject( p ),
#else
::ImageProperties() :
#endif
  m_Flags(),
  m_NoData( ComponentType( 0 ) )
{
  m_Flags.m_NoData = false;
}

/*******************************************************************************/
ImageProperties
::~ImageProperties()
{
}

/*******************************************************************************/
bool
ImageProperties
::operator == ( const ImageProperties & other ) const
{
  return
    other.m_Flags.m_NoData==m_Flags.m_NoData &&
    other.m_NoData==m_NoData;
}

/*******************************************************************************/
bool
ImageProperties
::operator != ( const ImageProperties & other ) const
{
  return !operator == ( other );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
