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

const char* DatasetModel::QUICKLOOK_FILE_EXT = ".ql.tif";

const char* DatasetModel::HISTOGRAM_FILE_MARKER = ".hg";

const char* DatasetModel::HISTOGRAM_FILE_EXT = ".txt";

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
  AbstractImageModel::BuildContext context( true, filename );
  LoadImage( context, width, height );
}

/*******************************************************************************/
AbstractImageModel*
DatasetModel
::LoadImage( AbstractImageModel::BuildContext& context,
	     int width,
	     int height )
{
  // Check input parameters.
  assert( (context.m_Id<0 && context.m_Quicklook.isEmpty()) ||
	  (context.m_Id>=0  && !context.m_Quicklook.isEmpty()) );

  // 1. Instanciate local image model.
  VectorImageModel* vectorImageModel = new VectorImageModel( this );

  //
  // 1.1. Assign new image ID to build-context if first time loading.
  // Remember provided ID.
#if 0
  int id = context.m_Id;
#endif
  // Assign image-model ID to build-context if there is none
  // provided but keep provided one to test if it's first time
  // import or next time loading of image-model.
  if( context.IsBeingStored() )
    {
    AbstractImageModelList aimList( GetImageModels() );

    context.m_Id = aimList.indexOf( vectorImageModel );

    context.m_Quicklook = QuicklookFileInfo( context.m_Filename ).filePath();
    context.m_Histogram = HistogramFileInfo( context.m_Filename ).filePath();

    qDebug()
      << "Storing image:"
      << "\n- id:" << context.m_Id
      << ";\n- filename:" << context.m_Filename
      << ";\n- quicklook:" << context.m_Quicklook
      << ";\n- histogram:" << context.m_Histogram
      << ".";
    }

  // 2. Safely load data from file.
  try
    {
    //
    // 2.1. Set image-model content.
    // TODO: SetFilename() into VectorImageModel::virtual_BuildModel().
    vectorImageModel->SetFilename( context.m_Filename, width, height );

    //
    // 2.2. Build image-model structure and generate cached data).
    vectorImageModel->BuildModel( &context );

    assert( vectorImageModel->GetQuicklookModel()!=NULL );
    assert( vectorImageModel->GetHistogramModel()!=NULL );

    // If it's first time, import image-model into descriptor.
    if( context.IsBeingStored() )
      {
      //
      // 2.3a: Add image to Dataset descriptor file...
      m_Descriptor->InsertImageModel(
	// ...providing newly calculated image-model ID.
	context.m_Id,
	context.m_Filename,
	&vectorImageModel->GetSettings(),
	vectorImageModel->GetQuicklookModel()->GetFilename(),
	context.m_Histogram
      );

      //
      // 2.4a: Force writing descriptor with newly imported image.
      Save();
      }

    //
    // 2.5. Connect rendering-settings updated of image-model to
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
    ParseDescriptor( buildContext );
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
    VectorImageModel::Settings settings;
    AbstractImageModel::BuildContext imageContext( false, &settings );

    // Read image-model descriptor information.
    DatasetDescriptor::GetImageModel(
      imageElt,
      imageContext.m_Id,
      imageContext.m_Filename,
      imageContext.m_Settings,
      imageContext.m_Quicklook,
      imageContext.m_Histogram
    );

    // Traces.
    qDebug()
      << "Input image:"
      << "\n- ID:" << imageContext.m_Id
      << ";\n- filename:" << imageContext.m_Filename
      << ";\n- quicklook:" << imageContext.m_Quicklook 
      << ";\n- histogram:" << imageContext.m_Histogram
      << ".";

    // TODO: 3) Remove WxH for screen best-fit during loading of model!
    LoadImage(
      imageContext,
      context->m_Width, context->m_Height
    );

#if 0
    // Access vector image-model.
    VectorImageModel* vectorImageModel =
      qobject_cast< VectorImageModel* >( imageModel );
    assert( vectorImageModel!=NULL );

    // Re-assign rendering-settings to image-model.
    vectorImageModel->SetSettings( settings );
#endif
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
      {
      const VectorImageModel* vim =
	qobject_cast< const VectorImageModel* >( *it );
      qDebug() << vim->GetFilename() << "is modified.";

      return true;
      }
    }

  // Otherwise, this dataset-model is not modified.
  return false;
}

/*******************************************************************************/
void
DatasetModel
::ClearModified()
{
  AbstractImageModelList aimList( GetImageModels() );

  for( AbstractImageModelList::iterator it( aimList.begin() );
       it!=aimList.end();
       ++it )
    {
    ( *it )->ClearModified();
    }
}

/*******************************************************************************/
void
DatasetModel
::virtual_Save()
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
