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

#include "mvdDatasetDescriptor.h"


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
#include "mvdTypes.h"
#include "mvdSystemError.h"
#include "mvdVectorImageModel.h"


/*****************************************************************************/
/* MACROS                                                                    */

namespace mvd
{

/*
  TRANSLATOR mvd::DatasetDescriptor

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

/** \brief Indent space when writing XML DOM documents. */
namespace
{
const int XML_INDENT = 2;
}

const char*
DatasetDescriptor::TAG_NAMES[ ELEMENT_COUNT ] =
{
  PROJECT_NAME "_Dataset",
  //
  "dataset",
  "name",
  "alias",
  "date",
  // "path",
  // "directory",
  //
  "images",
  "image_information",
  "image_placename",
  "input_image",
  "ql_input_image",
  "histogram",
  //
  "settings",
  "rgb",
  "grayscale",
  "rgb-dynamics",
  "w-dynamics",
  //
  "properties",
  "no_data",
  //
  "view",
  "center",
  "zoom"
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
DatasetDescriptor
::DatasetDescriptor( QObject* parent ) :
  AbstractModel( parent ),
  m_DomDocument( TAG_NAMES[ ELEMENT_DOCUMENT_ROOT ] ),
  m_RootElement(),
  m_DatasetGroupElement(),
  m_ImagesGroupElement(),
  m_ImageViewGroupElement()
{
}

/*****************************************************************************/
DatasetDescriptor
::~DatasetDescriptor()
{
}

/*****************************************************************************/
void
DatasetDescriptor
::InsertImageModel( int id,
		    const QString& imageFilename,
                    const QString& placename,
		    const void* imageSettings,
		    const QString& quicklookFilename,
		    const QString& histogramFilename,
		    const ImageProperties * properties )
{
  //
  // Image information node.
  QDomElement imageInfoElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_IMAGE_INFORMATION ] )
  );
  imageInfoElt.setAttribute( "id", QString( "%1" ).arg( id ) );
  m_ImagesGroupElement.appendChild( imageInfoElt );

  // Input image filename.
  QDomElement imageElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_IMAGE ] )
  );
  imageElt.setAttribute( "href", imageFilename );
  imageInfoElt.appendChild( imageElt );

  // Quicklook input image filename.
  QDomElement quicklookElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_QUICKLOOK ] )
  );
  quicklookElt.setAttribute( "href", quicklookFilename );
  imageInfoElt.appendChild( quicklookElt );

  // Histogram filename.
  QDomElement histogramElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_HISTOGRAM ] )
  );
  histogramElt.setAttribute( "href", histogramFilename );
  imageInfoElt.appendChild( histogramElt );

  // Input Image placename (computed from the image center coordinates)
  QDomElement placenameElt(
    CreateTextNode(
      placename, 
      TAG_NAMES[ ELEMENT_IMAGE_PLACENAME ] )
  );
  imageInfoElt.appendChild( placenameElt );

  //
  // Properties node.
  assert( properties!=NULL );
  QDomElement propertiesElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_PROPERTIES_GROUP ] )
  );
  imageInfoElt.appendChild( propertiesElt );

  // No-data
  QDomElement noDataElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_PROPERTIES_NO_DATA ] )
  );
  noDataElt.setAttribute( "flag", properties->IsNoDataEnabled() );
  noDataElt.setAttribute( "value", properties->GetNoData() );
  propertiesElt.appendChild( noDataElt );

  //
  // Settings node.
  QDomElement settingsElement(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_SETTINGS_GROUP ] )
  );
  imageInfoElt.appendChild( settingsElement );

  // TODO: Generalize code section
  {
  const VectorImageModel::Settings* settings =
    static_cast< const VectorImageModel::Settings* >( imageSettings );

  // RGB channels.
  QDomElement rgbElement(
    CreateTextNode(
      ContainerToString(
        settings->GetRgbChannels().begin(),
        settings->GetRgbChannels().end()
      ),
      TAG_NAMES[ ELEMENT_RGB_CHANNELS ]
    )
  );
  settingsElement.appendChild( rgbElement );

  // Grayscale
  QDomElement grayscaleElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_GRAYSCALE ] )
  );
  grayscaleElt.setAttribute(
    "activated", settings->IsGrayscaleActivated()
  );
  grayscaleElt.setAttribute( "gray", settings->GetGrayChannel() );
  settingsElement.appendChild( grayscaleElt );

  //
  // Dynamics parameters.
  QDomElement rgbDynamicsElement(
    CreateTextNode(
      ContainerToString(
        settings->GetRgbDynamicsParams().begin(),
        settings->GetRgbDynamicsParams().end()
      ),
      TAG_NAMES[ ELEMENT_RGB_DYNAMICS_PARAMETERS ]
    )
  );
  settingsElement.appendChild( rgbDynamicsElement );

  QDomElement wDynamicsElement(
    CreateTextNode(
      ContainerToString(
        settings->GetGrayDynamicsParams().begin(),
        settings->GetGrayDynamicsParams().end()
      ),
      TAG_NAMES[ ELEMENT_W_DYNAMICS_PARAMETERS ]
    )
  );
  settingsElement.appendChild( wDynamicsElement );
  }
}

