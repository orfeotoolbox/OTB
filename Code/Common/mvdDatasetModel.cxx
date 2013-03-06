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

#include "mvdDatasetModel.h"


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
#include "mvdDatasetDescriptor.h"
#include "mvdI18nApplication.h"
#include "mvdQuicklookModel.h"
#include "mvdSystemError.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::DatasetModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* DatasetModel::DESCRIPTOR_FILENAME = "descriptor.xml";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatasetModel
::DatasetModel( QObject* parent ) :
  AbstractModel( parent ),
  m_Descriptor( NULL ),
  m_Path(),
  m_Name(),
  m_Directory()
{
}

/*******************************************************************************/
DatasetModel
::~DatasetModel()
{
}

/*******************************************************************************/
#if 0
bool
DatasetModel
::SetContent( const QString& path, const QString& name )
{
  return true;
}
#endif

/*******************************************************************************/
void
DatasetModel
::ImportImage( const QString& filename , int w, int h )
{
  // 1. Instanciate local image model.
  VectorImageModel* vectorImageModel = new VectorImageModel( this );

  // 2. Safely load data from file.
  try
    {
    //
    // 2.1. Information.
    vectorImageModel->SetFilename( filename, w, h );

    //
    // 2.2. Generate cached data.
    // TODO: generate image-model cached data (quicklook,
    // histogram-list etc.)
    vectorImageModel->BuildModel();

    AbstractImageModelList aimList( GetImageModels() );
    qDebug() << aimList;

    //
    // 2.3: Add image to Dataset descriptor file.
    assert( vectorImageModel->GetQuicklookModel()!=NULL );

    m_Descriptor->InsertImageModel(
      GetImageModels().size(),
      vectorImageModel->GetFilename(),
      &vectorImageModel->GetSettings(),
      vectorImageModel->GetQuicklookModel()->GetFilename()
    );

    WriteDescriptor();
    }
  catch( std::exception& exc )
    {
    // Release local memory allocation.
    delete vectorImageModel;
    vectorImageModel = NULL;

    // Forward exception to upper level (GUI).
    throw;
    }

  // If everything has gone well, parent image model to dataset model.
  vectorImageModel->setParent( this );
}

/*******************************************************************************/
void
DatasetModel
::virtual_BuildModel( void* context )
{
  // Get build-context.
  assert( context!=NULL );
  BuildContext* buildContext = static_cast< BuildContext* >( context );

  // Create directory structure, if needed.
  bool isEmpty = I18nApplication::MakeDirTree(
    buildContext->m_Path,
    buildContext->m_Name
  );

  // Access working directory.
  QDir workingDir( buildContext->m_Path );
  if( !workingDir.cd( buildContext->m_Name ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( workingDir.filePath( buildContext->m_Name ) ) )
    );    

  // Remember access to directory structure.
  m_Path = buildContext->m_Path;
  m_Name = buildContext->m_Name;
  m_Directory = workingDir;

  // Add child-object of model.
  assert( m_Descriptor==NULL );
  m_Descriptor = newChildModel< DatasetDescriptor >();

  // Initialize content, if needed.
  if( true || isEmpty )
    WriteDescriptor();

  // Load directory content.
  Load( buildContext->m_Path, buildContext->m_Name );
}

/*******************************************************************************/
void
DatasetModel
::Load( const QString& path, const QString& name )
{
}

/*******************************************************************************/
void
DatasetModel
::WriteDescriptor() const
{
  QFileInfo finfo( m_Directory, DatasetModel::DESCRIPTOR_FILENAME );

  m_Descriptor->Write( finfo.filePath() );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
