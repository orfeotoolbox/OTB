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
#include "mvdImportSubDatasetDialog.h"
#include "ui_mvdImportSubDatasetDialog.h"


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
#include "otbGDALImageIO.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"

namespace mvd
{

/*
  TRANSLATOR mvd::ImportSubDatasetDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

char const * const
ImportSubDatasetDialog
::DATASET_COLUMN_NAMES[ ImportSubDatasetDialog::DATASET_COLUMN_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::ImportSubDatasetDialog", "Index" ),
  QT_TRANSLATE_NOOP( "mvd::ImportSubDatasetDialog", "Desc" ),
  QT_TRANSLATE_NOOP( "mvd::ImportSubDatasetDialog", "Name" ),
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
ImportSubDatasetDialog
::ImportSubDatasetDialog( const QString & filename,
		     QWidget * p,
		     Qt::WindowFlags flags  ):
  QDialog( p, flags ),
  m_UI( new mvd::Ui::ImportSubDatasetDialog() ),
  m_HasSubDatasets( false )
{
  m_UI->setupUi( this );

  Setup( filename );
}

/*****************************************************************************/
ImportSubDatasetDialog
::~ImportSubDatasetDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void
ImportSubDatasetDialog
::GetSubDatasets( IntVector & dataset ) const
{
  assert( m_UI!=NULL );
  assert( m_UI->datasetTreeView!=NULL );

  assert(
    m_UI->datasetTreeView->model()==
    qobject_cast< QStandardItemModel * >( m_UI->datasetTreeView->model() )
  );

  QStandardItemModel * itemModel =
    qobject_cast< QStandardItemModel * >( m_UI->datasetTreeView->model() );

  assert( itemModel!=NULL );

  for( int i=0; i<itemModel->rowCount(); ++i )
    {
    assert( itemModel->item( i, DATASET_COLUMN_DESC )!=NULL );

    if( itemModel->item( i, DATASET_COLUMN_DESC )
	->checkState()==Qt::Checked )
      dataset.push_back( i );
    }
}

/*****************************************************************************/
bool
ImportSubDatasetDialog
::HasSubDatasets() const
{
  return m_HasSubDatasets;
}

/*****************************************************************************/
void
ImportSubDatasetDialog
::Setup( const QString & filename )
{
  assert( m_UI!=NULL );
  assert( m_UI->datasetTreeView!=NULL );

  QStandardItemModel * itemModel = NULL;

  //
  // Create item-model.
  {
  QItemSelectionModel * ism = m_UI->datasetTreeView->selectionModel();

  itemModel = new QStandardItemModel( m_UI->datasetTreeView );

  m_UI->datasetTreeView->setModel( itemModel );

  delete ism;
  ism = NULL;
  }

  //
  // Setup item-model
  assert( itemModel!=NULL );

  itemModel->setColumnCount( DATASET_COLUMN_COUNT );

  {
    QStringList headers;

    for( int i=0; i<DATASET_COLUMN_COUNT; ++i )
      headers << QApplication::translate( "mvd", DATASET_COLUMN_NAMES[ i ] );

    itemModel->setHorizontalHeaderLabels( headers );
  }

  //
  // Fill item-model.
  otb::GDALImageIO::Pointer gdalIO( otb::GDALImageIO::New() );

  std::string fname( QFile::encodeName( filename ).constData() );

  if( gdalIO->CanReadFile( fname.c_str() ) )
    {
    gdalIO->SetFileName( fname );

    StringVector names;
    StringVector descs;

    m_HasSubDatasets = gdalIO->GetSubDatasetInfo( names, descs );

    assert( names.size()==descs.size() );

    typedef QList< QStandardItem * > StandardItemList;

    for( std::string::size_type i=0; i<names.size(); ++i )
      {
      assert( i<names.size() );
      assert( i<descs.size() );

      // qDebug() << "name:" << FromStdString( names[ i ] );
      // qDebug() << "desc:" << FromStdString( descs[ i ].c_str() );

      StandardItemList items;

      items << new QStandardItem( QString( "%1" ).arg( i + 1 ) );
      items << new QStandardItem( FromStdString( descs[ i ] ) );

      items.back()->setCheckable( true );
      items.back()->setCheckState( Qt::Checked );

      items << new QStandardItem( FromStdString( names[ i ] ) );

      itemModel->appendRow( items );
      }
    }

  //
  // Fit tree-view to item-model content.
  {
    for( int i=0; i<itemModel->columnCount(); ++i )
      m_UI->datasetTreeView->resizeColumnToContents( i );
  }
}

/*****************************************************************************/
void
ImportSubDatasetDialog
::SetSubDatasetsCheckState( Qt::CheckState state )
{
  assert( m_UI!=NULL );
  assert( m_UI->datasetTreeView!=NULL );

  assert(
    m_UI->datasetTreeView->model()==
    qobject_cast< QStandardItemModel * >( m_UI->datasetTreeView->model() )
  );

  QStandardItemModel * itemModel =
    qobject_cast< QStandardItemModel * >( m_UI->datasetTreeView->model() );

  assert( itemModel!=NULL );

  for( int i=0; i<itemModel->rowCount(); ++i )
    {
    assert( itemModel->item( i, DATASET_COLUMN_DESC )!=NULL );

    itemModel->item( i, DATASET_COLUMN_DESC )->setCheckState( state );
    }
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
ImportSubDatasetDialog
::on_allButton_clicked()
{
  SetSubDatasetsCheckState( Qt::Checked );
}

/*****************************************************************************/
void
ImportSubDatasetDialog
::on_invertButton_clicked()
{
  assert( m_UI!=NULL );
  assert( m_UI->datasetTreeView!=NULL );

  assert(
    m_UI->datasetTreeView->model()==
    qobject_cast< QStandardItemModel * >( m_UI->datasetTreeView->model() )
  );

  QStandardItemModel * itemModel =
    qobject_cast< QStandardItemModel * >( m_UI->datasetTreeView->model() );

  assert( itemModel!=NULL );

  for( int i=0; i<itemModel->rowCount(); ++i )
    {
    QStandardItem * item = itemModel->item( i, DATASET_COLUMN_DESC );

    assert( item!=NULL );

    item->setCheckState(
      item->checkState()==Qt::Unchecked
      ? Qt::Checked
      : Qt::Unchecked
    );
    }
}

/*****************************************************************************/
void
ImportSubDatasetDialog
::on_noneButton_clicked()
{
  SetSubDatasetsCheckState( Qt::Unchecked );
}

} // end namespace 'mvd'
