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

#include "ConfigureMonteverdi2.h"

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
#include "Core/mvdDatabaseQueries.h"
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
#if (USE_DEBUG && USE_DEBUG_BINDINGS) || FORCE_DEBUG_BINDINGS
#define QUERY_DEBUG_BINDINGS( query )                           \
  qDebug() << "params:" << ( query ).boundValues()
#else
#define QUERY_DEBUG_BINDINGS( query )
#endif

/*****************************************************************************/
#if (USE_DEBUG && USE_DEBUG_SIZE) || FORCE_DEBUG_SIZE
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

const char* DatabaseConnection::TAG_NAMES[] = {
  "Root",
  "Datasets",
  "Temporary"
};

/*****************************************************************************/
QFileInfo
DatabaseConnection
::SqlDatabaseFileInfo()
{
  return
    QFileInfo(
      I18nCoreApplication::ConstInstance()->GetCacheDir().filePath( "db.sqlite" )
    );
}

/*******************************************************************************/
void
DatabaseConnection
::InitializeDatabase()
{
  // MANTIS-854 (http://bugs.orfeo-toolbox.org/view.php?id=854).
  // MANTIS-876 (http://bugs.orfeo-toolbox.org/view.php?id=876).
  //
  // Added connection-mode.
  DatabaseConnection dbc( DatabaseConnection::CONNECTION_MODE_OPEN_OR_CREATE );

  // Force FOREIGN KEYS support.
  dbc.DirectExecuteQuery( "PRAGMA foreign_keys=1;" );

  // Create tables.
  for( int i=0; i<SQL_DB_CREATE_COUNT; ++i )
    {
    dbc.DirectExecuteQuery( SQL_DB_CREATE[ i ] );
    }

  // Add database attributes.
  dbc.AddAttribute( "version", Monteverdi2_DATA_VERSION_STRING );

  // Add some basic entries into tables.
  for( int i=0; i<SQL_DB_SETUP_COUNT; ++i )
    {
    dbc.DirectExecuteQuery( SQL_DB_SETUP[ i ] );
    }

// Conditionaly create demonstration groups.
// #if (defined( _DEBUG ) && FORCE_DISABLE) || FORCE_ENABLE
#if (defined( _DEBUG ) && 0) || 0
  SqlId datasetNodeId(
    GetNodeFields( dbc.FindDatasetNode() )
  );

  dbc.InsertNode( "Quickbird", datasetNodeId );
  dbc.InsertNode( "Pleiades", datasetNodeId );
  dbc.InsertNode( "SPOT", datasetNodeId );
  dbc.InsertNode( "TERASAR", datasetNodeId );
  dbc.InsertNode( "WV2", datasetNodeId );
#endif

// Conditionaly create debug groups.
// #if (defined( _DEBUG ) && FORCE_DISABLE) || FORCE_ENABLE
#if (defined( _DEBUG ) && 0) || 0
  SqlId rootNodeId(
    GetRootNodeFields( dbc.FindRootNode() )
  );

  // TEST-1
  dbc.InsertNode( "Test-1", datasetNodeId );

  // TEST-2
  SqlId test2NodeId = -1;
  dbc.InsertNode( "Test-2", datasetNodeId, &test2NodeId );
  dbc.InsertNode( "Test-2.1", test2NodeId );
  dbc.InsertNode(
    DatabaseConnection::TAG_NAMES[ TAG_NAME_TEMPORARY ], test2NodeId
  );

  // TEST-3
  SqlId test3NodeId = -1;
  dbc.InsertNode( "Test-3", datasetNodeId, &test3NodeId );
  SqlId test31NodeId = -1;
  dbc.InsertNode( "Test-3.1", test3NodeId, &test31NodeId );
  dbc.InsertNode( "Test-3.1.1", test31NodeId );
  dbc.InsertNode( "Test-3.1.2", test31NodeId );
  dbc.InsertNode( "Test-3.2", test3NodeId );

  dbc.InsertNode( "Test-4", rootNodeId );
#endif
}