/*****************************************************************************/
bool
DatasetDescriptor
::SetImageModelSettings( int id, const void* imageSettings )
{
  qDebug() << "DatasetDescriptor::SetImageModelSettings(" << id << ")";

  assert( imageSettings!=NULL );  

  // Access image information element.
  QDomElement imageInfoElt( GetImageElement( id ) );
  if( imageInfoElt.isNull() )
    return false;

  // Access settings group element.
  QDomElement settingsElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_SETTINGS_GROUP ] )
  );
  // TODO: Manage XML structure errors.
  assert( !settingsElt.isNull() );

  const VectorImageModel::Settings* settings =
    static_cast< const VectorImageModel::Settings* >( imageSettings );
  
  {
    // RGB
    QDomElement rgbElt(
      settingsElt.firstChildElement( TAG_NAMES[ ELEMENT_RGB_CHANNELS  ] )
    );
    // TODO: Manage XML structure errors.
    assert( !rgbElt.isNull() );

    QDomNode node = rgbElt.firstChild();
    // TODO: Manage XML structure errors.
    assert( !node.isNull() );
    assert( node.isText() );
    
    QDomText textNode( node.toText() );
    // TODO: Manage XML structure errors.
    assert( !textNode.isNull() );
    
    textNode.setData(
      ContainerToString(
        settings->GetRgbChannels().begin(),
        settings->GetRgbChannels().end()
      )
    );
  }

  //
  // Grayscale
  {
  // Access element.
  QDomElement grayscaleElt(
    settingsElt.firstChildElement( TAG_NAMES[ ELEMENT_GRAYSCALE ] )
  );
  // TODO: Manage XML structure errors.
  assert( !grayscaleElt.isNull() );
  // Store values.
  grayscaleElt.setAttribute( "activated", settings->IsGrayscaleActivated() );
  grayscaleElt.setAttribute( "gray", settings->GetGrayChannel() );
  }

  {
    // RGB Dynamics
    QDomElement rgbDynamicsElt(
      settingsElt.firstChildElement(
	TAG_NAMES[ ELEMENT_RGB_DYNAMICS_PARAMETERS ]
      )
    );
    // TODO: Manage XML structure errors.
    assert( !rgbDynamicsElt.isNull() );

    QDomNode node = rgbDynamicsElt.firstChild();
    // TODO: Manage XML structure errors.
    assert( !node.isNull() );
    assert( node.isText() );

    QDomText textNode( node.toText() );
    // TODO: Manage XML structure errors.
    assert( !textNode.isNull() );

    textNode.setData(
      ContainerToString(
        settings->GetRgbDynamicsParams().begin(),
        settings->GetRgbDynamicsParams().end()
      )
    );
  }

  {
    // Gray Dynamics
    QDomElement wDynamicsElt(
      settingsElt.firstChildElement(
	TAG_NAMES[ ELEMENT_W_DYNAMICS_PARAMETERS ]
      )
    );
    // TODO: Manage XML structure errors.
    assert( !wDynamicsElt.isNull() );

    QDomNode node = wDynamicsElt.firstChild();
    // TODO: Manage XML structure errors.
    assert( !node.isNull() );
    assert( node.isText() );

    QDomText textNode( node.toText() );
    // TODO: Manage XML structure errors.
    assert( !textNode.isNull() );

    textNode.setData(
      ContainerToString(
        settings->GetGrayDynamicsParams().begin(),
        settings->GetGrayDynamicsParams().end()
      )
    );
  }

  // if everything went ok
  return true;
}

