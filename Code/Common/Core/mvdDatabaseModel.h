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

#ifndef __mvdDatabaseModel_h
#define __mvdDatabaseModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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
#include "mvdAbstractModel.h"
#include "mvdTypes.h"

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
class DatabaseConnection;
class DatasetModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class DatabaseModel
 *
 */
class Monteverdi2_EXPORT DatabaseModel :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

  /**
   * \class BuildContext
   * \brief WIP.
   */
  class BuildContext
  {
    //
    // Public methods.
  public:
    BuildContext() :
      m_NbOutdatedDatasetModels( 0 )
    {
    }

    //
    // Public attributes
  public:
    CountType m_NbOutdatedDatasetModels;
  };

//
// Public methods.
public:

  /** \brief Constructor. */
  DatabaseModel( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~DatabaseModel();

  /**
   */
  inline const DatabaseConnection* GetDatabaseConnection() const;

  /**
   */
  inline DatabaseConnection* GetDatabaseConnection();

  /**
   */
  StringPairListType QueryDatasetModels() const;

  /**
   */
  inline
    const DatasetModel* FindDatasetModel( const DatasetHash& id ) const;

  /**
   */
  inline
    DatasetModel* FindDatasetModel( const DatasetHash& id );

  /**
   */
  inline
    DatasetModel* GetSelectedDatasetModel();

  /**
   */
  inline
    const DatasetModel* GetSelectedDatasetModel() const;

  /**
   */
  DatasetModel* SelectDatasetModel( const DatasetHash& id );

  /**
   */
  void RemoveDatasetModel( const DatasetHash& id );

  //
  // AbstractModel overrides.

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void AboutToChangeSelectedDatasetModel( const DatasetModel*  );

  /**
   */
  void DatabaseChanged();

  /**
   */
  void SelectedDatasetModelChanged( DatasetModel* );

  /**
   */
  //void RenameDeclined(const QString&, const QString &);

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
public slots:

  /**
   */
  void OnDatasetRenamed( const QString& alias, const QString & id );

  /**
   */
  DatasetHash RegisterDatasetModel( DatasetModel* );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModel overrides.

  void virtual_BuildModel( void* context =NULL );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:
  /**
   */
  typedef QMap< DatasetHash, DatasetModel* > DatasetModelMap;

//
// Private methods.
private:
  /**
   */
  CountType InitializeDatasetModels();   

  /**
   */
  QStringList ListAvailableDatasets() const;

  /**
   */
  void ClearDatasetModels();

  /**
   */
  inline
    DatasetModelMap::const_iterator
    DatasetModelIterator( const DatasetHash& id ) const;

  /**
   */
  inline
    DatasetModelMap::iterator
    DatasetModelIterator( const DatasetHash& id );

  /**
   * \brief Find dataset identified by given key, delete its
   * instance in memory, keep it in or remove it from list of
   * registered datasets depending on the remove flag.
   *
   * \param id Key identifying dataset.
   * \param remove true to remove it from list of registered dataset,
   * false to keep it in.
   */
  void ReleaseDatasetModel( const DatasetHash& id, bool remove );

  /**
   */
  inline
    void SetSelectedDatasetModel( const DatasetHash& id );

  /**
   */
  inline
    void SetSelectedDatasetModel( DatasetModel* model );

  /**
   */
  DatasetModel* NewDatasetModel( const DatasetHash& id );

//
// Private attributes.
private:
  /**
   */
  DatabaseConnection* m_Db;

  /**
   */
  DatasetModelMap m_DatasetModels;

  /**
   */
  DatasetModel* m_SelectedDatasetModel;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// System includes (sorted by alphabetic order)
#include <stdexcept>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi deferred includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"

namespace mvd
{

/*****************************************************************************/
inline
const DatabaseConnection*
DatabaseModel
::GetDatabaseConnection() const
{
  return m_Db;
}

/*****************************************************************************/
inline
DatabaseConnection*
DatabaseModel
::GetDatabaseConnection()
{
  return m_Db;
}

/*****************************************************************************/
inline
const DatasetModel*
DatabaseModel
::GetSelectedDatasetModel() const
{
  return m_SelectedDatasetModel;
}

/*****************************************************************************/
inline
DatasetModel*
DatabaseModel
::GetSelectedDatasetModel()
{
  return m_SelectedDatasetModel;
}

/*****************************************************************************/
inline
void
DatabaseModel
::SetSelectedDatasetModel( const DatasetHash& id )
{
  SetSelectedDatasetModel( FindDatasetModel( id ) );
}

/*****************************************************************************/
inline
void
DatabaseModel
::SetSelectedDatasetModel( DatasetModel* model )
{
  emit AboutToChangeSelectedDatasetModel( model );

  m_SelectedDatasetModel = model;

  emit SelectedDatasetModelChanged( model );
}

/*****************************************************************************/
inline
const DatasetModel*
DatabaseModel
::FindDatasetModel( const DatasetHash& id ) const
{
  // qDebug() << this << "::FindDatasetModel(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::const_iterator it( DatasetModelIterator( id ) );

  // Return found element.
  return it.value();
}

/*****************************************************************************/
inline
DatasetModel*
DatabaseModel
::FindDatasetModel( const DatasetHash& id )
{
  // qDebug() << this << "::FindDatasetModel(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( DatasetModelIterator( id ) );

  // Return found element.
  return it.value();
}

/*****************************************************************************/
inline
DatabaseModel::DatasetModelMap::const_iterator
DatabaseModel
::DatasetModelIterator( const DatasetHash& id ) const
{
  // qDebug() << this << "::DatasetModelIterator(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::const_iterator it( m_DatasetModels.find( id ) );

  // Should be present because it should have been initialized in
  // InitializeDatasetModels().
  if( it==m_DatasetModels.end() )
    throw std::out_of_range(
      ToStdString( tr( "Dataset '%1' not found." ).arg( id ) )
    );

  // Return found element.
  return it;
}

/*****************************************************************************/
inline
DatabaseModel::DatasetModelMap::iterator
DatabaseModel
::DatasetModelIterator( const QString& id )
{
  // qDebug() << this << "::DatasetModelIterator(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( m_DatasetModels.find( id ) );

  // Should be present because it should have been initialized in
  // InitializeDatasetModels().
  if( it==m_DatasetModels.end() )
    throw std::out_of_range(
      ToStdString( tr( "Dataset '%1' not found!" ).arg( id ) )
    );

  // Return found element.
  return it;
}

} // end namespace 'mvd'

#endif // __mvdDatabaseModel_h