/*****************************************************************************/
QSqlDatabase
DatabaseConnection
::SqlDatabase( ConnectionMode mode )
{
  if( QSqlDatabase::contains( "mvd2" ) )
    {
    qDebug() <<
      ( QSqlDatabase::database( "mvd2" ).isOpen()
        ? "Database connection is opened."
        : "Database connection is closed." );

    return QSqlDatabase::database( "mvd2" );
    }

  // MANTIS-854 (http://bugs.orfeo-toolbox.org/view.php?id=854).
  // MANTIS-876 (http://bugs.orfeo-toolbox.org/view.php?id=876).
  //
  // When OPEN_ONLY, database file must exists.
  //
  // Otherwise, when OPEN_OR_CREATE, database file will be created by
  // QSqlDatabase.
  //
  // BEGIN 
  // {

  QFileInfo finfo( DatabaseConnection::SqlDatabaseFileInfo() );

  if( mode==DatabaseConnection::CONNECTION_MODE_OPEN_ONLY && !finfo.exists() )
    throw
      std::runtime_error(
        ToStdString(
          tr( "SQLite database file '%1' not found!" ).arg( finfo.filePath() )
        )
      );

  // }
  // END
  //
  // MANTIS-854 (http://bugs.orfeo-toolbox.org/view.php?id=854).
  // MANTIS-876 (http://bugs.orfeo-toolbox.org/view.php?id=876).

  qDebug() << "Adding database connection...";

  QSqlDatabase db( QSqlDatabase::addDatabase( "QSQLITE", "mvd2" ) );

  db.setDatabaseName( finfo.filePath() );

  assert( db.driver()!=NULL );
  assert( db.driver()->hasFeature( QSqlDriver::Transactions ) );
  assert( db.driver()->hasFeature( QSqlDriver::PreparedQueries ) );

  return db;
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*******************************************************************************/
DatabaseConnection
::DatabaseConnection( ConnectionMode mode, QObject* parent ) :
  QObject( parent ),
  m_SqlDatabase( SqlDatabase( mode ) )
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
::AddAttribute( const QString& name, const QVariant& value )
{
  DirectExecuteQuery(
    QString(
      "INSERT INTO database_attribute( id, name, value )\n"
      "VALUES( 1, '%1', '%2' );"
    )
    .arg( name )
    .arg( value.toString() )
  );
}

/*****************************************************************************/
void
DatabaseConnection
::SetAttribute( const QString& name, const QVariant& value )
{
  DirectExecuteQuery(
    QString( "UPDATE database_attribute SET value='%2' WHERE name='%1';" )
    .arg( name )
    .arg( value.toString() )
  );
}

/*****************************************************************************/
QVariant
DatabaseConnection
::GetAttribute( const QString& name, SqlId* id ) const
{
  QSqlQuery query(
    DirectExecuteQuery(
      QString( "SELECT database_attribute.id, database_attribute.value\n"
               "FROM database_attribute\n"
               "WHERE database_attribute.name='%1';"
      ).arg( name )
    )
  );

  //
  // Get ID, if needed.
  assert( query.value( 0 ).type()==QVariant::LongLong );

  if( id!=NULL )
    {
    *id = query.value( 0 ).toLongLong();
    }

  //
  // Return value.
  assert( query.value( 1 ).type()==QVariant::String );

  return query.value( 1 );
}

/*****************************************************************************/
void
DatabaseConnection
::InsertDataset( const QString& hash,
                 const QString& alias,
                 SqlId nodeId )
{
  //
  // Insert dataset entry.
  ExecuteQuery(
    QString( "INSERT INTO dataset( hash, alias ) VALUES( '%1', '%2' );" )
    .arg( hash )
    .arg( alias )
  );


  //
  // Get dataset ID.
  // TODO: Write accessor method.
  QSqlQuery findDatasetId(
    ExecuteQuery(
      QString( "SELECT dataset.id FROM dataset WHERE dataset.hash='%1'" )
      .arg( hash )
    )
  );

  QUERY_NEXT( findDatasetId );
  // QUERY_DEBUG_FIELDS( findDatasetId );

  QVariant datasetId( findDatasetId.value( 0 ) );


  //
  // Get tag path.
  QString path;

  SqlId id = GetNodeFields(
    ( nodeId<0
      ? FindTemporaryNode()
      : FindNode( nodeId )
    ),
    NULL, NULL, NULL, NULL, &path
  );

  QStringList nodeList( path.split( "/" ) );
  qDebug() << "Discard:" << nodeList.first();
  nodeList.pop_front();
  nodeList.append( QString( "%1" ).arg( id ) );
  qDebug() << nodeList;

  //
  // Add dataset tag membership.
  ExecuteQuery(
    QString( SQL_QUERIES_INSERT[ SQLQ_INSERT_DATASET_TAG_MEMBERSHIP ] )
    .arg( datasetId.toString() )
    .arg( nodeList.join( "," ) )
  );

  //
  // Add dataset node membership.
  ExecuteQuery(
    QString( SQL_QUERIES_INSERT[ SQLQ_INSERT_DATASET_NODE_MEMBERSHIP ] ),
    QVariantList() << datasetId << id
  );
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
void
DatabaseConnection
::UpdateDatasetAliasFromHash( const QString& hash, const QString& alias )
{
  ExecuteQuery(
    "UPDATE dataset SET alias=:alias WHERE hash=:id",
    QVariantList() << alias << hash
  );
}

/*****************************************************************************/
void
DatabaseConnection
::UpdateDatasetNodeMembership( SqlId id, SqlId nodeId )
{
  ExecuteQuery(
    "UPDATE dataset_node_membership SET node_id=:node_id WHERE dataset_id=:id;",
    QVariantList() << nodeId << id
  );

  // WARNING: dataset_tag_membership not yet maintained.
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
::FindRootNode() const
{
  QSqlQuery query(
    ExecuteSelectQuery( SQLQ_SELECT_NODE_ROOT )
  );

  QUERY_NEXT( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::FindDatasetNode() const
{
  return FindNodeChild(
    GetRootNodeFields( FindRootNode() ),
    DatabaseConnection::TAG_NAMES[ DatabaseConnection::TAG_NAME_DATASETS ]
  );
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::FindTemporaryNode() const
{
  return FindNodeChild(
    GetNodeFields( FindDatasetNode() ),
    DatabaseConnection::TAG_NAMES[ DatabaseConnection::TAG_NAME_TEMPORARY ]
  );
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::FindNode( SqlId id ) const
{
  QSqlQuery query(
    ExecuteSelectQuery( SQLQ_SELECT_NODE )
  );

  QUERY_NEXT( query );

  return query;
}

/*****************************************************************************/
void
DatabaseConnection
::InsertNode( const QString& label, SqlId parentId, SqlId* id )
{
  SqlId tagId = FindTagIdByLabel( label );

  if( tagId<0 )
    {
    ExecuteQuery(
      QString( "INSERT INTO tag( label ) VALUES( '%1' );" ).arg( label )
    );
    }

  ExecuteQuery(
    SQL_QUERIES_INSERT[ SQLQ_INSERT_NODE_CHILD ],
    QVariantList() << label << label << parentId
  );

  if( id!=NULL )
    {
    *id = GetNodeFields( FindNodeChild( parentId, label ) );
    }
}

/*****************************************************************************/
void
DatabaseConnection
::DeleteNode( SqlId id )
{
  qDebug() << this << "::DeleteNode(" << id << ")";
 
  ExecuteQuery(
  QString( "DELETE FROM node WHERE node.id='%1';" )
    .arg( id )
  );
}

/*****************************************************************************/
void
DatabaseConnection
::UpdateNodeLabel( SqlId id, const QString& label )
{
  qDebug() << this << "::UpdateNodeLabel(" << id << "," << label << ")";

  ExecuteQuery(
    "UPDATE node SET label=:label WHERE id=:id",
    QVariantList() << label << id
  );
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::SelectNodeChildren( SqlId nodeId ) const
{
  return
    ExecuteSelectQuery(
      SQLQ_SELECT_NODE_CHILDREN,
      QVariantList() << nodeId
    );
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::FindNodeChild( SqlId nodeId, const QString& childLabel ) const
{
  QSqlQuery query(
    ExecuteSelectQuery(
      SQLQ_SELECT_NODE_CHILD,
      QVariantList() << nodeId << childLabel
    )
  );

  QUERY_NEXT( query );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::SelectNodeDatasets( SqlId nodeId ) const
{
  QSqlQuery query(
    ExecuteSelectQuery(
      SQLQ_SELECT_NODE_DATASETS,
      QVariantList() << nodeId
    )
  );

  return query;
}

/*****************************************************************************/
QSqlQuery
DatabaseConnection
::DirectExecuteQuery( const QString& sql ) const
{
  QUERY_DEBUG_0( sql );

  QSqlQuery query( m_SqlDatabase );

  QUERY_DIRECT_EXEC( query, sql );

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

  // QUERY_DEBUG_SIZE( query );

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

  // QUERY_DEBUG_SIZE( query );

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

  // QUERY_DEBUG_SIZE( query );

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

  // QUERY_DEBUG_SIZE( query );

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

  // QUERY_DEBUG_SIZE( query );

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

  /*
  assert( query.size()==0 || query.size()==1 );

  if( query.size()==0 )
    return -1;
  */

  /*
  if( query.size()!=1 )
    throw std::runtime_error(
      ToStdString(
        tr( "Non-unique tag (label '%1') found in database." ).arg( label )
      )
    );
  */

  if( !query.next() )
    return -1;

  assert( query.value( 0 ).type()==QVariant::LongLong );

  return query.value( 0 ).toLongLong();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