/*****************************************************************************/
bool
DatasetDescriptor
::SetImageModelProperties( int id, const ImageProperties* imageProperties )
{
  qDebug() << "DatasetDescriptor::SetImageModelProperties(" << id << ")";

  assert( imageProperties!=NULL );  

  // Access image information element.
  QDomElement imageInfoElt( GetImageElement( id ) );
  if( imageInfoElt.isNull() )
    return false;

  // Access properties group element.
  QDomElement propertiesElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_PROPERTIES_GROUP ] )
  );
  // TODO: Manage XML structure errors.
  assert( !propertiesElt.isNull() );

  // Access no-data.
  QDomElement noDataElt(
    propertiesElt.firstChildElement( TAG_NAMES[ ELEMENT_PROPERTIES_NO_DATA ] )
  );
  assert( !noDataElt.isNull() );

  noDataElt.setAttribute( "flag", imageProperties->IsNoDataEnabled() );
  noDataElt.setAttribute( "value", imageProperties->GetNoData() );

  // if everything went ok
  return true;
}

/*****************************************************************************/
bool
DatasetDescriptor
::UpdateDatasetAlias( const QString & newAlias )
{
  assert( !newAlias.isEmpty() ); 
 
  // Access dataset group
  if ( m_DatasetGroupElement.isNull() )
    return false;

  // Access dataset alias
  QDomElement dsAliasElt(
    m_DatasetGroupElement.firstChildElement( TAG_NAMES[ ELEMENT_DATASET_ALIAS] )
  );

  // TODO: Manage XML structure errors.
  assert( !dsAliasElt.isNull() );  

  // create a new node
  QDomElement newdsAliasElt(
    CreateTextNode( newAlias, 
                    TAG_NAMES[ ELEMENT_DATASET_ALIAS ] )
    );

  // replace the node
  m_DatasetGroupElement.replaceChild(newdsAliasElt, dsAliasElt );

  // if everything went ok
  return true;
}

/*****************************************************************************/
bool
DatasetDescriptor
::UpdateViewContext(const PointType& center, double zoom)
{
  // Create QString from parameters
  QString scenter = QString( "%1 %2" ).arg( center[0] ).arg( center[1] );
  QString szoom   = QString( "%1" ).arg( zoom );
 
  // Access image view group
  if ( m_ImageViewGroupElement.isNull() )
    return false;

  // Access center
  QDomElement centerElt(
    m_ImageViewGroupElement.firstChildElement( TAG_NAMES[ ELEMENT_VIEW_CENTER] )
  );
  assert( !centerElt.isNull() );

  // create a new center node
  QDomElement newcenterElt(
    CreateTextNode( scenter, 
                    TAG_NAMES[ ELEMENT_VIEW_CENTER ] )
    );

  // replace the node
  m_ImageViewGroupElement.replaceChild(newcenterElt,  centerElt);

  // Access center
  QDomElement zoomElt(
    m_ImageViewGroupElement.firstChildElement( TAG_NAMES[ ELEMENT_VIEW_ZOOM] )
  );
  assert( !zoomElt.isNull() );

  // create a new center node
  QDomElement newzoomElt(
    CreateTextNode( szoom, 
                    TAG_NAMES[ ELEMENT_VIEW_ZOOM ] )
    );

  // replace the node
  m_ImageViewGroupElement.replaceChild(newzoomElt,  zoomElt);

  // if everything went ok
  return true;
}

/*****************************************************************************/
bool
DatasetDescriptor
::UpdateImagePlacename( const QString & nplacename )
{
  assert( !nplacename.isEmpty() ); 
 
  // Access dataset group
  if ( m_ImagesGroupElement.isNull() )
    return false;

  //
  // Image information node.
  QDomElement imageInfoElt(
    m_ImagesGroupElement.firstChildElement( TAG_NAMES[ ELEMENT_IMAGE_INFORMATION ] )
    );

  // Access image placename elt
  QDomElement placenameElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_IMAGE_PLACENAME ] )
    );

  // TODO: Manage XML structure errors.
  assert( !placenameElt.isNull() );  

  // create a new node
  QDomElement nPlacenameElt(
    CreateTextNode( nplacename, 
                    TAG_NAMES[ ELEMENT_IMAGE_PLACENAME ] )
    );

  // replace the node
  imageInfoElt.replaceChild(nPlacenameElt, placenameElt );

  // if everything went ok
  return true;
}

