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
#ifndef __mvdDatabaseQueries_h
#define __mvdDatabaseQueries_h

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
#include "Core/mvdTypes.h"


/*****************************************************************************/
/* MACROS                                                                    */


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

/*****************************************************************************/
/* FUNCTIONS DEFINITION SECTION                                              */

/**
 */
inline
SqlId
GetRootNodeFields( const QSqlQuery& query,
                   QString* label =NULL,
                   SqlId* parentId =NULL,
                   SqlId* tagId =NULL,
                   int* level =NULL,
                   QString* path =NULL );

/**
 */
SqlId
GetNodeFields( const QSqlQuery& query,
               QString* label =NULL,
               SqlId* parentId =NULL,
               SqlId* tagId =NULL,
               int* level =NULL,
               QString* path =NULL );

/**
 */
SqlId
GetChildNodeFields( const QSqlQuery& query,
                    QString* label =NULL,
                    SqlId* parentId =NULL,
                    SqlId* tagId =NULL,
                    int* level =NULL,
                    QString* path =NULL );

/**
 */
SqlId
GetDatasetFields( const QSqlQuery& query,
                  QString* hash =NULL,
                  QString* alias =NULL );

//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DatabaseQueries
 *
 * \brief WIP.
 */
class Monteverdi2_EXPORT DatabaseQueries :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

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

//
// Private methods.
private:

  /** \brief Constructor. */
  DatabaseQueries( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~DatabaseQueries();

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
SqlId
GetRootNodeFields( const QSqlQuery& query,
                   QString* label,
                   SqlId* parentId,
                   SqlId* tagId,
                   int* level,
                   QString* path )
{
  return GetNodeFields( query, label, parentId, tagId, level, path );
}

} // end namespace 'mvd'

#endif // __mvdDatabaseQueries_h
