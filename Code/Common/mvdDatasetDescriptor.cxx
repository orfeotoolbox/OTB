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


namespace mvd
{

/** \brief Indent space when writing XML DOM documents. */
const int XML_INDENT = 2;
  
/*
  TRANSLATOR mvd::DatasetDescriptor

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
DatasetDescriptor
::DatasetDescriptor( QObject* parent ) :
  AbstractModel( parent ),
  m_DomDocument(QString(PROJECT_NAME).append("_Dataset") ),
  m_DatasetElement()
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
::InsertImageModel( const QString& imageFilename,
		    void* imageSettings,
		    const QString& quicklookFilename )
{
  //
  // Image information node.
  QDomElement imagesElement = m_DomDocument.createElement("image_information");
  m_DatasetElement.appendChild(imagesElement);
  imagesElement.setAttribute("id", "0");

  // Input image filename
  QDomElement imgElement = m_DomDocument.createElement("input_image");
  imagesElement.appendChild(imgElement);
  imgElement.setAttribute("href",imageFilename);

  // QL input image filename
  QDomElement qlElement = m_DomDocument.createElement("ql_input_image");
  imagesElement.appendChild(qlElement);
  qlElement.setAttribute("href",quicklookFilename);


  //
  // Settings node.
  QDomElement settingsElement( m_DomDocument.createElement( "settings" ) );
  imagesElement.appendChild(settingsElement);

  // TODO: Generalize code section
  {
  VectorImageModel::Settings* settings =
    static_cast< VectorImageModel::Settings* >( imageSettings );

  //
  // RGB node
  settingsElement.appendChild( CreateContainerNode(settings->GetRgbChannels(), "rgb") );

  //
  // Dynamics node.
  settingsElement.appendChild( CreateContainerNode(settings->GetDynamicsParams(), "dynamics") );

  }

}

/*******************************************************************************/
void
DatasetDescriptor
::GetImageModel( const QDomElement& imageSibling,
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
}

/*******************************************************************************/
void
DatasetDescriptor
::Write( const QString& filename ) const
{
  qDebug() << filename;

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
  qDebug() << "descriptor.xml: " << m_DomDocument.toByteArray( XML_INDENT );
  qDebug() << "descriptor.xml: " << m_DomDocument.toString( XML_INDENT );

  // TODO: Check IO device is formatted to UTF-8 data.
  if( device.write( m_DomDocument.toByteArray( XML_INDENT ) )==-1 )
    throw SystemError();
}

/*******************************************************************************/
void
DatasetDescriptor
::virtual_BuildModel( void* context )
{
  qDebug() << "DatasetDescriptor::virtual_BuildModel()";

#if 0
  // Get build-context.
  assert( context!=NULL );
  BuildContext* buildContext = static_cast< BuildContext* >( context );
#endif

  const DatasetModel* model = GetParentModel< DatasetModel >();
  assert( model );

  // Add the root element
  QDomElement root = m_DomDocument.createElement(QString(PROJECT_NAME).append("_Dataset"));
  m_DomDocument.appendChild(root);

  // Root document element: '<dataset>...</dataset>'.
  QDomElement dsElt = m_DomDocument.createElement("dataset_path");
  dsElt.setAttribute(
    "href",
    QDir::cleanPath( model->GetDirectory().path() )
  );
  m_DomDocument.firstChild().appendChild(dsElt);

  // keep the dataset element
  m_DatasetElement =  m_DomDocument.firstChild().toElement();

  // TODO Add the encoding in the processing instruction
  QDomNode node = m_DomDocument.createProcessingInstruction("xml", "version = \"1.0\"");
  m_DomDocument.insertBefore(node, m_DomDocument.firstChild());
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
