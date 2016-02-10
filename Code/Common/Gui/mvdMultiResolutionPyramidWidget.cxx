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
  m_GDALOverviewsBuilder = p;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
