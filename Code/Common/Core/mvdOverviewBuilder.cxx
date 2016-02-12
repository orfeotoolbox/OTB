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

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdDatasetModel.h"
#include "mvdI18nCoreApplication.h"
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
  m_GDALOverviewsBuilders( builders )
{
}

/*******************************************************************************/
OverviewBuilder
::~OverviewBuilder()
{
  // qDebug() << this << "destroyed.";
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
  int count = 0;

  for( GDALOverviewsBuilderVector::const_iterator it(
	 m_GDALOverviewsBuilders.begin()
       );
       it!=m_GDALOverviewsBuilders.end();
       ++ it )
    if( !it->IsNull() && ( *it )->GetNbResolutions()>0 )
      ++ count;

  emit ProgressRangeChanged( 0, count - 1 );

  //
  // Process elements.
  int i = 0;

  for( GDALOverviewsBuilderVector::const_iterator it(
	 m_GDALOverviewsBuilders.begin()
       );
       it!=m_GDALOverviewsBuilders.end();
       ++ it )
    if( !it->IsNull() && ( *it )->GetNbResolutions()>0 )
      {
      ( *it )->Update();

      emit ProgressValueChanged( i ++ );

      emit ProgressTextChanged(
	QString(
	  tr( "Generting overviews for file %1/%2 '%3'." )
	)
	.arg( i + 1 )
	.arg( count )
	.arg(
	  QFile::decodeName(
	    ( *it )->GetInputFileName().c_str()
	  )
	)
      );
      }

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
