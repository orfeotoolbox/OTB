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

#include "mvdDatabaseModel.h"


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
#include "mvdDatasetModel.h"
#include "mvdI18nCoreApplication.h"

namespace mvd
{
/*
  TRANSLATOR mvd::DatabaseModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatabaseModel
::DatabaseModel( QObject* parent ) :
  AbstractModel( parent ),
  m_DatasetModels(),
  m_SelectedDatasetModel( NULL )
{
}

/*******************************************************************************/
DatabaseModel
::~DatabaseModel()
{
}

/*******************************************************************************/
QStringList
DatabaseModel
::ListAvailableDatasets() const
{
  QDir cacheDir( I18nCoreApplication::ConstInstance()->GetCacheDir() );

  QStringList nameFilters;
  nameFilters << QString( "*%1" ).arg( I18nCoreApplication::DATASET_EXT );

  return cacheDir.entryList(
    nameFilters,
    QDir::Dirs | QDir::NoDotAndDotDot,
    QDir::Name
    );
}

/*******************************************************************************/
StringPairListType
DatabaseModel
::QueryDatasetModels() const
{
  StringPairListType   list;

  // need to send a list of pairs <alias, id> to know what is the
  // corresponding model to each alias in the WidgetTree
  DatasetModelMap::const_iterator it =  m_DatasetModels.begin();
  while( it != m_DatasetModels.end() )
    {
    // store alias <-> name
    StringPairType spair;
    spair.first = it.value()->GetAlias();
    spair.second = it.value()->GetName();

    // insert the pair in the list
    list.append( spair);
    ++it;
    }
  
  return list;
}

/*******************************************************************************/
DatasetModel*
DatabaseModel
::SelectDatasetModel( const DatasetId& id )
{
  // qDebug() << this << "::SelectDatasetModel(" << id << ")";

  // Find dataset model or interrupt by exception.
  DatasetModel* datasetModel = FindDatasetModel( id );
  assert( datasetModel!=NULL );

  try
    {
    // Load dataset sub-models.
    datasetModel->LoadImageModels( -1, -1 );

    // If dataset model has been loaded, select it.
    SetSelectedDatasetModel( datasetModel );
    }
  catch( std::exception& exc )
    {
    throw exc;
    }

  // Return loaded and selected dataset model.
  return datasetModel;
}

/*******************************************************************************/
void
DatabaseModel
::RemoveDatasetModel( const DatasetId& id )
{
  ReleaseDatasetModel( id, true );

  QFileInfo finfo( I18nCoreApplication::ConstInstance()->GetCacheDir(), id );
  QDir dir( finfo.filePath() );
  QFileInfoList fileInfos(
    dir.entryInfoList( QDir::NoDotAndDotDot | QDir::Files )
  );

  for( QFileInfoList::const_iterator it( fileInfos.begin() );
       it!=fileInfos.end();
       ++it )
    {
    if( !dir.remove( it->fileName() ) )
      throw SystemError(
	ToStdString(
	  tr( "Failed to remove file '%1'." ).arg( it->filePath() )
	)
      );
    }

  QDir parentDir( dir );

  if( !parentDir.cdUp() )
    throw SystemError(
      ToStdString(
	tr( "Failed to access parent directory of '%1'." ).arg( dir.path() )
      )
    );

  if( !parentDir.rmdir( id  ) )
    throw SystemError(
      ToStdString(
	tr( "Failed to remove dataset directory '%1'." ).arg( dir.path() )
      )
    );
}

/*******************************************************************************/
DatabaseModel::DatasetId
DatabaseModel
::RegisterDatasetModel( DatasetModel* model )
{
  assert( model!=NULL );

  // Construct DatasetId.
  DatasetId id( model->GetName() );

  // Find possible previously registerd dataset-model...
  DatasetModelMap::iterator it( m_DatasetModels.find( id ) );

  if( it!=m_DatasetModels.end() )
    {
    // BUGFIX: Clear selected dataset-model if it is to be deleted below.
    if( it.value()==m_SelectedDatasetModel )
      {
      SetSelectedDatasetModel( NULL );
      }

    // ...and destroy it.
    delete it.value();
    it.value() = NULL;
    }

  // Insert new dataset-model...
  it = m_DatasetModels.insert( id, model );

  // Re-parent dataset-model.
  model->setParent( this );

  // Signal model has changed.
  emit DatabaseChanged();

  // Return registered ID.
  return id;
}

/*******************************************************************************/
void
DatabaseModel
::ReleaseDatasetModel( const DatasetId& id, bool remove )
{
  qDebug() << this << "::ReleaseDatasetModel(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( DatasetModelIterator( id ) );

  // Clear selected dataset-model if it is to be deleted below.
  if( it.value()==m_SelectedDatasetModel )
    {
    SetSelectedDatasetModel( NULL );
    }

  // Release dataset-model.
  delete it.value();
  it.value() = NULL;

  // Remove dataset-model.
  if( remove )
    {
    // Erase it from registered list.
    m_DatasetModels.erase( it );

    // Safety reset.
    it = m_DatasetModels.end();

    // Signal database content has changed.
    emit DatabaseChanged();
    }
}

/*******************************************************************************/
DatasetModel*
DatabaseModel
::NewDatasetModel( const DatasetId& id )
{
  // qDebug() << this << "::NewDatasetModel(" << id << ")";

  // Find dataset model or interrupt by exception.
  DatasetModel* datasetModel = NULL;

  // Otherwise, load it from disk.
  try
    {
    // Create empty dataset model.
    datasetModel = new DatasetModel( this );

    // Build dataset model.
    assert( I18nCoreApplication::ConstInstance() );

    DatasetModel::BuildContext context(
      I18nCoreApplication::ConstInstance()->GetCacheDir().path(),
      id,
      id,
      false
    );

    datasetModel->BuildModel( &context );
    }

  catch( std::exception& exc )
    {
    // If loading was interrupted, delete allocated memory.
    delete datasetModel;
    datasetModel = NULL;

    // And forward interrupting exception.
    throw exc;
    }

  // Return loaded and selected dataset model.
  return datasetModel;
}

/*******************************************************************************/
void
DatabaseModel
::virtual_BuildModel( void* context )
{
  InitializeDatasetModels();
}

/*******************************************************************************/
void
DatabaseModel
::InitializeDatasetModels()
{
  QStringList datasets( ListAvailableDatasets() );

  ClearDatasetModels();

  for( QStringList::const_iterator it( datasets.begin() );
       it!=datasets.end();
       ++it )
    {
    try
      {
      DatasetModel* datasetModel = NewDatasetModel( *it );
      assert( datasetModel!=NULL );

      // by default alias == name
      m_DatasetModels.insert( datasetModel->GetName(), datasetModel );
      }
    catch( std::exception& exc )
      {
      throw exc;
      }
    }
}

/*******************************************************************************/
void
DatabaseModel
::ClearDatasetModels()
{
  while( !m_DatasetModels.empty() )
    {
    DatasetModelMap::iterator it( m_DatasetModels.begin() );

    delete it.value();
    it.value() = NULL;

    it = m_DatasetModels.erase( it );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/
void
DatabaseModel
::OnDatasetRenamed( const QString&  alias, const QString & id)
{
  // get the model relative to the previous key
  DatasetModel* datasetModel = FindDatasetModel(id);
  
  // update the alias
  datasetModel->SetAlias( alias );
}

} // end namespace 'mvd'