/*****************************************************************************/
void
DatasetDescriptor
::GetImageModel( const QDomElement& imageSibling,
		 int& id,
		 QString& imageFilename,
		 void* imageSettings,
		 QString& quicklookFilename,
		 QString& histogramFilename,
		 ImageProperties* imageProperties )
{
  // TODO: Manager XML structure errors.
  assert( !imageSibling.isNull() );

  // Alias imageSibling into image-information element.
  QDomElement imageInfoElt( imageSibling );
  id = imageInfoElt.attribute( "id" ).toInt();

  // Acces input image element.
  QDomElement imageElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_IMAGE ] )
  );
  // TODO: Manage XML structure errors.
  assert( !imageElt.isNull() );
  imageFilename = imageElt.attribute( "href" );
  
  // Access quicklook image element.
  QDomElement quicklookElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_QUICKLOOK ] )
  );
  // TODO: Manage XML structure errors.
  assert( !quicklookElt.isNull() );
  quicklookFilename = quicklookElt.attribute( "href" );

  // Access histogram element.
  QDomElement histogramElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_HISTOGRAM ] )
  );
  // TODO: Manage XML structure errors.
  //assert( !histogramElt.isNull() );
  histogramFilename = histogramElt.attribute( "href" );

  // Properties.
  if( imageProperties!=NULL )
    {
    QDomElement propertiesElt(
      imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_PROPERTIES_GROUP ] )
    );
    assert( !propertiesElt.isNull() );

    QDomElement noDataElt(
      propertiesElt.firstChildElement( TAG_NAMES[ ELEMENT_PROPERTIES_NO_DATA ] )
    );
    assert( !noDataElt.isNull() );

    bool isOk = true;

    imageProperties->SetNoDataEnabled(
      static_cast< bool >( noDataElt.attribute( "flag" ).toInt( &isOk ) ) );
    assert( isOk );

    imageProperties->SetNoData(
      noDataElt.attribute( "value" ).toDouble( &isOk ) );
    assert( isOk );
    }

  // TODO: Generalize code section.
  if( imageSettings!=NULL )
    {
    VectorImageModel::Settings* settings =
      static_cast< VectorImageModel::Settings* >( imageSettings );

    // Access settings group element.
    QDomElement settingsElt(
      imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_SETTINGS_GROUP ] )
    );
    // TODO: Manage XML structure errors.
    assert( !settingsElt.isNull() );

    // RGB
    QDomElement rgbElt(
      settingsElt.firstChildElement( TAG_NAMES[ ELEMENT_RGB_CHANNELS  ] )
    );
    // TODO: Manage XML structure errors.
    assert( !rgbElt.isNull() );
    VectorImageModel::Settings::ChannelVector rgb;
    ExtractVectorFromElement( rgb, rgbElt );
    assert( rgb.size() == 3 );
    settings->SetRgbChannels( rgb );

    // Grayscale.
    QDomElement grayscaleElt(
      settingsElt.firstChildElement( TAG_NAMES[ ELEMENT_GRAYSCALE ] )
    );
    // TODO: Manage XML structure errors.
    assert( !grayscaleElt.isNull() );
    // Get values.
    settings->SetGrayscaleActivated(
      static_cast< bool >( grayscaleElt.attribute( "activated" ).toInt() )
    );
    settings->SetGrayChannel( grayscaleElt.attribute( "gray" ).toUInt() );

    // RGB Dynamics
    QDomElement rgbDynamicsElt(
      settingsElt.firstChildElement(
	TAG_NAMES[ ELEMENT_RGB_DYNAMICS_PARAMETERS ]
      )
    );
    // TODO: Manage XML structure errors.
    assert( !rgbDynamicsElt.isNull() );
    ParametersType rgbDynamics;
    ExtractArrayFromElement( rgbDynamics, rgbDynamicsElt );
    assert( rgbDynamics.GetSize() == 6 );
    settings->SetRgbDynamicsParams( rgbDynamics );

    // Gray Dynamics
    QDomElement wDynamicsElt(
      settingsElt.firstChildElement(
	TAG_NAMES[ ELEMENT_W_DYNAMICS_PARAMETERS ]
      )
    );
    // TODO: Manage XML structure errors.
    assert( !wDynamicsElt.isNull() );
    ParametersType wDynamics;
    ExtractArrayFromElement( wDynamics, wDynamicsElt );
    assert( wDynamics.GetSize() == 6 );
    settings->SetRgbDynamicsParams( wDynamics );
    }
}

