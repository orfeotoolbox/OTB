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

#include "mvdAbstractImageModel.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdQuicklookModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::AbstractImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
AbstractImageModel
::AbstractImageModel( QObject* parent ) :
  AbstractModel( parent ),
  m_NativeLargestRegion(),
  m_CurrentLod( 0 )
{
}

/*******************************************************************************/
AbstractImageModel
::~AbstractImageModel()
{
}


/*****************************************************************************/
const QuicklookModel*
AbstractImageModel
::GetQuicklookModel() const
{
  return findChild< const QuicklookModel* >();
}

/*****************************************************************************/
QuicklookModel*
AbstractImageModel
::GetQuicklookModel()
{
  return findChild< QuicklookModel* >();
}

/*******************************************************************************/
QStringList
AbstractImageModel
::GetBandNames( bool enhanced ) const
{
  ImageBaseType::ConstPointer output( ToImageBase() );

  otb::ImageMetadataInterfaceBase::ConstPointer metaDataInterface(
    GetMetaDataInterface()
  );

  //
  // Basic band names.
  StringVector stdBandNames1( metaDataInterface->GetBandName() );

  /*
  qDebug() << "stdBandNames1.size(): " <<  stdBandNames1.size();
  for( unsigned int i=0; i<stdBandNames1.size(); ++i )
    qDebug() << i << ": " << QString::fromStdString( stdBandNames1[ i ] );
  */

  assert( stdBandNames1.empty() ||
	  stdBandNames1.size()==output->GetNumberOfComponentsPerPixel() );

  if( stdBandNames1.size()!=output->GetNumberOfComponentsPerPixel() )
    {
    stdBandNames1.resize( output->GetNumberOfComponentsPerPixel() );
    }

  //
  // Enhanced band names.
  StringVector stdBandNames2;
  if( enhanced )
    stdBandNames2 = metaDataInterface->GetEnhancedBandNames();


  /*
  qDebug() << "stdBandNames2.size(): " <<  stdBandNames2.size();
  for( unsigned int i=0; i<stdBandNames2.size(); ++i )
    qDebug() << i << ": " << QString::fromStdString( stdBandNames2[ i ] );
  */

  assert( stdBandNames2.empty() ||
	  stdBandNames2.size()==output->GetNumberOfComponentsPerPixel() );

  if( stdBandNames2.size()!=output->GetNumberOfComponentsPerPixel() )
    {
    stdBandNames2.resize( output->GetNumberOfComponentsPerPixel() );
    }

  //
  // Join basic and enhanced band-name lists.
  assert( stdBandNames1.size()==stdBandNames2.size() );

  QStringList qBandNames1( ToQStringList( stdBandNames1 ) );
  QStringList qBandNames2( ToQStringList( stdBandNames2 ) );

  QStringList::iterator it1( qBandNames1.begin() );
  QStringList::const_iterator it2( qBandNames2.begin() );
  for( ; it1!=qBandNames1.end(); ++it1, ++it2 )
    {
    if( !it2->isEmpty() )
      it1->append( " (" + *it2 + ")" );
    }

  //
  // Return joined band-name list.
  return qBandNames1;
}

/*******************************************************************************/
void
AbstractImageModel
::virtual_BuildModel()
{
  newChildModel< HistogramModel >();
  newChildModel< QuicklookModel >();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
