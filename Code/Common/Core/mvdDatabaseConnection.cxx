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

int DatabaseConnection::m_InstanceCount = 0;

/*******************************************************************************/
void
DatabaseConnection
::InitializeDatabase()
{
  DatabaseConnection dbc;

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
    "CREATE INDEX idx_dataset_attribute_dsid "
    "ON dataset_attribute( dataset_id );"
  );
}

/*******************************************************************************/
void
DatabaseConnection
::InitializeDatabase2()
{
  DatabaseConnection dbc;

  for( int i=0; i<SQL_CREATE_DB_COUNT; ++i )
    {
    // qDebug() << SQL_CREATE_DB[ i ];
    dbc.ExecuteQuery( SQL_CREATE_DB[ i ] );
    }

#if 0
  for( int i=0; i<SQL_INSERT_ITEMS_COUNT; ++i )
    {
    // qDebug() << SQL_INSERT_ITEMS[ i ];
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
  if( QSqlDatabase::contains( "mvd2" ) )
    {
    qDebug() <<
      ( QSqlDatabase::database( "mvd2" ).isOpen()
        ? "Database connection is opened."
        : "Database connection is closed." );

    return QSqlDatabase::database( "mvd2" );
    }

  QString filename(
    I18nCoreApplication::ConstInstance()->GetCacheDir().filePath( "db.sqlite" )
  );

  qDebug() << "Adding database connection...";

  QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "mvd2" ) );

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
  if( !m_SqlDatabase.isOpen() )
    {
      qDebug() << "Opening database connection...";

      if( !m_SqlDatabase.open() )
        {
        throw DatabaseError(
          m_SqlDatabase.lastError(),
          tr( "Failed to open database: " )
        );
        }
    }

  assert( DatabaseConnection::m_InstanceCount>=0 );

  ++ DatabaseConnection::m_InstanceCount;
}

/*******************************************************************************/
DatabaseConnection
::~DatabaseConnection()
{
  -- DatabaseConnection::m_InstanceCount;

  assert( DatabaseConnection::m_InstanceCount>=0 );

  if( m_InstanceCount==0 )
    {
    qDebug() << "Closing database connection...";

    m_SqlDatabase.close();

    assert( !m_SqlDatabase.isOpen() );

    m_SqlDatabase = QSqlDatabase();

    qDebug() << "Removing database connection...";

    QSqlDatabase::removeDatabase( "mvd2" );
    }
}

/*****************************************************************************/
void
DatabaseConnection
::InsertDataset( const QString& hash )
{
  ExecuteQuery(
    QString( "INSERT INTO dataset( hash ) VALUES( '%1' );" ).arg( hash )
  );
}

/*****************************************************************************/
void
DatabaseConnection
::ExecuteQuery( const QString& sql )
{
  QSqlQuery query( m_SqlDatabase );

  qDebug() << sql;

  if( !query.exec( sql ) )
    {
    throw DatabaseError(
      query.lastError(),
      tr( "Failed to execute query: " ),
      QString( "\n'%1'" ).arg( sql )
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
      tr( "Failed to prepare query: " ),        \
      QString( "\n'%1'" ).arg( sql )            \
    );                                          \
    }

/*****************************************************************************/
#define BATCH_QUERY_EXEC()                       \
  if( !query.execBatch() )                       \
    {                                            \
    throw DatabaseError(                         \
      query.lastError(),                         \
      tr( "Failed to execute batch query: " )    \
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