/*****************************************************************************/
void
DatasetDescriptor
::GetDatasetInformation( const QDomElement& datasetSibling,
                         QString& datasetPath,
                         QString& datasetAlias )
{

  // TODO: Manager XML structure errors.
  assert( !datasetSibling.isNull() );

  // Alias imageSibling into image-information element.
  QDomElement datasetElt( datasetSibling );

  // Acces dataset name
  QDomElement datasetNameElt(
		  datasetElt.firstChildElement( TAG_NAMES[ ELEMENT_DATASET_NAME ] )
  );
  // TODO: Manage XML structure errors.
  assert( !datasetNameElt.isNull() );
  datasetPath = datasetNameElt.text();

  // Access dataset alias
  QDomElement datasetAliasElt(
		  datasetElt.firstChildElement( TAG_NAMES[ ELEMENT_DATASET_ALIAS] )
  );
  // TODO: Manage XML structure errors.
  assert( !datasetAliasElt.isNull() );
  datasetAlias = datasetAliasElt.text();
}

/*****************************************************************************/
void
DatasetDescriptor
::GetRenderingImageInformation( const QDomElement& datasetSibling,
                                PointType& center,
                                double&  zoom )
{
  // TODO: Manager XML structure errors.
  assert( !datasetSibling.isNull() );

  // Alias imageSibling into image-information element.
  QDomElement imageViewElt( datasetSibling );

  // Acces roi element
  QDomElement centerElt(
    imageViewElt.firstChildElement( TAG_NAMES[ ELEMENT_VIEW_CENTER ] )
    );
  // TODO: Manage XML structure errors.
  assert( !centerElt.isNull() );

  // get the viewport physical center
  itk::Array<double>  aCenter;
  ExtractArrayFromElement(aCenter, centerElt );

  // fill the reference to the center argument
  center[0] = aCenter.GetElement(0);
  center[1] = aCenter.GetElement(1);
  
  // Access zoom 
  QDomElement zoomElt(
    imageViewElt.firstChildElement( TAG_NAMES[ ELEMENT_VIEW_ZOOM ] )
    );
  // TODO: Manage XML structure errors.
  assert( !zoomElt.isNull() );
  zoom = QVariant( zoomElt.text() ).toReal() ;
}

/*****************************************************************************/
void
DatasetDescriptor
::GetImagePlacename( const QDomElement& datasetSibling,
                    QString& placename)
 {
  // TODO: Manager XML structure errors.
  assert( !datasetSibling.isNull() );

  // Alias imageSibling into image-information element.
  QDomElement imageInfoElt( datasetSibling );

  // Acces dataset name
  QDomElement placenameElt(
    imageInfoElt.firstChildElement( TAG_NAMES[ ELEMENT_IMAGE_PLACENAME ] )
  );
  // TODO: Manage XML structure errors.
  assert( !placenameElt.isNull() );
  placename = placenameElt.text();
}

/*****************************************************************************/
void
DatasetDescriptor
::virtual_Read( QIODevice* device )
{
  qDebug() << "Reading  XML descriptor...";

  // Temporary DOM document.
  QDomDocument domDoc;

  // Error information.
  QString error;
  int line;
  int column;

  // Link to content and manages error.
  if( !domDoc.setContent( device, true, &error, &line, &column ) )
    throw SystemError();

  // If Ok, remember DOM document data (otherwise, forget temporary
  // DOM document and leave class state unchanged).
  m_DomDocument = domDoc;

  // Relink root element.
  m_RootElement =
    m_DomDocument.firstChildElement( TAG_NAMES[ ELEMENT_DOCUMENT_ROOT ] );

  if( m_RootElement.isNull() )
    throw SystemError();

  qDebug() << "XML descriptor has been read.";
}

/*****************************************************************************/
void
DatasetDescriptor
::virtual_Write( QIODevice& device ) const
{
  qDebug() << "Writing XML descriptor...";

  // TODO: Check IO device is formatted to UTF-8 data.
  if( device.write( m_DomDocument.toByteArray( XML_INDENT ) )==-1 )
    throw SystemError();

  qDebug() << "XML descriptor has been written.";
}

