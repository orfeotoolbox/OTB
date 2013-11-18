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
#include "mvdDatabaseConnection.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtSql>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdDatabaseError.h"
#include "Core/mvdI18nCoreApplication.h"

namespace mvd
{
/*
  TRANSLATOR mvd::DatabaseConnection

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

//
// Include generated SQL strings.
#include "mvdSql.cxx"

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*******************************************************************************/
void
DatabaseConnection
::InitializeDatabase()
{
  DatabaseConnection dbc;

#if 0
  if( !db.transaction() )
    throw DatabaseError(
      db.lastError(),
      tr( "Error while beginning database transaction: " )
    );
#endif

  dbc.ExecuteQuery( "PRAGMA foreign_keys = ON;" );


  //
  // DATASET TABLE.
  dbc.ExecuteQuery(
    "CREATE TABLE dataset( id INTEGER PRIMARY KEY, hash TEXT NOT NULL );" );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_dataset_id ON dataset( id );" );
  // dbc.ExecuteQuery(
  //   "CREATE UNIQUE INDEX idx_dataset_hash ON dataset( hash );" );


  //
  // TAG TABLE.
  dbc.ExecuteQuery(
    "CREATE TABLE tag( "
    "id INTEGER PRIMARY KEY,"
    "parent_id INTEGER NOT NULL DEFAULT 0 REFERENCES tag( id ),"
    "label TEXT NOT NULL DEFAULT 'Label' );"
  );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_tag_id ON tag( id );"
  );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_tag_label ON tag( label );"
  );
  // TAG VALUES.
  /*
  dbc.BatchQuery(
    "INSERT INTO tag( id, parent_id, label ) VALUES( :id, :parent_id, :label );",
    QVariantList() << 0 << 1,
    QVariantList() << 0 << 0,
    QVariantList() << "Root" << "Incoming",
    QSqlQuery::ValuesAsRows
  );
  */
  dbc.BatchQuery(
    "INSERT INTO tag( id, parent_id, label ) VALUES( :id, :parent_id, :label );",
    QVariantList() << 0 << 1,
    QVariantList() << 0 << 0,
    QVariantList() << "Root" << "Incoming",
    QSqlQuery::ValuesAsRows
  );

#if 0
  if( !query.exec( SQL_CREATE_DB ) )
    {
    /*
    // Do throw exception which would mask main exception. Simply
    // trace it as a warning.
    if( !db.rollback() )
      qWarning()
        << "Error while rolling database transaction back: "
        << db.lastError();
    */

    // Securely close database connection.
    // db.close();

    // Throw main exception.
    throw DatabaseError(
      query.lastError(),
      tr( "Error when executing database query: " )
    );
    }

  /*
  if( !db.commit() )
    {
    throw DatabaseError(
      db.lastError(),
      tr( "Error while committing database transaction: " )
    );
    }
  */
#endif
}

/*****************************************************************************/
inline
QSqlDatabase
DatabaseConnection
::SqlDatabase()
{
  QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "mvd2" ) );

  QString filename(
    I18nCoreApplication::ConstInstance()->GetCacheDir().filePath( "db.sqlite" )
  );

  db.setDatabaseName( filename );

  assert( db.driver()!=NULL );
  assert( db.driver()->hasFeature( QSqlDriver::Transactions ) );
  assert( db.driver()->hasFeature( QSqlDriver::PreparedQueries ) );

  return db;
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*******************************************************************************/
DatabaseConnection
::DatabaseConnection( QObject* parent ) :
  m_SqlDatabase( SqlDatabase() )
{
  if( !m_SqlDatabase.open() )
    {
    throw DatabaseError(
      m_SqlDatabase.lastError(),
      tr( "Failed to open database: " )
    );
    }
}

/*******************************************************************************/
DatabaseConnection
::~DatabaseConnection()
{
  m_SqlDatabase.close();

  m_SqlDatabase = QSqlDatabase();
}

/*****************************************************************************/
void
DatabaseConnection
::ExecuteQuery( const QString& sql )
{
  QSqlQuery query( m_SqlDatabase );

  if( !query.exec( sql ) )
    {
    throw DatabaseError(
      query.lastError(),
      tr( "Failed to execute query: " )
    );
    }
}

/*****************************************************************************/
#define BATCH_QUERY_DEBUG_2( sql, values1, values2, mode )      \
  qDebug() << this;                                             \
  qDebug() << "\tsql:" << ( sql );                              \
  qDebug() << "\tvalues1:" << ( values1 );                      \
  qDebug() << "\tvalues2:" << ( values2 );                      \
  qDebug() << "\tmode:" << ( mode );

/*****************************************************************************/
#define BATCH_QUERY_DEBUG_3( sql, values1, values2, values3, mode )     \
  qDebug() << this;                                                     \
  qDebug() << "\tsql:" << ( sql );                                      \
  qDebug() << "\tvalues1:" << ( values1 );                              \
  qDebug() << "\tvalues2:" << ( values2 );                              \
  qDebug() << "\tvalues3:" << ( values3 );                              \
  qDebug() << "\tmode:" << ( mode );

/*****************************************************************************/
#define BATCH_QUERY_PREPARE( query, sql )       \
  QSqlQuery query( m_SqlDatabase );             \
                                                \
  if( !query.prepare( sql ) )                   \
    {                                           \
    throw DatabaseError(                        \
      query.lastError(),                        \
      tr( "Failed to prepare query: " )         \
    );                                          \
    }

/*****************************************************************************/
#define BATCH_QUERY_EXEC( mode )                \
  if( !query.execBatch( mode ) )                \
    {                                           \
    throw DatabaseError(                        \
      query.lastError(),                        \
      tr( "Failed to execute query: " )          \
    );                                          \
    }

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1,
              const QVariantList& values2,
              QSqlQuery::BatchExecutionMode mode )
{
  BATCH_QUERY_DEBUG_2( sql, values1, values2, mode );
  BATCH_QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1,
    values2
  );

  BATCH_QUERY_EXEC( mode );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1,
              const QVariantList& values2,
              const QVariantList& values3,
              QSqlQuery::BatchExecutionMode mode )
{
  BATCH_QUERY_DEBUG_3( sql, values1, values2, values3, mode );
  BATCH_QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1,
    values2,
    values3
  );

  BATCH_QUERY_EXEC( mode );

  return query;
}

/*****************************************************************************/
#if 0
void
DatabaseConnection
::ExecuteQuery( QSqlDatabase& database, QSqlQuery& query )
{
  if( !query.exec() )
    {
    // Do throw exception which would mask main exception. Simply
    // trace it as a warning.
    if( !database.rollback() )
      qWarning()
        << "Error when rolling database transaction back: "
        << database.lastError();

    // Securely close database connection.
    database.close();

    // Throw main exception.
    throw DatabaseError(
      query.lastError(),
      tr( "Error when executing database query: " )
    );
    }

  if( !database.commit() )
    {
    throw DatabaseError(
      database.lastError(),
      tr( "Error when committing database transaction: " )
    );
    }
}
#endif

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
