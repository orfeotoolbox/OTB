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
#ifndef __mvdAlgorithm_h
#define __mvdAlgorithm_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

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

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

//
// Functions declaration.

/**
 * Convert a StringVector object to a QStringList object.
 *
 * Because Qt uses a shallow mecanism to copy containers, the returned
 * QStringList is shallow-copied.
 */
inline
QStringList ToQStringList( const StringVector& sv );

/**
 * Append the content of a StringVector object to the content of a
 * QStringList object.
 *
 * Because Qt uses a shallow mecanism to copy containers, the returned
 * QStringList is shallow-copied.
 */
inline
QStringList AppendToQStringList( QStringList& qsl,
				 const StringVector& sv );

//
// Inlined-functions implementations.
//

/*******************************************************************************/
inline
QStringList
ToQStringList( const StringVector& sv )
{
  QStringList qsl;
  return AppendToQStringList( qsl, sv );
}

/*******************************************************************************/
inline
QStringList
AppendToQStringList( QStringList& qsl, const StringVector& sv )
{
  for( StringVector::const_iterator it( sv.begin() );
       it!=sv.end();
       ++ it )
    {
    qsl.append( QString::fromStdString( *it  ) );
    }

  return qsl;
}

/*******************************************************************************/

} // end namespace 'mvd'

#endif // __mvdAlgorithm_h
