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

#include "mvdSerializableInterface.h"


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
#include "mvdAlgorithm.h"
#include "mvdSystemError.h"

namespace mvd
{
/*
  TRANSLATOR mvd::SerializableInterface

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
SerializableInterface
::SerializableInterface()
{
}

/*******************************************************************************/
SerializableInterface
::~SerializableInterface()
{
}

/*******************************************************************************/
void
SerializableInterface
::Read( const QString& filename, Mode mode )
{
  // File instance.
  QFile file( filename );

  // Setup open-mode.
  QIODevice::OpenMode openMode( QIODevice::ReadOnly );

  if( mode==MODE_TEXT )
    openMode |= QIODevice::Text;

  // Open file on device.
  if( !file.open( openMode ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

  try
    {
    // Read file context.
    virtual_Read( &file );
    }
  catch( SystemError& syserr )
    {
    // Catch any SystemError thrown by DatasetDescriptor::Write() and
    // morph it into the same SystemError containing filename
    // information.
    syserr = SystemError(
      syserr.GetErrorCode(),
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

    // Throw morphed SystemError.
    throw syserr;
    }

  // File is closed by automatic scope detruction of QFile instance.
}

/*******************************************************************************/
void
SerializableInterface
::Write( const QString& filename, Mode mode ) const
{
  // File instance.
  QFile file( filename );

  // Setup open-mode.
  QIODevice::OpenMode openMode( QIODevice::WriteOnly );

  if( mode==MODE_TEXT )
    openMode |= QIODevice::Text;

  // Open file on device.
  if( !file.open( openMode ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

  try
    {
    // Write file context.
    virtual_Write( file );
    }
  catch( SystemError& syserr )
    {
    // Catch any SystemError thrown by DatasetDescriptor::Write() and
    // morph it into the same SystemError containing filename
    // information.
    syserr = SystemError(
      syserr.GetErrorCode(),
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

    // Throw morphed SystemError.
    throw syserr;
    }

  // File is closed by automatic scope detruction of QFile instance.
}

/*******************************************************************************/

} // end namespace 'mvd'
