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
#include "mvdImportImagesDialog.h"
#include "ui_mvdImportImagesDialog.h"


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

namespace mvd
{

/*
  TRANSLATOR mvd::ImportImagesDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
ImportImagesDialog
::ImportImagesDialog( const QStringList & filenames,
		      QWidget * parent,
		      Qt::WindowFlags flags  ) :
  QDialog( parent, flags ),
  m_UI( new Ui::ImportImagesDialog() )
{
  m_UI->setupUi( this );

  {
  QItemSelectionModel * ism = m_UI->filenamesListView->selectionModel();

  m_UI->filenamesListView->setModel(
    new QStandardItemModel( m_UI->filenamesListView )
  );

  delete ism;
  ism = NULL;
  }

  QObject::connect(
    m_UI->filenamesListView->selectionModel(),
    SIGNAL( currentChanged( const QModelIndex &, const QModelIndex & ) ),
    // to:
    this,
    SLOT( OnCurrentChanged( const QModelIndex &, const QModelIndex & ) )
  );

  SetFilenames( filenames );
}

/*****************************************************************************/
ImportImagesDialog
::~ImportImagesDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
ImportImagesDialog
::SetFilenames( const QStringList & filenames )
{
  assert( m_UI!=NULL );
  assert( m_UI->filenamesListView!=NULL );

  m_GdalOverviewsBuilders.resize( filenames.size()  );

  QStandardItemModel * itemModel =
    qobject_cast< QStandardItemModel * >( m_UI->filenamesListView->model() );

  assert( itemModel!=NULL );

  itemModel->clear();

  for( int i=0;
       i<filenames.size();
       ++ i )
    {
    otb::GDALOverviewsBuilder::Pointer builder(
      otb::GDALOverviewsBuilder::New()
    );

    m_GdalOverviewsBuilders[ i ] = builder;

    builder->SetInputFileName(
      QFile::encodeName( filenames[ i ] ).constData()
    );

    builder->SetResolutionFactor( 2 );
    builder->SetNbResolutions( builder->CountResolutions( 2 ) );
    builder->SetResamplingMethod( otb::GDAL_RESAMPLING_AVERAGE );
    builder->SetCompressionMethod( otb::GDAL_COMPRESSION_NONE );
    builder->SetFormat( otb::GDAL_FORMAT_GEOTIFF );

    itemModel->appendRow( new QStandardItem( filenames[ i ] ) );
    }
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
ImportImagesDialog
::OnCurrentChanged( const QModelIndex & current, const QModelIndex & previous )
{
  qDebug() << this << "::OnCurrentChanged(" << current << "," << previous << ")";
}

} // end namespace 'mvd'
