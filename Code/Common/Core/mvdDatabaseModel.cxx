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
::QueryDatasetModels() const
{
  QDir cacheDir( I18nApplication::ConstInstance()->GetCacheDir() );

  QStringList nameFilters;
  nameFilters << QString( "*%1" ).arg( I18nApplication::DATASET_EXT );

  /*
  cacheDir.setNameFilters( nameFilters );
  cacheDir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
  cacheDir.setSorting( QDir::Name );
  */

  return cacheDir.entryList(
    nameFilters,
    QDir::Dirs | QDir::NoDotAndDotDot,
    QDir::Name
  );
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
void
DatabaseModel
::RegisterDatasetModel( DatasetModel* model )
{
  qDebug() << this << "::RegisterDatasetModel(" << model << ")";

  assert( model!=NULL );

  // Construct DatasetId.
  DatasetId id( model->GetName() );

  // Find possible previously registerd dataset-model...
  DatasetModelMap::iterator it( DatasetModelIterator( id ) );
  if( it!=m_DatasetModels.end() )
    {
    // ...and destroy it.
    delete it.value();
    it.value() = NULL;
    }

  // Insert new dataset-model...
  it = m_DatasetModels.insert( id, model );
}

/*******************************************************************************/
void
DatabaseModel
::ReleaseDatasetModel( const DatasetId& id )
{
  qDebug() << this << "::ReleaseDatasetModel(" << id << ")";

  // Find (key, value) pair.
  DatasetModelMap::iterator it( DatasetModelIterator( id ) );

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

/**************************************************************************************************************************************************************/

/*******************************************************************************/
void
DatabaseModel
::InitializeDatasetModels()
{
  QStringList datasets( QueryDatasetModels() );

  ClearDatasetModels();

  for( QStringList::const_iterator it( datasets.begin() );
       it!=datasets.end();
       ++it )
    {
    try
      {
      DatasetModel* datasetModel = NewDatasetModel( *it );
      assert( datasetModel!=NULL );

      m_DatasetModels.insert( *it, datasetModel );
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

    // get the full path to the dataset dir
    QDir datasetQDir( I18nApplication::Instance()->GetCacheDir().absolutePath() + "/" + id );
    QString datasetDir = datasetQDir.absolutePath();

    //
    // remove the files in this directory
    bool removingFilesStatus = true;
    QStringList fileList = datasetQDir.entryList();
    for( int i = 0; i < fileList.count(); ++i )
      {
      qDebug()<< "Removing file "<< fileList.at(i);
      removingFilesStatus &= datasetQDir.remove( fileList.at(i) );
      }

    // remove the dir
    QDir datasetQDirParent( I18nApplication::Instance()->GetCacheDir() );
    
    // if path removed successfuly : 
    //  - release dataset model
    //  - notify change to update the database browser
    if (datasetQDirParent.rmpath( datasetDir ) )
      {
      // 
      if ( FindDatasetModel( id ) == GetSelectedDatasetModel() )
        {
        // set the Tree browser to point on nothing
        emit CurrentSelectedItemDeleted();

        // release the selected dataset model
        emit AboutToChangeSelectedDatasetModel( GetSelectedDatasetModel() );

        // delete from the map of datasets 
        ReleaseDatasetModel( id );

        // set the current dataset model to null
        m_SelectedDatasetModel = NULL;
        
        // notify the selected dataset change
        emit SelectedDatasetModelChanged( NULL );
        }
      else
        {
        // delete from the map of datasets 
        ReleaseDatasetModel( id );
        }
        
      //  notify database changed
      emit DatabaseChanged();
      }
    }
}

} // end namespace 'mvd'
