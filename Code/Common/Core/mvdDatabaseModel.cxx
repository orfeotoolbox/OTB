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
#include "Core/mvdDatabaseConnection.h"
#include "Core/mvdDatasetModel.h"
#include "Core/mvdI18nCoreApplication.h"

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
  m_Db( NULL ),
  m_DatasetModels(),
  m_SelectedDatasetModel( NULL )
{
}

/*******************************************************************************/
DatabaseModel
::~DatabaseModel()
{
  delete m_Db;
  m_Db = NULL;
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
  StringPairListType list;

  // need to send a list of pairs <alias, id> to know what is the
  // corresponding model to each alias in the WidgetTree
  DatasetModelMap::const_iterator it =  m_DatasetModels.begin();
  while( it != m_DatasetModels.end() )
    {
    // store alias <-> name
    StringPairType spair;
    spair.first = it.value()->GetAlias();
    spair.second = it.value()->GetHash();

    // insert the pair in the list
    list.append( spair);
    ++it;
    }
  
  return list;
}

/*******************************************************************************/
DatasetModel*
DatabaseModel
::SelectDatasetModel( const DatasetHash& id )
{
  // qDebug() << this << "::SelectDatasetModel(" << id << ")";

  // Find dataset model or interrupt by exception.
  DatasetModel* datasetModel = FindDatasetModel( id );
  assert( datasetModel!=NULL );

  // Optimize select by only returning already selected
  // dataset-model.
  if( datasetModel==m_SelectedDatasetModel )
    return datasetModel;

#if 1
  // Release memory from previous model
  if( m_SelectedDatasetModel!=NULL )
    {
    m_SelectedDatasetModel->GetSelectedImageModel()->ReleaseMemory();
    }

  try
    {
    // Load dataset sub-models.
    datasetModel->LoadImageModels( -1, -1 );

    // If dataset model has been loaded, select it.
    SetSelectedDatasetModel( datasetModel );
    }
  catch( ... )
    {
    // If dataset model has not beed successfully loaded, clear
    // selection...
    SetSelectedDatasetModel( NULL );

    // ...and forward exception.
    throw;
    }

#else
  // Load dataset sub-models.
  datasetModel->LoadImageModels( -1, -1 );

  // Release memory from previous model
  if( m_SelectedDatasetModel!=NULL )
    {
    m_SelectedDatasetModel->GetSelectedImageModel()->ReleaseMemory();
    }

  // If dataset model has been loaded, select it.
  SetSelectedDatasetModel( datasetModel );
#endif

  // Return loaded and selected dataset model.
  return datasetModel;
}

/*******************************************************************************/
void
DatabaseModel
::RemoveDatasetModel( const DatasetHash& hash )
{
  // Release memory resources.
  ReleaseDatasetModel( hash, true );

  // Delete files.
  I18nCoreApplication::DeleteDatasetModel( hash );
}

/*******************************************************************************/
DatasetHash
DatabaseModel
::RegisterDatasetModel( DatasetModel* model )
{
  assert( model!=NULL );

  // Construct DatasetHash.
  DatasetHash hash( model->GetHash() );

  // Find possible previously registerd dataset-model...
  DatasetModelMap::iterator it( m_DatasetModels.find( hash ) );

  if( it==m_DatasetModels.end() )
    {
    assert( m_Db!=NULL );
    m_Db->InsertDataset( hash, model->GetAlias() );
    }
  else
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
  it = m_DatasetModels.insert( hash, model );

  // Re-parent dataset-model.
  model->setParent( this );

  // Signal model has changed.
  emit DatabaseChanged();

  // Return registered ID.
  return hash;
}

/*******************************************************************************/
void
DatabaseModel
::ReleaseDatasetModel( const DatasetHash& hash, bool remove )
{
  // qDebug() << this << "::ReleaseDatasetModel(" << hash << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( DatasetModelIterator( hash ) );

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

    // Delete dataset from database.
    assert( m_Db!=NULL );
    m_Db->DeleteDataset( hash );

    // Signal database content has changed.
    emit DatabaseChanged();
    }
}

/*******************************************************************************/
DatasetModel*
DatabaseModel
::NewDatasetModel( const DatasetHash& id )
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
      I18nCoreApplication::ConstInstance()->GetCacheDir().path(), id );

    datasetModel->BuildModel( &context );
    }

  catch( ... )
    {
    // If loading was interrupted, delete allocated memory.
    delete datasetModel;
    datasetModel = NULL;

    // And forward interrupting exception.
    throw;
    }

  // Return loaded and selected dataset model.
  return datasetModel;
}

/*******************************************************************************/
void
DatabaseModel
::virtual_BuildModel( void* context )
{
  assert( m_Db==NULL );

  delete m_Db;
  m_Db = new DatabaseConnection( this );

  CountType nbOutdated = InitializeDatasetModels();

  if( context!=NULL )
    {
    BuildContext* buildContext = static_cast< BuildContext* >( context );

    buildContext->m_NbOutdatedDatasetModels = nbOutdated;
    }

  if( nbOutdated>0 )
    {
    qWarning() <<
      ToString(
        tr( "%1 outdated datasets. Please clear cache directory." )
        .arg( nbOutdated ) );
    }
}

/*******************************************************************************/
CountType
DatabaseModel
::InitializeDatasetModels()
{
  assert( m_Db );

  // (key: ID; value: hash) dataset-map.
  DatabaseConnection::DatasetMap datasets(
    m_Db->ListAllDatasets()
  );

  qDebug() << datasets;

  ClearDatasetModels();

  CountType nbOutdated = 0;

  for( DatabaseConnection::DatasetMap::const_iterator it( datasets.begin() );
       it!=datasets.end();
       ++it )
    {
    if( DatasetModel::IsVersionCompliant(
          I18nCoreApplication::ConstInstance()->GetCacheDir().path(),
          it.value() ) )
      {
      DatasetModel* datasetModel = NewDatasetModel( it.value() );
      assert( datasetModel!=NULL );

      // by default alias == name
      m_DatasetModels.insert( datasetModel->GetHash(), datasetModel );
      }
    else
      {
      qWarning() << "Dataset '" << it.value() << "' is outdated.";

      ++ nbOutdated;
      }
    }

  return nbOutdated;
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
::OnDatasetRenamed( const QString& alias, const QString& hash )
{
  // Get the model relative to the given hash key.
  DatasetModelMap::const_iterator it( DatasetModelIterator( hash ) );

  // Update alias in memory and in XML descriptor.
  assert( it.value()!=NULL );
  it.value()->SetAlias( alias );

  // Update alias in database .
  assert( m_Db );
  m_Db->UpdateDatasetAliasFromHash( hash, alias );
  // qDebug() << "Updated:" << hash << alias;
}

} // end namespace 'mvd'
