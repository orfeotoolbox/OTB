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
  m_DatasetModels()
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
  DatasetModel* datasetModel = FindDatasetModel( id );

  if( datasetModel!=NULL )
    return datasetModel;

  return NULL;
}

/*******************************************************************************/
void
DatabaseModel
::ReleaseDatasetModel( const DatasetId& id )
{
}

/*****************************************************************************/
DatasetModel*
DatabaseModel
::FindDatasetModel( const DatasetId& id )
{
  // Find (key, value) pair.
  DatasetModelMap::iterator it( m_DatasetModels.find( id ) );

  // Should be present because it should have been initialized in
  // InitializeDatasetModels().
  assert( it!=m_DatasetModels.end() );

  // But, in case of, safe return value.
  return it==m_DatasetModels.end() ? NULL : it.value();
}

/*******************************************************************************/
void
DatabaseModel
::virtual_BuildModel( void* context )
{
}

#if 0

/*******************************************************************************/
bool
DatabaseModel
::IsModified() const
{
  return true;
}

/*******************************************************************************/

void
DatabaseModel
::ClearModified()
{
}

#endif

/*******************************************************************************/
#if 0

void
DatabaseModel
::virtual_Save()
{
}

#endif

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
    m_DatasetModels.insert( *it, NULL );
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

} // end namespace 'mvd'
