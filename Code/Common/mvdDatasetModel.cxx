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
void
DatasetModel
::ImportImage( const QString& filename, int width, int height )
{
  LoadImage( filename, width, height, -1, QString() );
}

/*******************************************************************************/
AbstractImageModel*
DatasetModel
::LoadImage( const QString& filename,
	     int width,
	     int height,
	     int id,
	     const QString& quicklook )
{
  // Check input parameters.
  assert( (id<0 && quicklook.isEmpty()) ||
	  (id>=0  && !quicklook.isEmpty()) );

  // 1. Instanciate local image model.
  VectorImageModel* vectorImageModel = new VectorImageModel( this );

  // 2. Safely load data from file.
  try
    {
    //
    // 2.1. Fill-in image-model build-contex with provided id (which
    // may be -1 if first-time import of image-model.
    AbstractImageModel::BuildContext context( filename, id, quicklook );
    // Assign image-model ID to build-context if there is none
    // provided but keep provided one to test if it's first time
    // import or next time loading of image-model.
    if( id<0 )
      {
      AbstractImageModelList aimList( GetImageModels() );
      context.m_Id = aimList.indexOf( vectorImageModel );

      qDebug()
	<< "Generated ID" << context.m_Id
	<< "for image-file " << context.m_Filename << ".";
      }

    //
    // 2.2. Set image-model content.
    // TODO: SetFilename() into VectorImageModel::virtual_BuildModel().
    vectorImageModel->SetFilename( context.m_Filename, width, height );


    //
    // 2.3. Build image-model structure and generate cached data).
    vectorImageModel->BuildModel( &context );
    assert( vectorImageModel->GetQuicklookModel()!=NULL );

    // If it's first time, import image-model into descriptor.
    if( id<0 )
      {
      //
      // 2.4a: Add image to Dataset descriptor file...
      m_Descriptor->InsertImageModel(
	// ...providing newly calculated image-model ID.
	context.m_Id,
	vectorImageModel->GetFilename(),
	&vectorImageModel->GetSettings(),
	vectorImageModel->GetQuicklookModel()->GetFilename()
      );

      //
      // 2.5a: Force writing descriptor with newly imported image.
      WriteDescriptor();
      }

    //
    // 2.6
    // Connect rendering-settings updated of image-model to
    // dataset-model in order to update XML descriptor.
    QObject::connect(
      vectorImageModel,
      SIGNAL( SettingsUpdated( AbstractImageModel* ) ),
      // to:
      this,
      SLOT( OnSettingsUpdated( AbstractImageModel* ) )
    );
    }
  catch( std::exception& exc )
    {
    // Release local memory allocation.
    delete vectorImageModel;
    vectorImageModel = NULL;

    // Forward exception to upper level (GUI).
    throw;
    }

  return vectorImageModel;
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

  // Initialize content.
  if( isEmpty )
    {
    // Create in-memory descriptor.
    assert( m_Descriptor==NULL );
    m_Descriptor = newChildModel< DatasetDescriptor >();

    // Force writing in-memory descriptor into disk in order to have a
    // consistent empty dataset directory structure.
    WriteDescriptor();
    }
  else
    {
    // Create context with filename.
    QFileInfo finfo( m_Directory, DatasetModel::DESCRIPTOR_FILENAME );
    DatasetDescriptor::BuildContext context( finfo.filePath() );

    // Relink to on-disk descriptor.
    assert( m_Descriptor==NULL );
    m_Descriptor = newChildModel< DatasetDescriptor >( &context );

    // Load image-models from descriptor.
    // TODO: Replace DatasetModel::BuildContext() by (width, height).
    ParseDescriptor(buildContext);
    }
}

/*******************************************************************************/
void
DatasetModel
::ParseDescriptor( BuildContext* context )
{
  assert( context );

  for( QDomElement imageElt( m_Descriptor->FirstImageElement() );
       !imageElt.isNull();
       imageElt = DatasetDescriptor::NextImageSiblingElement( imageElt ) )
    {
    // Locals.
    int id = -1;
    QString filename;
    QString quicklook;
    VectorImageModel::Settings settings;

    // Read image-model descriptor information.
    DatasetDescriptor::GetImageModel(
      imageElt,
      id, filename, &settings, quicklook
    );

    // Traces.
    qDebug()
      << "Input image:"
      << "\nID:" << id
      << "\nfilename:" << filename
      << "\nquicklook:" << quicklook;

    // TODO: 3) Remove WxH for screen best-fit during loading of model!
    AbstractImageModel* imageModel = LoadImage(
      filename,
      context->m_Width, context->m_Height,
      id,
      quicklook
    );

    // Access vector image-model.
    VectorImageModel* vectorImageModel =
      qobject_cast< VectorImageModel* >( imageModel );
    assert( vectorImageModel!=NULL );

    // Re-assign rendering-settings to image-model.
    vectorImageModel->SetSettings( settings );
    }
}

/*******************************************************************************/
void
DatasetModel
::WriteDescriptor() const
{
  QFileInfo finfo( m_Directory, DatasetModel::DESCRIPTOR_FILENAME );

  assert( m_Descriptor!=NULL );
  m_Descriptor->Write( finfo.filePath() );
}

/*******************************************************************************/
bool
DatasetModel
::IsModified() const
{
  ConstAbstractImageModelList aimList( GetImageModels() );

  // If, at least one image-model is modified, then this dataset-model
  // is considered modified.
  for( ConstAbstractImageModelList::const_iterator it( aimList.begin() );
       it!=aimList.end();
       ++it )
    {
    if( ( *it )->IsModified() )
      return true;
    }

  // Otherwise, this dataset-model is not modified.
  return false;
}

/*******************************************************************************/
void
DatasetModel
::Save() const
{
  WriteDescriptor();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/
void
DatasetModel
::OnSettingsUpdated( AbstractImageModel* imageModel )
{
  assert( imageModel!=NULL );

  qDebug() << "DatasetModel::OnSettingsUpdated(" << imageModel->GetId() << ")";

  VectorImageModel* vectorImageModel =
    qobject_cast< VectorImageModel* >( imageModel );

  assert( vectorImageModel!=NULL );

  m_Descriptor->SetImageModel(
    vectorImageModel->GetId(),
    &vectorImageModel->GetSettings()
  );
}

} // end namespace 'mvd'
