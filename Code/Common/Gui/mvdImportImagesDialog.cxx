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

namespace
{
/**
 */
enum Columns
{
  COLUMN_NONE = -1,
  //
  COLUMN_FILENAME = 0,
  //
  COLUMN_COUNT,
};


const char * const
HEADERS[ COLUMN_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::ImportImagesDialog", "Filename" ),
};

} // end of anonymous namespace.


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
  m_UI( new Ui::ImportImagesDialog() ),
  m_EffectiveCount( 0 )
{
  m_UI->setupUi( this );

  {
  QItemSelectionModel * ism = m_UI->filenamesTreeView->selectionModel();

  m_UI->filenamesTreeView->setModel(
    new QStandardItemModel( 0, COLUMN_COUNT, m_UI->filenamesTreeView )
  );

  delete ism;
  ism = NULL;
  }

  QObject::connect(
    m_UI->filenamesTreeView->model(),
    SIGNAL( itemChanged( QStandardItem * ) ),
    // to:
    this,
    SLOT( OnItemChanged( QStandardItem * ) )
  );

  QObject::connect(
    m_UI->filenamesTreeView->selectionModel(),
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
const otb::GDALOverviewsBuilder::Pointer &
ImportImagesDialog
::GetBuilder( const QModelIndex & index ) const
{
  assert( index.isValid() );

  assert(
    index.row()>=0 &&
    static_cast< size_t >( index.row() )<m_GDALOverviewsBuilders.size() );

  return m_GDALOverviewsBuilders[ index.row() ];
}

/*****************************************************************************/
int
ImportImagesDialog
::GetEffectiveCount() const
{
  return m_EffectiveCount;
}

/*****************************************************************************/
const ImportImagesDialog::GDALOverviewsBuilderVector &
ImportImagesDialog
::GetGDALOverviewsBuilders() const
{
  return m_GDALOverviewsBuilders;
}

/*****************************************************************************/
void
ImportImagesDialog
::SetFilenames( const QStringList & filenames )
{
  assert( m_UI!=NULL );
  assert( m_UI->filenamesTreeView!=NULL );

  m_GDALOverviewsBuilders.clear();

  QStandardItemModel * itemModel =
    qobject_cast< QStandardItemModel * >( m_UI->filenamesTreeView->model() );

  assert( itemModel!=NULL );

  itemModel->clear();

  SetHeaders();

  m_EffectiveCount = 0;

  for( int i=0;
       i<filenames.size();
       ++ i )
    {
    std::string filename( QFile::encodeName( filenames[ i ] ).constData() );
    assert( !filename.empty() );

    if( otb::GDALOverviewsBuilder::CanGenerateOverviews( filename ) )
      {
      otb::GDALOverviewsBuilder::Pointer builder(
	otb::GDALOverviewsBuilder::New()
      );

      QStandardItem * item = new QStandardItem( filenames[ i ] );
      assert( item!=NULL );

      item->setFlags( Qt::NoItemFlags );

      try
	{
	builder->SetInputFileName( filename );

	item->setFlags( item->flags() | Qt::ItemIsSelectable );

	if( builder->GetOverviewsCount()<=1 )
	  {
	  item->setFlags( item->flags() | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

	  item->setCheckState( Qt::Checked );

	  ++ m_EffectiveCount;
	  }

	builder->SetResolutionFactor( 2 );
	builder->SetNbResolutions( builder->CountResolutions( 2 ) );
	builder->SetResamplingMethod( otb::GDAL_RESAMPLING_AVERAGE );
	builder->SetCompressionMethod( otb::GDAL_COMPRESSION_NONE );
	builder->SetFormat( otb::GDAL_FORMAT_GEOTIFF );
	}
      catch( const std::exception & e )
	{
	QMessageBox::warning(
	  this,
	  PROJECT_NAME,
	  tr(
	    "The following exception has raised when scanning file '%1' for GDAL overview settings:\n\n%2" )
	  .arg( filenames[ i ] )
	  .arg( e.what() )
	);

	builder = otb::GDALOverviewsBuilder::Pointer();
	}

      m_GDALOverviewsBuilders.push_back( builder );

      itemModel->appendRow( item );
      }
#if ( defined( _DEBUG ) && 1 ) || 0
    else
      qDebug() << "Skipped:" << filenames[ i ];
#endif
    }
}

/*****************************************************************************/
void
ImportImagesDialog
::SetHeaders()
{
  assert( m_UI!=NULL );
  assert(
    m_UI->filenamesTreeView->model()==
    qobject_cast< QStandardItemModel * >( m_UI->filenamesTreeView->model() )
  );

  QStandardItemModel * model =
    qobject_cast< QStandardItemModel * >( m_UI->filenamesTreeView->model() );

  assert( model!=NULL );

  for( int i=0; i<COLUMN_COUNT; ++i )
    {
    qDebug() <<
      qApp->translate(
	"mvd::ImportImagesDialog",
	HEADERS[ i ]
      );

    // labels <<
    //   qApp->translate(
    // 	"mvd::ImportImagesDialog",
    // 	HEADERS[ i ]
    //   );

    // model->horizontalHeaderItem( i )->setText(
    //   qApp->translate(
    // 	"mvd::ImportImagesDialog",
    // 	HEADERS[ i ]
    //   )
    // );

    model->setHorizontalHeaderItem(
      i,
      new QStandardItem(
    	qApp->translate(
    	  "mvd::ImportImagesDialog",
    	  HEADERS[ i ]
    	)
      )
    );
    }

  // qDebug() << labels;

  // model->setHorizontalHeaderLabels( labels );
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
ImportImagesDialog
::on_buttonBox_clicked( QAbstractButton * button )
{
  // qDebug() << this << "::on_buttonBox_clicked(" << button << ")";

  assert( m_UI!=NULL );
  assert( button!=NULL );

  switch( m_UI->buttonBox->standardButton( button ) )
    {
    case QDialogButtonBox::Ok:
      accept();
      break;

    case QDialogButtonBox::Cancel:
      reject();
      break;

    case QDialogButtonBox::Ignore:
      done( -1 );
      break;

    default:
      assert( false && "Unhandled QDialogButtonBox::StandardButton enum value!" );
      reject();
      break;
    }
}

/*****************************************************************************/
void
ImportImagesDialog
::OnCurrentChanged( const QModelIndex & current, const QModelIndex & )
{
  // qDebug() << this << "::OnCurrentChanged(" << current << "," << previous << ")";

  // const QStandardItemModel * itemModel =
  //   qobject_cast< const QStandardItemModel * >(
  //     m_UI->filenamesTreeView->model()
  //   );

  // assert( itemModel!=NULL );

  const otb::GDALOverviewsBuilder::Pointer & builder =
    GetBuilder( current );

  m_UI->pyramidWidget->setEnabled(
    !builder.IsNull() &&
    !builder->IsBypassEnabled()
  );

  m_UI->pyramidWidget->SetBuilder( builder );
}

/*****************************************************************************/
void
ImportImagesDialog
::OnItemChanged( QStandardItem * item )
{
  // qDebug() << this << "::OnItemChanged(" << item << ")";

  assert( item!=NULL );

  QModelIndex index( item->index() );


  const otb::GDALOverviewsBuilder::Pointer & builder =
    GetBuilder( item->index() );

  if( !builder.IsNull() )
    builder->SetBypassEnabled( item->checkState()!=Qt::Checked );


  assert( m_UI!=NULL );
  assert( m_UI->filenamesTreeView->selectionModel()!=NULL );

  if( m_UI->filenamesTreeView->selectionModel()->currentIndex()!=index )
    return;


  m_UI->pyramidWidget->setEnabled(
    !builder.IsNull() &&
    !builder->IsBypassEnabled()
  );
}

} // end namespace 'mvd'
