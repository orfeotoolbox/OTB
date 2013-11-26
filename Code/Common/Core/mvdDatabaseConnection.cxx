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
/* MACROS                                                                    */

#define USE_DEBUG 0

/*****************************************************************************/
#if USE_DEBUG
#define QUERY_DEBUG_0( sql ) qDebug() << ( sql )
#else
#define QUERY_DEBUG_0( sql )
#endif

/*****************************************************************************/
#if USE_DEBUG
#define QUERY_DEBUG_1( sql, values1 )           \
  qDebug()                                      \
  << "\n" << ( sql )                            \
  << "\nvalues_1:" << ( values1 )
#else
#define QUERY_DEBUG_1( sql, values1 )
#endif

/*****************************************************************************/
#if USE_DEBUG
#define QUERY_DEBUG_2( sql, values1, values2 )                   \
  qDebug()                                                       \
  << "\n" << ( sql )                                             \
  << "\nvalues_1:" << ( values1 )                                \
  << "\nvalues_2:" << ( values2 )
#else
#define QUERY_DEBUG_2( sql, values1, values2 )
#endif

/*****************************************************************************/
#if USE_DEBUG
#define QUERY_DEBUG_3( sql, values1, values2, values3 )                 \
  qDebug()                                                              \
  << "\n" << ( sql )                                                    \
  << "\nvalues_1:" << ( values1 )                                       \
  << "\nvalues_2:" << ( values2 )                                       \
  << "\nvalues_3:" << ( values3 )
#else
#define QUERY_DEBUG_3( sql, values1, values2, values3 )
#endif

/*****************************************************************************/
#define QUERY_PREPARE( qry, sql )               \
  QSqlQuery query( m_SqlDatabase );             \
  if( !qry.prepare( sql ) )                     \
    {                                           \
    throw DatabaseError(                        \
      qry.lastError(),                          \
      tr( "Failed to prepare query: " ),        \
      QString( "\n\"%1\"" ).arg( sql )          \
    );                                          \
    }

/*****************************************************************************/
#define QUERY_EXEC( qry )                               \
  if( !qry.exec() )                                     \
    {                                                   \
    throw DatabaseError(                                \
      qry.lastError(),                                  \
      tr( "Failed to execute query: " ),                \
      QString( "\n\"%1\"" ).arg( qry.lastQuery() )      \
    );                                                  \
    }                                                   \
  assert( qry.isActive() )

/*****************************************************************************/
#define BATCH_QUERY_EXEC( qry )                         \
  if( !qry.execBatch() )                                \
    {                                                   \
    throw DatabaseError(                                \
      qry.lastError(),                                  \
      tr( "Failed to batch query: " ),                  \
      QString( "\n\"%1\"" ).arg( qry.lastQuery() )      \
    );                                                  \
    }                                                   \
  assert( qry.isActive() )

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
::InitializeDatabase2()
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
::InitializeDatabase()
{
  DatabaseConnection dbc;

  for( int i=0; i<SQL_DB_CREATE_COUNT; ++i )
    {
    dbc.ExecuteQuery( SQL_DB_CREATE[ i ] );
    }

#if 1
  for( int i=0; i<SQL_DB_SETUP_COUNT; ++i )
    {
    dbc.ExecuteQuery( SQL_DB_SETUP[ i ] );
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
SqlId
DatabaseConnection
::FindTagIdByName( const QString& label ) const
{
  QSqlQuery query(
    ExecuteQuery(
      QString(
        "SELECT tag.id, tag.label\n"
        "FROM tag\n"
        "WHERE tag.label='%1'\n"
      ).arg( label )
    )
  );

  assert( query.size()==0 || query.size()==1 );

  if( query.size()==0 )
    return -1;

  /*
  if( query.size()!=1 )
    throw std::runtime_error(
      ToStdString(
        tr( "Non-unique tag (label '%1') found in database." ).arg( label )
      )
    );
  */

  if( !query.next() )
    throw DatabaseError(
      query.lastError(),
      tr( "Failed to fetch tag-id by tag-label: " )
    );

  assert( query.value( 0 ).type()==QVariant::Int );

  return query.value( 0 ).toInt();
}

/*****************************************************************************/
void
DatabaseConnection
::InsertTag( const QString& label, const QString& parent )
{
  ExecuteQuery(
    QString( "INSERT INTO tag( label ) VALUES( '%1' );" ).arg( label )
  );

  if( !parent.isEmpty() )
    {
    ExecuteQuery(
      SQL_QUERIES_INSERT[ SQLQ_INSERT_TAG_NODE ],
      QVariantList() << label << parent
    );
    }
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::ExecuteQuery( const QString& sql ) const
{
  QUERY_DEBUG_0( sql );
  QUERY_PREPARE( query, sql );
  QUERY_EXEC( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::ExecuteQuery( const QString& sql, const QVariantList& params ) const
{
  QUERY_DEBUG_1( sql, params );
  QUERY_PREPARE( query, sql );

  int i = 0;

  for( QVariantList::const_iterator it( params.begin() );
       it!=params.end();
       ++it, ++i )
    {
    // qDebug() << i << ":" << *it;

    query.bindValue( i, *it );
    }

  QUERY_EXEC( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1 )
{
  QUERY_DEBUG_1( sql, values1 );
  QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1
  );

  BATCH_QUERY_EXEC( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::BatchQuery( const QString& sql,
              const QVariantList& values1,
              const QVariantList& values2 )
{
  QUERY_DEBUG_2( sql, values1, values2 );
  QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1,
    values2
  );

  BATCH_QUERY_EXEC( query );

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
  QUERY_DEBUG_3( sql, values1, values2, values3 );
  QUERY_PREPARE( query, sql );

  DatabaseConnection::AddBindValue(
    query,
    values1,
    values2,
    values3
  );

  BATCH_QUERY_EXEC( query );

  return query;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
