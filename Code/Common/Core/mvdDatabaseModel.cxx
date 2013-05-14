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
#include "mvdI18nApplication.h"
#include "mvdVectorImageModel.h"

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
  qDebug() << this << " ::ListAvailableDatasets( )";
  
  QDir cacheDir( I18nApplication::ConstInstance()->GetCacheDir() );

  QStringList nameFilters;
  nameFilters << QString( "*%1" ).arg( I18nApplication::DATASET_EXT );

  return cacheDir.entryList(
    nameFilters,
    QDir::Dirs | QDir::NoDotAndDotDot,
    QDir::Name
    );
}

/*******************************************************************************/
QStringList
DatabaseModel
::QueryDatasetModels() const
{
  qDebug() << this 
           << " ::QueryDatasetModels( Available Datasets keys: "
           << m_DatasetModels.keys() <<") ";
  
  return m_DatasetModels.keys();
}

/*******************************************************************************/
DatasetModel*
DatabaseModel
::SelectDatasetModel( const DatasetId& id )
{
  qDebug() << this << "::SelectDatasetModel(" << id << ")";

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
DatabaseModel::DatasetId
DatabaseModel
::RegisterDatasetModel( DatasetModel* model )
{
  assert( model!=NULL );

  // Construct DatasetId.
  DatasetId id( model->GetAlias() );

  // Find possible previously registerd dataset-model...
  DatasetModelMap::iterator it( m_DatasetModels.find( id ) );
  if( it!=m_DatasetModels.end() )
    {
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
::ReleaseDatasetModel( const DatasetId& id )
{
  qDebug() << this << "::ReleaseDatasetModel(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( DatasetModelIterator( id ) );

  // release model
  delete it.value();
  it.value() = NULL;
}

/*******************************************************************************/
DatasetModel*
DatabaseModel
::NewDatasetModel( const DatasetId& id )
{
  qDebug() << this << "::NewDatasetModel(" << id << ")";

  // Find dataset model or interrupt by exception.
  DatasetModel* datasetModel = NULL;

  // Otherwise, load it from disk.
  try
    {
    // Create empty dataset model.
    datasetModel = new DatasetModel( this );

    // Build dataset model.
    assert( I18nApplication::ConstInstance() );

    DatasetModel::BuildContext context(
      I18nApplication::ConstInstance()->GetCacheDir().path(),
      id,
      id,
      true // TODO: is changing to true decrease performances ??
    );

    datasetModel->BuildModel( &context );

    // setup alias
    VectorImageModel * viModel =
       			dynamic_cast<VectorImageModel*> (datasetModel->GetSelectedImageModel());
    QFileInfo finfo( viModel->GetFilename() );
    datasetModel->SetAlias( finfo.baseName() );
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
      m_DatasetModels.insert( datasetModel->GetAlias(), datasetModel );
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
void
DatabaseModel
::OnDatasetToDeleteSelected( const QString&  id)
{
  // pop up a Dialog widget to confirm the user choice
  QMessageBox msgBox;
  msgBox.setWindowTitle( tr( "Warning") );
  msgBox.setText(tr("You are about to remove the dataset : \n %1 \n\n"
                    " Are your sure ? ").arg(id) );
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  
  // if choice confirmed
  if (ret == QMessageBox::Yes)
    {
    qDebug() << this << " ::OnDatasetToDeleteSelected -> About to remove : "<< id;

    // if current selected item is removed, make the TreeWidget
    // focusing on the root item
    if ( FindDatasetModel( id ) == GetSelectedDatasetModel() )
      {
      // set the Tree browser to point on nothing
      emit CurrentSelectedItemDeleted();

      //
      SetSelectedDatasetModel( NULL );
      }

    // get the full path to the dataset dir
    QDir datasetQDir( 
      I18nApplication::Instance()->GetCacheDir().absolutePath() + 
      "/" + 
      FindDatasetModel( id )->GetName() 
      );
    QString datasetDir = datasetQDir.absolutePath();

    //
    // remove the files in this directory
    bool removingFilesStatus = true;
    QStringList fileList = datasetQDir.entryList( QDir::NoDotAndDotDot |  QDir::Files );
    for( int i = 0; i < fileList.count(); ++i )
      {
      qDebug()<< "Removing file "<< fileList.at(i);
      removingFilesStatus &= datasetQDir.remove( fileList.at(i) );
      }
    
    // go to parent dir
    QDir datasetQDirParent = datasetQDir;
    datasetQDirParent.cdUp();
    
    // if path removed successfuly : 
    //  - release dataset model
    //  - notify change to update the database browser
    if ( removingFilesStatus &&  datasetQDirParent.rmpath( datasetDir ) )
      {
      qDebug() << this 
               << " ::OnDatasetToDeleteSelected -> Removing : "
               << datasetQDir.absolutePath();
      
      // release the model relative to 'id'
      ReleaseDatasetModel( id );

      // remove from the map the key
      m_DatasetModels.remove( id );
      
      //  notify database changed
      emit DatabaseChanged();
      }

    // rmpath() removes the parent directory (i.e mvd2 ) when no files
    // (datasets ) in it ->  recreate "mvd2" if removed
    // TODO : find a better solution to replace this hack
    if (! datasetQDirParent.exists() )
      {
      qDebug() << this<< "::OnDatasetToDeleteSelected Making removed dir "
               << datasetQDirParent.absolutePath();
      datasetQDirParent.mkpath( datasetQDirParent.absolutePath() );
      }
    }
}

/*******************************************************************************/
void
DatabaseModel
::OnDatasetRenamed( const QString&  previous, const QString & current)
{
  // get the model relative to the previous key
  DatasetModel * datasetModel = FindDatasetModel(previous);
  
  // create a new entry in the map
  m_DatasetModels.insert(current, datasetModel);
  
  // remove the old key in the map
  m_DatasetModels.remove(previous);
  
  // update the alias
  datasetModel->SetAlias( current );
}

} // end namespace 'mvd'
