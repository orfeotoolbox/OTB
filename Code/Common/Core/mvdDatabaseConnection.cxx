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
  << ( sql ) << "\n"                            \
  << "params_1:" << ( values1 )
#else
#define QUERY_DEBUG_1( sql, values1 )
#endif

/*****************************************************************************/
#if USE_DEBUG
#define QUERY_DEBUG_2( sql, values1, values2 )                   \
  qDebug()                                                       \
  << ( sql ) << "\n"                                             \
  << "params_1:" << ( values1 ) << "\n"                          \
  << "params_2:" << ( values2 )
#else
#define QUERY_DEBUG_2( sql, values1, values2 )
#endif

/*****************************************************************************/
#if USE_DEBUG
#define QUERY_DEBUG_3( sql, values1, values2, values3 )                 \
  qDebug()                                                              \
  << ( sql ) << "\n"                                                    \
  << "params_1:" << ( values1 ) << "\n"                                 \
  << "params_2:" << ( values2 ) << "\n"                                 \
  << "params_3:" << ( values3 )
#else
#define QUERY_DEBUG_3( sql, values1, values2, values3 )
#endif

/*****************************************************************************/
#if USE_DEBUG || USE_DEBUG_BINDINGS
#define QUERY_DEBUG_BINDINGS( query )                           \
  qDebug() << "params:" << ( query ).boundValues()
#else
#define QUERY_DEBUG_BINDINGS( query )
#endif

/*****************************************************************************/
#if USE_DEBUG || USE_DEBUG_SIZE
#define QUERY_DEBUG_SIZE( query )                       \
  if( ( query ).isSelect() )                            \
    qDebug() << "size:" << ( query ).size()
#else
#define QUERY_DEBUG_SIZE( query )
#endif

/*****************************************************************************/
#define QUERY_PREPARE( qry, sql )               \
  QSqlQuery qry( m_SqlDatabase );               \
  if( !( qry ).prepare( sql ) )                 \
    {                                           \
    throw DatabaseError(                        \
      ( qry ).lastError(),                      \
      tr( "Failed to prepare query: " ),        \
      QString( "\n\"%1\"" ).arg( sql )          \
    );                                          \
    }

/*****************************************************************************/
#define QUERY_EXEC( qry )                               \
  if( !( qry ).exec() )                                 \
    {                                                   \
    throw DatabaseError(                                \
      ( qry ).lastError(),                              \
      tr( "Failed to execute query: " ),                \
      QString( "\n\"%1\"" ).arg( ( qry ).lastQuery() )  \
    );                                                  \
    }                                                   \
  assert( ( qry ).isActive() )

/*****************************************************************************/
#define QUERY_DIRECT_EXEC( qry, sql )                   \
  if( !( qry ).exec( sql ) )                            \
    {                                                   \
    throw DatabaseError(                                \
      ( qry ).lastError(),                              \
      tr( "Failed to execute query: " ),                \
      QString( "\n\"%1\"" ).arg( ( qry ).lastQuery() )  \
    );                                                  \
    }                                                   \
  assert( ( qry ).isActive() )

/*****************************************************************************/
#define BATCH_QUERY_EXEC( qry )                         \
  if( !( qry ).execBatch() )                            \
    {                                                   \
    throw DatabaseError(                                \
      ( qry ).lastError(),                              \
      tr( "Failed to batch query: " ),                  \
      QString( "\n\"%1\"" ).arg( ( qry ).lastQuery() )  \
    );                                                  \
    }                                                   \
  assert( ( qry ).isActive() )

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

const char* DatabaseConnection::TAG_NAMES[] = {  "Root", "Datasets", "Cached" };

/*******************************************************************************/
/*
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
*/

