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
/* CONSTANTS                                                                 */

namespace
{

/**
 * \brief Array of enhanced band names that OTB can return.
 *
 * It is defined (statically) as a constant for translation purposes.
 */
const char*
ENHANCED_BAND_NAMES[ 10 ] = {
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "Red" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "Green" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "Blue" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "Yellow" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "RedEdge" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "Coastal" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "NIR" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "NIR1" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "NIR2" ),
  QT_TRANSLATE_NOOP( "mvd::AbstractImageModel", "PAN" ),
};

} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
AbstractImageModel
::AbstractImageModel( QObject* parent ) :
  AbstractModel( parent ),
  m_NativeLargestRegion(),
  m_Id( -1 ),
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

  // output band name list
  QStringList qBandNames1( QString("") );

  //
  // PS: need to handle images with extracted channels and a geom file
  // storing the original image band names.
  if (stdBandNames1.empty() ||
      stdBandNames1.size()!=output->GetNumberOfComponentsPerPixel())
    {
    // output stringVector
    StringVector outputBandNames;

    //
    // Fill the output std::vector with default values
    for( unsigned int count = 0;
         count < output->GetNumberOfComponentsPerPixel(); 
         count++)
      {
      std::ostringstream oss;
      oss <<ToStdString( tr("BAND ") )<< count;
      outputBandNames.push_back(oss.str());
      }
    
    // set the output QStringList
    qBandNames1 = ToQStringList( outputBandNames );

    }
  else
    {
    // useless
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
      {
      try
        {
        stdBandNames2 = metaDataInterface->GetEnhancedBandNames();
        }
      catch( itk::ExceptionObject& exc )
        {
	qDebug() << exc.what();
	}
      }

    assert( stdBandNames2.empty() ||
            stdBandNames2.size()==output->GetNumberOfComponentsPerPixel() );

    if( stdBandNames2.size()!=output->GetNumberOfComponentsPerPixel() )
      {
      stdBandNames2.resize( output->GetNumberOfComponentsPerPixel() );
      }

    //
    // Join basic and enhanced band-name lists.
    assert( stdBandNames1.size()==stdBandNames2.size() );

    qBandNames1 =  ToQStringList( stdBandNames1 );
    QStringList qBandNames2( ToQStringList( stdBandNames2 ) );

    QStringList::iterator it1( qBandNames1.begin() );
    QStringList::const_iterator it2( qBandNames2.begin() );
    for( ; it1!=qBandNames1.end(); ++it1, ++it2 )
      {
      if( !it2->isEmpty() )
        it1->append( " (" + *it2 + ")" );
      }
    }

  //
  // Return joined band-name list.
  return qBandNames1;
}

/*******************************************************************************/
void
AbstractImageModel
::virtual_BuildModel( void* context )
{
  //
  // Retrieve proper build-context.
  assert( context!=NULL );
  BuildContext* buildContext = static_cast< BuildContext* >( context );

  //
  // ID.
  m_Id = buildContext->m_Id;

  //
  // QUICKLOOK.
  //
  // Build quicklook before histogram because histogram calculation is
  // based on quicklook image.
  newChildModel< QuicklookModel >( context );

  //
  // HISTOGRAM.
  HistogramModel::BuildContext histogramContext(
    buildContext->IsBeingStored(),
    buildContext->m_Histogram
  );

  // WIP (crashes on reload because serialization methods are not
  // fully implemented.
  newChildModel< HistogramModel >( &histogramContext );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
