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
#include "mvdDatabaseError.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <sstream>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"

namespace mvd
{
/*
  TRANSLATOR mvd::DatabaseError

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
std::string
DatabaseError
::whatString( const QSqlError& sqlError,
              const QString& prefix,
              const QString& suffix )
{
  std::stringstream strStream( std::ios_base::out );

  if( !prefix.isEmpty() )
    strStream << ToStdString( prefix );

  switch( sqlError.type() )
    {
    case QSqlError::NoError:
      strStream <<
        ToStdString(
          QCoreApplication::translate(
            "mvd::DatabaseError",
            "No database error"
          )
        );
      break;

    case QSqlError::ConnectionError:
      strStream <<
        ToStdString(
          QCoreApplication::translate(
            "mvd::DatabaseError",
            "Database connection error"
          )
        );
      break;

    case QSqlError::StatementError:
      strStream <<
        ToStdString(
          QCoreApplication::translate(
            "mvd::DatabaseError",
            "SQL statement syntax error" )
        );
      break;

    case QSqlError::TransactionError:
      strStream <<
        ToStdString(
          QCoreApplication::translate(
            "mvd::DatabaseError",
            "Database transaction failed error" )
        );
      break;

    case QSqlError::UnknownError:
      strStream <<
        ToStdString(
          QCoreApplication::translate(
            "mvd::DatabaseError",
            "Unknown database error" )
        );
      break;

    default:
      assert( false && "Unexpected QSqlError::ErrorType." );
      break;
    }

  strStream <<
    " " << sqlError.number() <<
    " '" << ToStdString( sqlError.databaseText() ) <<
    "'; '" << ToStdString( sqlError.driverText() ) <<
    "'.";

  if( !suffix.isEmpty() )
    strStream << ToStdString( suffix );

  return strStream.str();
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatabaseError
::DatabaseError( const QSqlError& sqlError,
                 const QString& prefix,
                 const QString& suffix ) :
  std::runtime_error( whatString( sqlError, prefix, suffix ) ),
  m_SqlError( sqlError )
{
  qDebug() << what();
}

/*******************************************************************************/
DatabaseError
::~DatabaseError()
  throw()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
