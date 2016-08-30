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
#include "mvdDoubleValidator.h"


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
  TRANSLATOR mvd::DoubleValidator

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
DoubleValidator
::DoubleValidator( QObject* p ) :
  QDoubleValidator( p )
{
}

/*******************************************************************************/
DoubleValidator
::DoubleValidator( double bottomVal, double topVal, int decim, QObject* p ) :
  QDoubleValidator( bottomVal, topVal, decim, p )
{
}

/*******************************************************************************/
DoubleValidator
::~DoubleValidator()
{
}

/*******************************************************************************/
void
DoubleValidator
::fixup( QString& input ) const
{
  int pos = input.length();

  if( validate( input, pos )!=QValidator::Intermediate )
    return;

  bool isOk;
  double value = input.toDouble( &isOk );

  if( !isOk )
    {
    }

  if( value<bottom() )
    input = QString::number( bottom(), 'g', decimals() );

  else if( value>top() )
    input = QString::number( top(), 'g', decimals() );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
