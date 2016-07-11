/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdOverviewBuilder.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)
#include <itkProcessObject.h>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nCoreApplication.h"
#include "mvdProcessObjectObserver.h"
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::OverviewBuilder

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
OverviewBuilder
::OverviewBuilder( const GDALOverviewsBuilderVector & builders,
		   QObject * parent ) :
  AbstractWorker( parent ),
  ProgressInterface(),
  m_GDALOverviewsBuilders( builders ),
  m_Index( 0 ),
  m_Count( 0 )
{
}

/*******************************************************************************/
OverviewBuilder
::~OverviewBuilder()
{
  // qDebug() << this << "destroyed.";
}

/*******************************************************************************/
void
OverviewBuilder
::SetProgress( double value )
{
  assert( m_Count>0 );
  assert( m_Index>=0 && m_Index<m_Count );
  assert( value>=0.0 && value<=1.0 );

  // qDebug()
  //   << m_Index << "/" << m_Count
  //   << value << "\t"
  //   << ( 100 * m_Index + static_cast< int >( 100.0 * value ) );

  emit ProgressValueChanged(
    100 * m_Index + static_cast< int >( 100.0 * value )
  );
}

/*******************************************************************************/
QObject *
OverviewBuilder
::virtual_Do()
{
  /*
  emit ProgressTextChanged(
    tr( "Importing image '%1' as dataset into cache directory..." )
    .arg( QFileInfo( m_Filename ).fileName() )
  );
  */

  // emit ProgressTextChanged( GetFirstProgressText() );

  //
  // Count elements to process.
  m_Count = 0;

  for( GDALOverviewsBuilderVector::const_iterator it(
	 m_GDALOverviewsBuilders.begin()
       );
       it!=m_GDALOverviewsBuilders.end();
       ++ it )
    if( !it->IsNull() && ( *it )->GetNbResolutions()>0 )
      ++ m_Count;

  emit ProgressRangeChanged( 0, 100 * m_Count );

  //
  // Process elements.
  m_Index = 0;

  {
    ProcessObjectObserver::Pointer observer( ProcessObjectObserver::New() );

    observer->SetProgressInterface( this );

    for( GDALOverviewsBuilderVector::const_iterator it(
	   m_GDALOverviewsBuilders.begin()
	 );
	 it!=m_GDALOverviewsBuilders.end();
	 ++ it, ++ m_Index )
      if( !it->IsNull() && ( *it )->GetNbResolutions()>0 )
	{
	unsigned long id =
	  ( *it )->AddObserver( itk::ProgressEvent(), observer );

	( *it )->Update();

	( *it )->RemoveObserver( id );

	emit ProgressTextChanged(
	  QString(
	    tr( "Generting overviews for file %1/%2 '%3'." )
	  )
	  .arg( m_Index + 1 )
	  .arg( m_Count )
	  .arg(
	    QFile::decodeName(
	      ( *it )->GetInputFileName().c_str()
	    )
	  )
	);
	}
  }

  emit ProgressValueChanged( 100 * m_Count );
 
  return NULL;
}

/*******************************************************************************/
QString
OverviewBuilder
::virtual_GetFirstProgressText() const
{
  return QString( tr( "Preparing to build GDAL overviews..." ) );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
