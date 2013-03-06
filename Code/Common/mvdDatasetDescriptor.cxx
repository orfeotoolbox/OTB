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
#include "mvdVectorImageModel.h"
#include "mvdSystemError.h"


/*****************************************************************************/
/* MACROS                                                                    */

/** \brief Indent space when writing XML DOM documents. */
#define XML_INDENT 2

namespace mvd
{

/*
  TRANSLATOR mvd::DatasetDescriptor

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */
  
const char*
DatasetDescriptor::TAG_NAMES[ ELEMENT_COUNT ] =
{
  PROJECT_NAME "_Dataset",
  //
  "dataset",
  "name",
  "path",
  "directory",
  //
  "images",
  "image_information",
  "input_image",
  "ql_input_image",
  //
  "settings",
  "rgb",
  "dynamics"
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatasetDescriptor
::DatasetDescriptor( QObject* parent ) :
  AbstractModel( parent ),
  m_DomDocument( TAG_NAMES[ ELEMENT_DOCUMENT_ROOT ] ),
  m_ImagesGroupElement()
{
}

/*******************************************************************************/
DatasetDescriptor
::~DatasetDescriptor()
{
}

/*******************************************************************************/
void
DatasetDescriptor
::InsertImageModel( int id,
		    const QString& imageFilename,
		    void* imageSettings,
		    const QString& quicklookFilename )
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


  //
  // Settings node.
  QDomElement settingsElement(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_SETTINGS_GROUP ] )
  );
  imageInfoElt.appendChild( settingsElement );

  // TODO: Generalize code section
  {
  VectorImageModel::Settings* settings =
    static_cast< VectorImageModel::Settings* >( imageSettings );

  // RGB channels.
  QDomElement rgbElement(
    CreateContainerNode(
      settings->GetRgbChannels().begin(),
      settings->GetRgbChannels().end(),
      TAG_NAMES[ ELEMENT_RGB_CHANNELS ]
    )
  );
  settingsElement.appendChild( rgbElement );

  //
  // Dynamics parameters.
  QDomElement dynamicsElement(
    CreateContainerNode(
      settings->GetDynamicsParams().begin(),
      settings->GetDynamicsParams().end(),
      TAG_NAMES[ ELEMENT_DYNAMICS_PARAMETERS ]
    )
  );
  settingsElement.appendChild( dynamicsElement );
  }
}

/*******************************************************************************/
void
DatasetDescriptor
::GetImageModel( const QDomElement& imageSibling,
		 int& id,
		 QString& imageFilename,
		 void* imageSettings,
		 QString& quicklookFilename )
{
  imageFilename = imageSibling.attribute( "filename" );

  // TODO: Generalize code section
  {
  VectorImageModel::Settings* settings =
    static_cast< VectorImageModel::Settings* >( imageSettings );

  // Settings
  QDomElement settingsElement( imageSibling.firstChildElement( "settings" ) );

  // RGB
  QDomElement rgbElement( settingsElement.firstChildElement( "rgb" ) );

  // Dynamics
  QDomElement dynamicsElement( settingsElement.firstChildElement( "dynamics" ) );

  // Ensure there is only one '<settings>...</settings>' child.
  settingsElement = settingsElement.nextSiblingElement( "settings" );
  assert( settingsElement.isNull() );
  }

  quicklookFilename = imageSibling.attribute( "quicklook" );
}

/*******************************************************************************/
void
DatasetDescriptor
::Read( const QString& filename )
{
  // File instance.
  QFile file( filename );

  // Open file on device.
  if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

  // Read file context.
  Read( &file );

  // File is closed by automatic scope detruction of QFile instance.
}

/*******************************************************************************/
void
DatasetDescriptor
::Read( QIODevice* device )
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
    throw /* XmlError( error, line, column ) */;

  // If Ok, remember DOM document data (otherwise, forget temporary
  // DOM document and leave class state unchanged).
  m_DomDocument = domDoc;

  // Relink root element.
  QDomElement rootElt(
    m_DomDocument.firstChildElement( TAG_NAMES[ ELEMENT_DOCUMENT_ROOT ] )
  );
  assert( !rootElt.isNull() );

  // Relink image-group element.
  m_ImagesGroupElement =
    rootElt.firstChildElement( TAG_NAMES[ ELEMENT_IMAGES_GROUP ] );

  qDebug()
    << "Read XML descriptor:\n"
    << m_DomDocument.toByteArray()
    << "\n.";
}

/*******************************************************************************/
void
DatasetDescriptor
::Write( const QString& filename ) const
{
  // File instance.
  QFile file( filename );

  // Open file on device.
  if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    throw SystemError(
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

  try
    {
    // Read file context.
    Write( file );
    }
  catch( SystemError& syserr )
    {
    // Catch any SystemError thrown by DatasetDescriptor::Write() and
    // morph it into the same SystemError containing filename
    // information.
    syserr = SystemError(
      syserr.GetErrorCode(),
      ToStdString(
	QString( "('%1')" ).arg( filename ) )
    );

    // Throw morphed SystemError.
    throw syserr;
    }

  // File is closed by automatic scope detruction of QFile instance.
}

/*******************************************************************************/
void
DatasetDescriptor
::Write( QIODevice& device ) const
{
  qDebug()
    << "Writing  XML descriptor:\n"
    << m_DomDocument.toByteArray( XML_INDENT )
    << "\n...";

  // TODO: Check IO device is formatted to UTF-8 data.
  if( device.write( m_DomDocument.toByteArray( XML_INDENT ) )==-1 )
    throw SystemError();

  qDebug() << "Written XML descriptor.";
}

/*******************************************************************************/
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
    }
}

/*******************************************************************************/
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
  datasetElt.setAttribute(
    "name",
    model->GetName()
  );
  rootElt.appendChild( datasetElt );

  // Dataset path element.
  QDomElement pathElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_DATASET_PATH ] )
  );
  pathElt.setAttribute(
    "href",
    QDir::cleanPath( model->GetPath() )
  );
  datasetElt.appendChild( pathElt );

  // Dataset path element.
  QDomElement dirElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_DATASET_DIRECTORY ] )
  );
  dirElt.setAttribute(
    "href",
    QDir::cleanPath( model->GetDirectory().path() )
  );
  datasetElt.appendChild( dirElt );

  // Image-group element.
  QDomElement imagesElt(
    m_DomDocument.createElement( TAG_NAMES[ ELEMENT_IMAGES_GROUP ] )
  );
  rootElt.appendChild( imagesElt );

  // Remember image group element.
  m_ImagesGroupElement = imagesElt;

  // Add XML processing instruction.
  QDomNode xmlNode(
    m_DomDocument.createProcessingInstruction(
      "xml",
      "version=\"1.0\" encoding=\"UTF-8\""
    )
  );
  m_DomDocument.insertBefore( xmlNode, m_DomDocument.firstChild() );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
