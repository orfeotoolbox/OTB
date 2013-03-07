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
::ImportImage( const QString& filename , int width, int height )
{
  LoadImage( filename, true, width, height );
}

/*******************************************************************************/
void
DatasetModel
::LoadImage( const QString& filename, bool foo, int width, int height )
{
  // 1. Instanciate local image model.
  VectorImageModel* vectorImageModel = new VectorImageModel( this );

  // 2. Safely load data from file.
  try
    {
    //
    // 2.1. Information.
    vectorImageModel->SetFilename( filename, width, height );

    //
    // 2.2. Generate cached data.
    // TODO: generate image-model cached data (quicklook,
    // histogram-list etc.)
    vectorImageModel->BuildModel();
    assert( vectorImageModel->GetQuicklookModel()!=NULL );

    // If first time, import image into descriptor.
    if( foo )
      {
      //
      // 2.3: Add image to Dataset descriptor file.
      m_Descriptor->InsertImageModel(
	GetImageModels().size(),
	vectorImageModel->GetFilename(),
	&vectorImageModel->GetSettings(),
	vectorImageModel->GetQuicklookModel()->GetFilename()
      );

      //
      // 2.4: Force writing descriptor with newly imported image.
      WriteDescriptor();
      }
    }
  catch( std::exception& exc )
    {
    // Release local memory allocation.
    delete vectorImageModel;
    vectorImageModel = NULL;

    // Forward exception to upper level (GUI).
    throw;
    }
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
    ParseDescriptor(buildContext);
    }
}

/*******************************************************************************/
void
DatasetModel
::ParseDescriptor(BuildContext* bContext )
{
  for( QDomElement imageElt( m_Descriptor->FirstImageElement() );
       !imageElt.isNull();
       imageElt = DatasetDescriptor::NextImageSiblingElement( imageElt ) )
    {
    int id = -1;
    QString filename;
    QString quicklook;
    VectorImageModel::Settings settings;

    DatasetDescriptor::GetImageModel(
      imageElt,
      id, filename, &settings, quicklook
    );

    qDebug()
      << "Input image:"
      << "\nid: " << id
      << "\nfilename: " << filename
      << "\nquicklook: " << quicklook;

    // TODO: 1) Re-use quicklook filename.
    // TODO: 2) Assign rendering-settings.
    // TODO: 3) Remove WxH for screen best-fit during loading of model!
    LoadImage( filename, false, bContext->m_Width, bContext->m_Height );
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
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
