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
#ifndef __mvdDatabaseConnection_h
#define __mvdDatabaseConnection_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
// #include <QtCore>
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
#include "Core/mvdTypes.h"


/*****************************************************************************/
/* MACROS                                                                    */

#define USE_DEBUG 0
#define USE_DEBUG_BINDINGS 1
#define USE_DEBUG_SIZE 0
#define USE_DEBUG_VALUES 1

/*****************************************************************************/
#define QUERY_NEXT( query )                     \
  if( !( query ).next() )                       \
    {                                           \
    throw DatabaseError(                        \
      ( query ).lastError(),                    \
      tr( "Failed to fetch query record %1. " ) \
      .arg( ( query ).at() ),                   \
      QString( "\n\"%1\"\nwith: %2" )           \
      .arg( ( query ).lastQuery() )             \
    );                                          \
    }

/*****************************************************************************/
#define QUERY_NEXT_STATIC( query )              \
  if( !( query ).next() )                       \
    {                                           \
    throw DatabaseError(                        \
      ( query ).lastError(),                    \
      QCoreApplication::translate(              \
        "mvd::DatabaseConnection",              \
        "Failed to fetch query record %1. "     \
      )                                         \
      .arg( ( query ).at() ),                   \
      QString( "\n\"%1\"" )                     \
      .arg( ( query ).lastQuery() )             \
    );                                          \
    }

/*****************************************************************************/
#if USE_DEBUG || USE_DEBUG_VALUES
#define QUERY_DEBUG_FIELDS( query )             \
  {                                             \
    QVariant field;                             \
    int i;                                      \
    for( i=0, field=( query ).value( i );       \
         field.isValid();                       \
         field=( query ).value( ++i ) )         \
      {                                         \
      qDebug() << i << field;                   \
      }                                         \
  }
#else
#define QUERY_DEBUG_FIELDS( query )
#endif

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DatabaseConnection
 *
 * \brief WIP.
 */
class Monteverdi2_EXPORT DatabaseConnection :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
//
public:
  /**
   */
  typedef QMap< SqlId, QString > DatasetMap;

//
// Public methods.
public:

  /** \brief Constructor. */
  DatabaseConnection( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~DatabaseConnection();

  //
  // Database related methods.

  /**
   */
  static void InitializeDatabase();
  // static void InitializeDatabase2();

  //
  // Dataset related methods.

  /**
   */
  void InsertDataset( const QString& hash, const QString& label = QString() );

  /**
   */
  void DeleteDataset( const QString& hash );

  /**
   */
  DatasetMap ListAllDatasets() const;

  //
  // Tag and tag-nodes related methods.

  /**
   */
  QSqlQuery GetRootTagNode() const;

  /**
   */
  QSqlQuery GetTagNodeChildren( SqlId tagNodeId ) const;

  /**
   */
  void InsertTag( const QString& label, const QString& parent =QString() );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:

  /**
   */
  enum TagName
  {
    TAG_NAME_ROOT = 0,
    TAG_NAME_DATASETS,
    TAG_NAME_TEMPORARY,
    //
    TAG_NAMES_COUNT,
  };

  /**
   */
  enum SqlQueriesInsert
  {
    //
    SQLQ_INSERT_NONE = -1,
    //
    SQLQ_INSERT_TAG_NODE = 0,
    SQLQ_INSERT_DATASET_MEMBERSHIP,
    //
    SQLQ_INSERT_COUNT,
  };

  /**
   */
  enum SqlQueriesSelect
  {
    //
    SQLQ_SELECT_NONE = -1,
    //
    SQLQ_SELECT_TAG_NODE_BY_TAG_LABEL = 0,
    SQLQ_SELECT_TAG_NODE_ROOT,
    SQLQ_SELECT_TAG_NODE_CHILDREN,
    //
    SQLQ_SELECT_COUNT,
  };

//
// Private methods.
private:

  /**
   */
  inline static QSqlDatabase SqlDatabase();

  /**
   */
  inline static void AddBindValue( QSqlQuery& query,
                                   const QVariantList& values1 );

  /**
   */
  inline static void AddBindValue( QSqlQuery& query,
                                   const QVariantList& values1,
                                   const QVariantList& values2 );

  /**
   */
  inline static void AddBindValue( QSqlQuery& query,
                                   const QVariantList& values1,
                                   const QVariantList& values2,
                                   const QVariantList& values3 );

  /**
   */
  QSqlQuery DirectExecuteQuery( const QString& sql ) const;

  /**
   */
  QSqlQuery ExecuteQuery( const QString& sql ) const;

  /**
   */
  QSqlQuery ExecuteQuery( const QString& sql,
                          const QVariantList& params ) const;

  /**
   */
  QSqlQuery BatchQuery( const QString& sql,
                        const QVariantList& values );

  /**
   */
  QSqlQuery BatchQuery( const QString& sql,
                        const QVariantList& values1,
                        const QVariantList& values2 );

  /**
   */
  QSqlQuery BatchQuery( const QString& sql,
                        const QVariantList& values1,
                        const QVariantList& values2,
                        const QVariantList& values3 );

  /**
   */
  void ExecuteQuery( QSqlQuery& query );

  /**
   */
  QSqlQuery ExecuteSelectQuery( SqlQueriesSelect queryId,
                                const QVariantList& params ) const;

  /**
   */
  SqlId FindTagIdByLabel( const QString& label ) const;

//
// Private attributes.
private:
  /**
   */
  static int m_InstanceCount;

  /**
   */
  static const char* TAG_NAMES[ DatabaseConnection::TAG_NAMES_COUNT ];

  /**
   */
  QSqlDatabase m_SqlDatabase;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

/*****************************************************************************/
inline
void
DatabaseConnection
::AddBindValue( QSqlQuery& query,
                const QVariantList& values1 )
{
  query.addBindValue( values1 );
}

/*****************************************************************************/
inline
void
DatabaseConnection
::AddBindValue( QSqlQuery& query,
                const QVariantList& values1,
                const QVariantList& values2 )
{
  query.addBindValue( values1 );
  query.addBindValue( values2 );
}

/*****************************************************************************/
inline
void
DatabaseConnection
::AddBindValue( QSqlQuery& query,
                const QVariantList& values1,
                const QVariantList& values2,
                const QVariantList& values3 )
{
  query.addBindValue( values1 );
  query.addBindValue( values2 );
  query.addBindValue( values3 );
}

} // end namespace 'mvd'

#endif // __mvdDatabaseConnection_h