/*******************************************************************************/
void
DatabaseConnection
::InitializeDatabase()
{
  DatabaseConnection dbc;

  dbc.DirectExecuteQuery( "PRAGMA foreign_keys=1;" );

  for( int i=0; i<SQL_DB_CREATE_COUNT; ++i )
    {
    dbc.DirectExecuteQuery( SQL_DB_CREATE[ i ] );
    }

  for( int i=0; i<SQL_DB_SETUP_COUNT; ++i )
    {
    dbc.DirectExecuteQuery( SQL_DB_SETUP[ i ] );
    }

#if defined( _DEBUG ) || 1
  dbc.InsertTag( "Quickbird", "Datasets" );
  dbc.InsertTag( "Pleiades", "Datasets" );
  dbc.InsertTag( "SPOT", "Datasets" );
  dbc.InsertTag( "TERASAR", "Datasets" );
  dbc.InsertTag( "WV2", "Datasets" );
#endif

#if defined( _DEBUG ) || 0
  dbc.InsertTag( "Test-1", "Datasets" );

  dbc.InsertTag( "Test-2", "Datasets" );
  dbc.InsertTag( "Test-2.1", "Test-2" );

  dbc.InsertTag( "Test-3", "Datasets" );
  dbc.InsertTag( "Test-3.1", "Test-3" );
  dbc.InsertTag( "Test-3.1.1", "Test-3.1" );
  dbc.InsertTag( "Test-3.1.2", "Test-3.1" );
  dbc.InsertTag( "Test-3.2", "Test-3" );

  /*
  dbc.InsertTag( "Duplicate", "Datasets" );
  dbc.InsertTag( "Duplicate", "Datasets" );
  */

  dbc.InsertTag( "Test-4", "Root" );
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
  QObject( parent ),
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

  // Force foreign_keys=ON each time client software connects to
  // SQLite database because this pragma is not persistent (see sqlite
  // 3).
  //
  // The foreign_keys pragma is needed to ensure integrity
  // constraints, especially 'ON DELETE CASCADE' ones.
  DirectExecuteQuery( "PRAGMA foreign_keys=ON;" );

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
::InsertDataset( const QString& hash, const QString& label )
{
  //
  // Insert dataset entry.
  ExecuteQuery(
    QString( "INSERT INTO dataset( hash ) VALUES( '%1' );" ).arg( hash )
  );


  //
  // Get dataset ID.
  QSqlQuery findDatasetId(
    ExecuteQuery(
      QString( "SELECT dataset.id FROM dataset WHERE dataset.hash='%1'" )
      .arg( hash )
    )
  );

  QUERY_NEXT( findDatasetId );
  QUERY_DEBUG_FIELDS( findDatasetId );

  //
  // Get tag path.
  QSqlQuery findTagNode(
    ExecuteSelectQuery(
      SQLQ_SELECT_TAG_NODE_BY_TAG_LABEL,
      QVariantList() <<
      ( label.isEmpty()
        ? TAG_NAMES[ TAG_NAME_CACHED ] :
        label )
    )
  );

  QUERY_NEXT( findTagNode );
  QUERY_DEBUG_FIELDS( findTagNode );

  QVariant datasetId( findDatasetId.value( 0 ) );
  //
  QVariant id( findTagNode.value( 0 ) );
  QVariant path( findTagNode.value( 4 ) );

  QStringList tagNodeList( path.toString().split( "/" ) );
  qDebug() << "Discard:" << tagNodeList.first();
  tagNodeList.pop_front();
  tagNodeList.append( id.toString() );
  qDebug() << tagNodeList;

  ExecuteQuery(
    QString( SQL_QUERIES_INSERT[ SQLQ_INSERT_DATASET_MEMBERSHIP ] )
    .arg( datasetId.toString() )
    .arg( tagNodeList.join( "," ) )
  );

  /*
  BatchQuery(
    SQL_QUERIES_INSERT[ SQLQ_INSERT_DATASET_MEMBERSHIP ],
    QVariantList() << datasetId << QVariant( tagNodeList )
  );
  */
}

/*****************************************************************************/
void
DatabaseConnection
::DeleteDataset( const QString& hash )
{
  ExecuteQuery(
    QString( "DELETE FROM dataset WHERE dataset.hash='%1';" )
    .arg( hash )
  );
}

/*****************************************************************************/
DatabaseConnection::DatasetMap
DatabaseConnection
::ListAllDatasets() const
{
  QSqlQuery query(
    ExecuteQuery(
      "SELECT dataset.id, dataset.hash\n"
      "FROM dataset;"
    )
  );

  DatasetMap datasets;

  while( query.next() )
    {
    datasets.insert(
      query.value( 0 ).toLongLong(),
      query.value( 1 ).toString()
    );
    }

  return datasets;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::GetRootTagNode() const
{
  QSqlQuery query(
    ExecuteSelectQuery(
      SQLQ_SELECT_TAG_NODE_ROOT,
      QVariantList()
    )
  );

  QUERY_NEXT( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::GetTagNodeChildren( SqlId tagNodeId ) const
{
  return
    ExecuteSelectQuery(
      SQLQ_SELECT_TAG_NODE_CHILDREN,
      QVariantList() << tagNodeId
    );
}

/*****************************************************************************/
void
DatabaseConnection
::InsertTag( const QString& label, const QString& parent )
{
  ExecuteQuery(
    QString( "INSERT INTO tag( label ) VALUES( '%1' );" ).arg( label )
  );

  // assert( FindTagIdByName( parent )!=-1 );

  ExecuteQuery(
    SQL_QUERIES_INSERT[ SQLQ_INSERT_TAG_NODE ],
    QVariantList() << label << ( parent.isEmpty() ? "Root" : parent )
  );
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::DirectExecuteQuery( const QString& sql ) const
{
  QUERY_DEBUG_0( sql );

  QSqlQuery query( m_SqlDatabase );

#if 1
  QUERY_DIRECT_EXEC( query, sql );
#else
  if( !query.exec( sql ) )
    {
    throw DatabaseError(
      query.lastError(),
      tr( "Failed to execute query: " ),
      QString( "\n\"%1\"" ).arg( query.lastQuery() )
    );
    }
  assert( query.isActive() );
#endif

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::ExecuteQuery( const QString& sql ) const
{
  QUERY_DEBUG_0( sql );

  QUERY_PREPARE( query, sql );

  QUERY_EXEC( query );

  QUERY_DEBUG_SIZE( query );

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

  QUERY_DEBUG_BINDINGS( query );

  QUERY_EXEC( query );

  QUERY_DEBUG_SIZE( query );

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

  QUERY_DEBUG_BINDINGS( query );

  BATCH_QUERY_EXEC( query );

  QUERY_DEBUG_SIZE( query );

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

  QUERY_DEBUG_BINDINGS( query );

  BATCH_QUERY_EXEC( query );

  QUERY_DEBUG_SIZE( query );

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

  QUERY_DEBUG_BINDINGS( query );

  BATCH_QUERY_EXEC( query );

  QUERY_DEBUG_SIZE( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::ExecuteSelectQuery( SqlQueriesSelect queryId, const QVariantList& params ) const
{
  return ExecuteQuery( SQL_QUERIES_SELECT[ queryId ], params );
}

/*****************************************************************************/
SqlId
DatabaseConnection
::FindTagIdByLabel( const QString& label ) const
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

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
