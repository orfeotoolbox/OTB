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
#include "mvdMultiResolutionPyramidWidget.h"
#include "ui_mvdMultiResolutionPyramidWidget.h"


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
#include "Gui/mvdGui.h"


namespace mvd
{

/*
  TRANSLATOR mvd::MultiResolutionPyramidWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

char const * const
GDAL_RESAMPLING_NAMES[ otb::GDAL_RESAMPLING_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "None" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "Nearest" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "Gauss" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "Cubic" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "Average" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "Mode" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "Average Mag/Phase" ),
};


char const * const
GDAL_COMPRESSION_NAMES[ otb::GDAL_COMPRESSION_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "None" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "JPEG" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "LZW" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "PACKBITS" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "DEFLATE" ),
};


char const * const
GDAL_FORMAT_NAMES[ otb::GDAL_FORMAT_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "ERDAS" ),
  QT_TRANSLATE_NOOP( "mvd::MultiResolutionPyramidWidget", "GeoTIFF" ),
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
MultiResolutionPyramidWidget
::MultiResolutionPyramidWidget( QWidget * parent, Qt::WindowFlags flags ) :
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::MultiResolutionPyramidWidget() )
{
  m_UI->setupUi( this );

  {
  QItemSelectionModel * ism = m_UI->resolutionsListView->selectionModel();

  m_UI->resolutionsListView->setModel(
    new QStandardItemModel( m_UI->resolutionsListView )
  );

  delete ism;
  ism = NULL;
  }

  AddItemsInto(
    m_UI->formatComboBox,
    "mvd::MultiResolutionPyramidWidget",
    GDAL_FORMAT_NAMES,
    otb::GDAL_FORMAT_COUNT
  );

  AddItemsInto(
    m_UI->algorithmComboBox,
    "mvd::MultiResolutionPyramidWidget",
    GDAL_RESAMPLING_NAMES,
    otb::GDAL_RESAMPLING_COUNT
  );

  AddItemsInto(
    m_UI->compressionComboBox,
    "mvd::MultiResolutionPyramidWidget",
    GDAL_COMPRESSION_NAMES,
    otb::GDAL_COMPRESSION_COUNT
  );

  m_UI->baseSpinBox->setValue( 2 );
  m_UI->levelsSpinBox->setValue( 1 );
  m_UI->sizeSpinBox->setValue( 2 );

  
}

/*****************************************************************************/
MultiResolutionPyramidWidget
::~MultiResolutionPyramidWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
MultiResolutionPyramidWidget
::SetBuilder( const otb::GDALOverviewsBuilder::Pointer & p )
{
  m_UI->formatComboBox->setCurrentIndex( otb::GDAL_FORMAT_GEOTIFF );
  m_UI->algorithmComboBox->setCurrentIndex( otb::GDAL_RESAMPLING_AVERAGE );
  m_UI->compressionComboBox->setCurrentIndex( otb::GDAL_COMPRESSION_NONE );

  m_UI->baseSpinBox->setValue( 2 );
  m_UI->levelsSpinBox->setValue( 1 );
  m_UI->sizeSpinBox->setValue( 1 );

  QStandardItemModel * itemModel =
    qobject_cast< QStandardItemModel * >( m_UI->resolutionsListView->model() );

  assert( itemModel!=NULL );

  itemModel->clear();

  if( p.IsNull() )
    return;

  m_GDALOverviewsBuilder = p;

  assert( !m_GDALOverviewsBuilder.IsNull() );

  m_UI->formatComboBox->setCurrentIndex(
    m_GDALOverviewsBuilder->GetFormat()
  );

  m_UI->algorithmComboBox->setCurrentIndex(
    m_GDALOverviewsBuilder->GetResamplingMethod()
  );

  m_UI->compressionComboBox->setCurrentIndex(
    m_GDALOverviewsBuilder->GetCompressionMethod()
  );

  m_UI->baseSpinBox->setValue( m_GDALOverviewsBuilder->GetResolutionFactor() );

  m_UI->levelsSpinBox->setValue( m_GDALOverviewsBuilder->GetNbResolutions() );

  m_UI->sizeSpinBox->setValue(
    m_GDALOverviewsBuilder->CountResolutions(
      m_GDALOverviewsBuilder->GetResolutionFactor(),
      m_GDALOverviewsBuilder->GetNbResolutions()
    )
  );

  {
  otb::GDALOverviewsBuilder::SizeVector resolutions;

  m_GDALOverviewsBuilder->ListResolutions( resolutions );

  for( otb::GDALOverviewsBuilder::SizeVector::const_iterator it(
	 resolutions.begin()
       );
       it!=resolutions.end();
       ++ it )
    itemModel->appendRow(
      new QStandardItem(
	QString( "%1x%2" )
	.arg( ( *it )[ 0 ] )
	.arg( ( *it )[ 1 ] )
      )
    );
  }
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
