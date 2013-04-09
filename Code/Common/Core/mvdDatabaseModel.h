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

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModel.h"


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
   */
  typedef QString DatasetId;

  /**
   * \class BuildContext
   * \brief WIP.
   */
  class BuildContext
  {
    //
    // Public methods.
  public:
    BuildContext()
    {
    }

    //
    // Public attributes
  public:
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
  QStringList QueryDatasetModels() const;

  /**
   */
  inline DatasetModel* GetSelectedDatasetModel();

  /**
   */
  DatasetModel* SelectDatasetModel( const DatasetId& id );

  /**
   */
  void ReleaseDatasetModel( const DatasetId& id );

  //
  // AbstractModel overrides.

#if 0
  bool IsModified() const;

  void ClearModified();
#endif

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void AboutToChangeSelectedDatasetModel( const DatasetModel*  );

  /**
   */
  void SelectedDatasetModelChanged( DatasetModel* );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModel overrides.

  void virtual_BuildModel( void* context =NULL );

#if 0
  void virtual_Save();
#endif

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:
  /**
   */
  typedef QMap< DatasetId, DatasetModel* > DatasetModelMap;

//
// Private methods.
private:
  /**
   */
  void InitializeDatasetModels();

  /**
   */
  void ClearDatasetModels();

  /**
   */
  DatasetModel* FindDatasetModel( const DatasetId& id );

  /**
   */
  inline
    DatasetModelMap::const_iterator
    DatasetModelIterator( const DatasetId& id ) const;

  /**
   */
  inline
    DatasetModelMap::iterator
    DatasetModelIterator( const DatasetId& id );

  /**
   */
  void SetSelectedDatasetModel( const DatasetId& id );

  /**
   */
  void SetSelectedDatasetModel( DatasetModel* model );

  /**
   */
  DatasetModel* NewDatasetModel( const DatasetId& id );

//
// Private attributes.
private:
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
::SetSelectedDatasetModel( const DatasetId& id )
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
DatasetModel*
DatabaseModel
::FindDatasetModel( const DatasetId& id )
{
  qDebug() << this << "::FindDatasetModel(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( DatasetModelIterator( id ) );

  // Return found element.
  return it.value();
}

/*****************************************************************************/
inline
DatabaseModel::DatasetModelMap::const_iterator
DatabaseModel
::DatasetModelIterator( const QString& id ) const
{
  qDebug() << this << "::DatasetModelIterator(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::const_iterator it( m_DatasetModels.find( id ) );

  // Should be present because it should have been initialized in
  // InitializeDatasetModels().
  if( it==m_DatasetModels.end() )
    throw std::out_of_range( ToStdString( id ) );

  // Return found element.
  return it;
}

/*****************************************************************************/
inline
DatabaseModel::DatasetModelMap::iterator
DatabaseModel
::DatasetModelIterator( const QString& id )
{
  qDebug() << this << "::DatasetModelIterator(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( m_DatasetModels.find( id ) );

  // Should be present because it should have been initialized in
  // InitializeDatasetModels().
  if( it==m_DatasetModels.end() )
    throw std::out_of_range( ToStdString( id ) );

  // Return found element.
  return it;
}

} // end namespace 'mvd'

#endif // __mvdDatabaseModel_h
