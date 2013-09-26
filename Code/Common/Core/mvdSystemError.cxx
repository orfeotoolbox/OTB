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
#include "mvdSystemError.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QDebug>

//
// System includes (sorted by alphabetic order)
#include <cerrno>
#include <cstring>
#include <sstream>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)


namespace mvd
{
/*
  TRANSLATOR mvd::SystemError

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
std::string
SystemError::whatString( int err, const std::string& msg )
{
  /*
  // Use Qt's tr() in order for the text string to be translated.
  // N.B.: strerror() string may use system locales.
  return ToStdString(
    msg.empty()
      ? tr( "System error %1: '%2'." )
      .arg( err )
      .arg( strerror( err ) )

      : tr( "System error %1: '%2' %3." )
      .arg( err )
      .arg( strerror( err ) )
      .arg( msg )

  );
  */

  std::stringstream sstream( std::ios_base::out );

  sstream
    << "System error "
    << err
    << ": "
    << strerror( err )
    << ".";

  if( !msg.empty() )
    sstream << std::endl << msg;

  return sstream.str();
}

/*******************************************************************************/
SystemError
::SystemError( const std::string& message ) :
  std::runtime_error( SystemError::whatString( errno, message ) ),
  m_ErrorCode( errno )
{
  // Do not use qCritial() nor qFatal() here because it will throw an
  // exception again causing infinite exception loop!
}

/*******************************************************************************/
SystemError
::SystemError( int errorCode, const std::string& message ) :
  std::runtime_error( SystemError::whatString( errorCode, message ) ),
  m_ErrorCode( errorCode )
{
  // Do not use qCritial() nor qFatal() here because it will throw an
  // exception again causing infinite exception loop!
}

/*******************************************************************************/
SystemError
::~SystemError()
  throw()
{
}

} // end namespace 'mvd'
