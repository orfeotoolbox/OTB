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


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatasetDescriptor
::DatasetDescriptor( QObject* parent ) :
  AbstractModel( parent ),
  m_DomDocument( QString( "%1 Dataset" ).arg( PROJECT_NAME ) )
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
  qDebug() << m_DomDocument.toByteArray( XML_INDENT );

  // TODO: Check IO device is formatted to UTF-8 data.
  if( device.write( m_DomDocument.toByteArray( XML_INDENT ) )==-1 )
    throw SystemError();
}

/*******************************************************************************/
void
DatasetDescriptor
::virtual_BuildModel( void* context )
{
#if 0
  // Get build-context.
  assert( context!=NULL );
  BuildContext* buildContext = static_cast< BuildContext* >( context );
#endif

  // TODO: Implement building of descriptor.
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
