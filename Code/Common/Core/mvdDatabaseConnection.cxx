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
// #include <cstdarg>

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


  //
  // TAG TABLE.
  dbc.ExecuteQuery(
    "CREATE TABLE tag( "
    "id INTEGER PRIMARY KEY, "
    // "parent_id INTEGER NOT NULL DEFAULT 0 REFERENCES tag( id ), "
    "label TEXT NOT NULL );"
  );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_tag_label ON tag( label );"
  );
  // // TAG VALUES.
  // dbc.BatchQuery(
  //   "INSERT INTO tag( id, label ) VALUES( :id, :label );",
  //   QVariantList() << 0 << 1 << 2,
  //   QVariantList() << "Root" << "Datasets" << "Cached"
  // );


  //
  // TAG-NODE TABLE.
  dbc.ExecuteQuery(
    "CREATE TABLE tag_node( "
    "id INTEGER PRIMARY KEY, "
    "parent_id INTEGER REFERENCES tag_node( id ), "
    "tag_id INTEGER NOT NULL REFERENCES tag( id ), "
    "level INTEGER, "
    "path TEXT );"
  );
  dbc.ExecuteQuery(
    "CREATE INDEX idx_tag_node_tid ON tag_node( tag_id );"
  );

  //
  // DATASET MEMBERSHIP.
  dbc.ExecuteQuery(
    "CREATE TABLE dataset_membership( "
    // "id INTEGER PRIMARY KEY, "
    "dataset_id INTEGER NOT NULL REFERENCES dataset( id ), "
    "tag_id INTEGER NOT NULL REFERENCES tag( id ), "
    "PRIMARY KEY( dataset_id, tag_id ) );"
  );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_ds_membership_dataset_id "
    "ON dataset_membership( dataset_id );"
  );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_dsmembership_dataset_tag_id "
    "ON dataset_membership( tag_id );"
  );


  //
  // DATASET ATTRIBUTE.
  dbc.ExecuteQuery(
    "CREATE TABLE dataset_attribute( "
    "id INTEGER PRIMARY KEY, "
    "dataset_id INTEGER NOT NULL REFERENCES dataset( id ), "
    "rank INTEGER NOT NULL, "
    "name TEXT NOT NULL, "
    "value TEXT );"
  );
  dbc.ExecuteQuery(
    "CREATE UNIQUE INDEX idx_dataset_attribute_id ON "
    "dataset_attribute( id );"
  );
  dbc.ExecuteQuery(
    "CREATE INDEX idx_dataset_attribute_dsid "
    "ON dataset_attribute( dataset_id );"
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

/*******************************************************************************/
void
DatabaseConnection
::InitializeDatabase2()
{
  DatabaseConnection dbc;

  for( int i=0; i<SQL_CREATE_DB_COUNT; ++i )
    {
    qDebug() << SQL_CREATE_DB[ i ];
    dbc.ExecuteQuery( SQL_CREATE_DB[ i ] );
    }

#if 1
  for( int i=0; i<SQL_INSERT_ITEMS_COUNT; ++i )
    {
    qDebug() << SQL_INSERT_ITEMS[ i ];
    dbc.ExecuteQuery( SQL_INSERT_ITEMS[ i ] );
    }
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
#define BATCH_QUERY_DEBUG_2( sql, values1, values2 )            \
  qDebug() << this;                                             \
  qDebug() << "\tsql:" << ( sql );                              \
  qDebug() << "\tvalues1:" << ( values1 );                      \
  qDebug() << "\tvalues2:" << ( values2 );

/*****************************************************************************/
#define BATCH_QUERY_DEBUG_3( sql, values1, values2, values3 )           \
  qDebug() << this;                                                     \
  qDebug() << "\tsql:" << ( sql );                                      \
  qDebug() << "\tvalues1:" << ( values1 );                              \
  qDebug() << "\tvalues2:" << ( values2 );                              \
  qDebug() << "\tvalues3:" << ( values3 );

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
#define BATCH_QUERY_EXEC()                       \
  if( !query.execBatch() )                       \
    {                                            \
    throw DatabaseError(                         \
      query.lastError(),                         \
      tr( "Failed to execute query: " )          \
    );                                           \
    }

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1,
              const QVariantList& values2 )
{
  BATCH_QUERY_DEBUG_2( sql, values1, values2 );
  BATCH_QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1,
    values2
  );

  BATCH_QUERY_EXEC();

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1,
              const QVariantList& values2,
              const QVariantList& values3 )
{
  BATCH_QUERY_DEBUG_3( sql, values1, values2, values3 );
  BATCH_QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1,
    values2,
    values3
  );

  BATCH_QUERY_EXEC();

  return query;
}

/*****************************************************************************/
/*
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1,
              ... )
{
  // BATCH_QUERY_DEBUG_3( sql, values1, values2, values3 );
  BATCH_QUERY_PREPARE( query, sql );

  query.addBindValue( values1 );

  va_list args;

  va_start( args, values1 );

  for( QVariantList values = va_arg( args, QVariantList );
       args != NULL;
       values = va_arg( args, QVariantList ) )
    {
    query.addBindValue( values );
    }

  va_end( args );

  BATCH_QUERY_EXEC();

  return query;
}
*/

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
