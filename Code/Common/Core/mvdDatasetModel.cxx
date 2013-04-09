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
bool
DatasetModel
::IsConsistent() const
{
  bool isConsistent = true;

  if( m_Descriptor==NULL )
    return false;

  for( QDomElement imageElt( m_Descriptor->FirstImageElement() );
       !imageElt.isNull() && isConsistent;
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

    QFileInfo fileInfo( GetDirectory(), imageContext.m_Filename );

    isConsistent =
      isConsistent &&
      fileInfo.exists() &&
      fileInfo.isReadable();
    }

  return isConsistent;
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
void
DatasetModel
::LoadImageModels( int width, int height )
{
  qDebug() << this << "::LoadImageModels(" << width << "," << height << ")";

  BuildContext context( width, height );

  ParseDescriptor( &context );
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
	GetDirectory().relativeFilePath( context.m_Quicklook ),
	GetDirectory().relativeFilePath( context.m_Histogram )
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

    // If enabled...
    if( buildContext->m_IsLoadSubModelsEnabled )
      {
      // ...load image-models from descriptor.
      // TODO: Replace DatasetModel::BuildContext() by (width, height).
      ParseDescriptor( buildContext );
      }
    }
}

/*******************************************************************************/
void
DatasetModel
::ParseDescriptor( BuildContext* context )
{
  qDebug() << this << "::ParseDescriptor(" << context << ")";

  assert( context );
  assert( context->m_IsLoadSubModelsEnabled );

  if( !context->m_IsLoadSubModelsEnabled )
    {
    return;
    }

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

    // Restore absolute file paths.
    if( !imageContext.m_Quicklook.isEmpty() )
      {
      imageContext.m_Quicklook =
	QFileInfo( GetDirectory(), imageContext.m_Quicklook ).filePath();
      }

    if( !imageContext.m_Histogram.isEmpty() )
      {
      imageContext.m_Histogram =
	QFileInfo( GetDirectory(), imageContext.m_Histogram ).filePath();
      }

    // TODO: 3) Remove WxH for screen best-fit during loading of model!
    LoadImage(
      imageContext,
      context->m_Width, context->m_Height
    );
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
const PropertiesContainer &
DatasetModel
::QueryDatasetProperties()
{
  //
  // clear the container
  m_DatasetProperties.clear();

  // get the vector image model
  VectorImageModel* viModel =
    qobject_cast< VectorImageModel* >( GetSelectedImageModel() );
    
  // ----------------------------------------------------------------------------
  // category 1 : file infos
  // ----------------------------------------------------------------------------
  QString key1 = tr("File Informations");

  PropertiesVectorByCategory pFileInfoCat;
  PropertiesVector vfileinfo;
  PropertyType pfile;
  PropertyType pdir;
  PropertyType psize;
  
  //
  std::string fname = ToStdString( viModel->GetFilename() );
  pfile.first  = ToStdString ( tr("File") );
  pfile.second = itksys::SystemTools::GetFilenameName(fname);

  //
  pdir.first  = ToStdString ( tr("Directory") );
  pdir.second = itksys::SystemTools::GetFilenamePath(fname);;

  //
  psize.first  = ToStdString ( tr("Size in Bytes") );
  psize.second = "";

  // add those properties to the vector of properties
  vfileinfo.push_back(pfile);
  vfileinfo.push_back(pdir);
  vfileinfo.push_back(psize);

  pFileInfoCat.first  = ToStdString ( key1 );
  pFileInfoCat.second = vfileinfo;
  
  // add the current category properties
  m_DatasetProperties.insert(pFileInfoCat);

  // ------------------------------------------------------------------------------
  // category 2 : image infos
  // ------------------------------------------------------------------------------
  QString key2 = tr("Image Informations");

  PropertiesVectorByCategory pImageInfoCat;
  PropertiesVector vimageinfo;

  PropertyType pdim;
  PropertyType porigin;
  PropertyType pspacing;
  PropertyType pnbcomp;
  PropertyType pblocksize;

  //
  pdim.first  = ToStdString ( tr("Dimension") );
  pdim.second = ToStdString( viModel->GetNativeLargestRegion().GetSize() );

  //
  porigin.first  = ToStdString ( tr("Origin") );
  porigin.second = ToStdString( viModel->ToImageBase()->GetOrigin() );

  //
  pspacing.first  = ToStdString ( tr("Pixels size") );
  pspacing.second = ToStdString( viModel->GetNativeSpacing() );

  //
  pnbcomp.first  = ToStdString ( tr("Number of components") );
  pnbcomp.second = ToStdString( viModel->ToImageBase()->GetNumberOfComponentsPerPixel() );

  // get the block size from the metadata dictionary
  unsigned int blockSizeX = 0;
  unsigned int blockSizeY = 0;
  itk::MetaDataDictionary dict = viModel->ToImageBase()->GetMetaDataDictionary();
  itk::ExposeMetaData< unsigned int >(dict, otb::MetaDataKey::TileHintX, blockSizeX);
  itk::ExposeMetaData< unsigned int >(dict, otb::MetaDataKey::TileHintY, blockSizeY);

  // from block size to string
  std::ostringstream ossBlocksize;
  ossBlocksize << blockSizeX <<","<<blockSizeY;
  
  // fill the property
  pblocksize.first  = ToStdString ( tr("Block size") );
  pblocksize.second = ossBlocksize.str();

  // add those properties to the vector of properties
  vimageinfo.push_back(pdim);
  vimageinfo.push_back(porigin);
  vimageinfo.push_back(pspacing);
  vimageinfo.push_back(pnbcomp);
  vimageinfo.push_back(pblocksize);

  pImageInfoCat.first  = ToStdString ( key2 );
  pImageInfoCat.second = vimageinfo;

  // add the current category properties
  m_DatasetProperties.insert(pImageInfoCat);
  
  // ------------------------------------------------------------------------------
  // category 3 : metadata infos
  // ------------------------------------------------------------------------------
  QString key3 = tr("Metadatas");

  // get meta-data interface.
  otb::ImageMetadataInterfaceBase::ConstPointer metaData = 
    otb::ConstCast< const otb::ImageMetadataInterfaceBase >(
      otb::ImageMetadataInterfaceFactory::CreateIMI(
        viModel->ToImageBase()->GetMetaDataDictionary()
        )
      );

  PropertiesVectorByCategory pMetadataCat;
  PropertiesVector vmetadata;

  PropertyType psensorid;
  PropertyType pdefaultrgb;

  //
  psensorid.first  = ToStdString ( tr("Sensor") );
  psensorid.second = metaData->GetSensorID();

  //
  pdefaultrgb.first  = ToStdString ( tr("Default RGB") );
  pdefaultrgb.second = ToStdString<unsigned int>( metaData->GetDefaultDisplay() );

  // add those properties to the vector of properties
  vmetadata.push_back(psensorid);
  vmetadata.push_back(pdefaultrgb);

  pMetadataCat.first  = ToStdString ( key3 );
  pMetadataCat.second = vmetadata;

  // add the current category properties
  m_DatasetProperties.insert(pMetadataCat);

  // ------------------------------------------------------------------------------

  return m_DatasetProperties;
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
