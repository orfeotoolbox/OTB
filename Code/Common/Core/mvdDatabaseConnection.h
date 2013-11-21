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
// Public methods.
public:

  /** \brief Constructor. */
  DatabaseConnection( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~DatabaseConnection();

  /**
   */
  static void InitializeDatabase();
  static void InitializeDatabase2();

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
// Private methods.
private:

  /**
   */
  inline static QSqlDatabase SqlDatabase();

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
  void ExecuteQuery( const QString& sql );

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
  // QSqlQuery BatchQuery( const QString& sql, const QVariantList& values1, ... );

  /**
   */
  void ExecuteQuery( QSqlQuery& query );

  /**
   */
  void TransactQuery( QSqlQuery& query );

//
// Private attributes.
private:
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