/*****************************************************************************/
void
DatasetDescriptor
::virtual_BuildModel( void* context )
{
  if( context==NULL )
    {
    BuildDocument();
    }
  else
    {
    // Get build-context.
    BuildContext* buildContext = static_cast< BuildContext* >( context );

    // Read XML DOM document from file.
    Read( buildContext->m_Filename );

    assert( !m_RootElement.isNull() );

    // Relink image.
    m_DatasetGroupElement =
      m_RootElement.firstChildElement( TAG_NAMES[ ELEMENT_DATASET_GROUP ] );

    // Relink image-group element.
    m_ImagesGroupElement =
      m_RootElement.firstChildElement( TAG_NAMES[ ELEMENT_IMAGES_GROUP ] );

    // Relink view-group element.
    m_ImageViewGroupElement =
      m_RootElement.firstChildElement( TAG_NAMES[ ELEMENT_VIEW_GROUP ] );
    }
}

/*****************************************************************************/
void
DatasetDescriptor
::BuildDocument()
{
  // Get dataset model.
  const DatasetModel* model = GetParentModel< DatasetModel >();
  assert( model );

  // Create root element.
  QDomElement rootElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_DOCUMENT_ROOT ] )
  );
  rootElt.setAttribute( "version", Monteverdi2_VERSION_STRING );
  m_DomDocument.appendChild( rootElt );

  // Dataset element.
  QDomElement datasetElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_DATASET_GROUP ] )
  );
  rootElt.appendChild( datasetElt );
  
  // dataset name element
  QDomElement nameElt(
    CreateTextNode( model->GetName(), 
                    TAG_NAMES[ ELEMENT_DATASET_NAME ] )
    );
  datasetElt.appendChild( nameElt );

  // dataset alias element
  QDomElement aliasElt(
    CreateTextNode( model->GetAlias(), 
                    TAG_NAMES[ ELEMENT_DATASET_ALIAS ] )
    );
  datasetElt.appendChild( aliasElt );

  // dataset creation-date element
  QDomElement dateElt(
    CreateTextNode( QDateTime::currentDateTime().toString( "yyyy-MM-ddThh:mm:ss.zzz" ),
                    TAG_NAMES[ ELEMENT_DATASET_DATE ] )
    );
  datasetElt.appendChild( dateElt );

  // Remember dataset group element.
  m_DatasetGroupElement = datasetElt;

#if 0
  // Dataset path element.
  QDomElement pathElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_DATASET_PATH ] )
  );
  pathElt.setAttribute(
    "href",
    QDir::cleanPath( model->GetPath() )
  );
  datasetElt.appendChild( pathElt );
#endif

#if 0
  // Dataset path element.
  QDomElement dirElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_DATASET_DIRECTORY ] )
  );
  dirElt.setAttribute(
    "href",
    QDir::cleanPath( model->GetDirectory().path() )
  );
  datasetElt.appendChild( dirElt );
#endif

  // Image-group element.
  QDomElement imagesElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_IMAGES_GROUP ] )
  );
  rootElt.appendChild( imagesElt );

  // Remember image group element.
  m_ImagesGroupElement = imagesElt;

  // Image view element.
  QDomElement viewElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_VIEW_GROUP ] )
  );
  rootElt.appendChild( viewElt );
  
  // Image view physicalCenter element
  QDomElement centerElt(
    CreateTextNode( QString::number( model->GetLastPhysicalCenter()[0] ) + 
                    " " + 
                    QString::number( model->GetLastPhysicalCenter()[1] ), 
                    TAG_NAMES[ ELEMENT_VIEW_CENTER ] )
    );
  viewElt.appendChild( centerElt );

  // Image View zoom factor element
  QDomElement zoomElt(
    CreateTextNode( QString::number( model->GetLastIsotropicZoom() ),
                    TAG_NAMES[ ELEMENT_VIEW_ZOOM ] )
    );
  viewElt.appendChild( zoomElt );

  // remember view group
  m_ImageViewGroupElement = viewElt;

  // Add XML processing instruction.
  QDomNode xmlNode(
    m_DomDocument.createProcessingInstruction(
      "xml",
      "version=\"1.0\" encoding=\"UTF-8\""
    )
  );
  m_DomDocument.insertBefore( xmlNode, m_DomDocument.firstChild() );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/

} // end namespace 'mvd'
