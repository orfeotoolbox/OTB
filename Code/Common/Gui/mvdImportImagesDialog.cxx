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
#include "Core/mvdAlgorithm.h"

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
  COLUMN_IMAGE_SIZE = 0,
  COLUMN_OVERVIEW_SIZE,
  COLUMN_FILENAME,
  //
  COLUMN_COUNT,
};


const char * const
HEADERS[ COLUMN_COUNT ] =
{
  QT_TRANSLATE_NOOP( "mvd::ImportImagesDialog", "Img Size" ),
  QT_TRANSLATE_NOOP( "mvd::ImportImagesDialog", "Ovr Size" ),
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

      Qt::ItemFlags flags = Qt::NoItemFlags;

      try
	{
	builder->SetInputFileName( filename );

	flags |= Qt::ItemIsSelectable;

	unsigned int count = builder->CountResolutions( 2, 256 );

	if( builder->GetOverviewsCount()>0 )
	  builder->SetBypassEnabled( true );
	else
	  {
	  flags |= Qt::ItemIsEnabled;

	  builder->SetBypassEnabled( count<=1 );

	  ++ m_EffectiveCount;
	  }

	builder->SetResolutionFactor( 2 );
	builder->SetNbResolutions( count );
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

      typedef QList< QStandardItem * > ItemList;

      ItemList items;

      //
      // COLUMN_IMAGE_SIZE
      if( !builder.IsNull() )
	{
	items.push_back(
	  new QStandardItem(
	    QString( "%1x%2" )
	    .arg( builder->GetWidth() )
	    .arg( builder->GetHeight() )
	  )
	);

	items.back()->setFlags( flags );
	}

      //
      // COLUMN_FILE_SIZE
      items.push_back(
	new QStandardItem(
	  ToHumanReadableSize(
	    builder->GetOverviewsCount() > 0
	    ? QFileInfo(
	      filenames[ i ] + ".ovr"
	    )
	    .size()
	    : builder->GetEstimatedSize()
	  )
	)
      );

      items.back()->setFlags( flags );

      //
      // COLUMN_FILENAME
      items.push_back( new QStandardItem( filenames[ i ] ) );

      if( flags.testFlag( Qt::ItemIsEnabled ) )
	{
	items.back()->setFlags( flags | Qt::ItemIsUserCheckable );

	items.back()->setCheckState(
	  builder->IsBypassEnabled()
	  ? Qt::Unchecked
	  : Qt::Checked
	);
	}
      else
	items.back()->setFlags( flags );

      //
      // Append items.
      itemModel->appendRow( items );
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
void
ImportImagesDialog
::UpdateFileSize( size_t row )
{
  assert( row<m_GDALOverviewsBuilders.size() );

  assert( !m_GDALOverviewsBuilders[ row ].IsNull() );
  assert( m_GDALOverviewsBuilders[ row ]->GetOverviewsCount()<=0 );

  //
  // Get item-model.
  assert( m_UI!=NULL );
  assert( m_UI->filenamesTreeView!=NULL );

  QStandardItemModel * model =
    qobject_cast< QStandardItemModel * >(
      m_UI->filenamesTreeView->model()
    );

  assert( model!=NULL );

  //
  // Create index.
  QModelIndex index( model->index( row, COLUMN_OVERVIEW_SIZE ) );

  //
  // Get GDAL overview builder.
  otb::GDALOverviewsBuilder::Pointer builder(
    m_GDALOverviewsBuilders[ index.row() ]
  );
  assert( !builder.IsNull() );

  //
  // Update file size.
  model->setData(
    index,
    ToHumanReadableSize(
      builder->GetEstimatedSize()
    )
  );
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
::on_pyramidWidget_BaseValueChanged( int )
{
  assert( m_UI!=NULL );
  assert( m_UI->filenamesTreeView!=NULL );
  assert( m_UI->filenamesTreeView->selectionModel()!=NULL );

  UpdateFileSize(
    m_UI->filenamesTreeView->selectionModel()->currentIndex().row()
  );
}

/*****************************************************************************/
void
ImportImagesDialog
::on_pyramidWidget_LevelsValueChanged( int )
{
  assert( m_UI!=NULL );
  assert( m_UI->filenamesTreeView!=NULL );
  assert( m_UI->filenamesTreeView->selectionModel()!=NULL );

  UpdateFileSize(
    m_UI->filenamesTreeView->selectionModel()->currentIndex().row()
  );
}

/*****************************************************************************/
void
ImportImagesDialog
::on_pyramidWidget_SizeValueChanged( int )
{
  assert( m_UI!=NULL );
  assert( m_UI->filenamesTreeView!=NULL );
  assert( m_UI->filenamesTreeView->selectionModel()!=NULL );

  UpdateFileSize(
    m_UI->filenamesTreeView->selectionModel()->currentIndex().row()
  );
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


  if( index.column()!=COLUMN_FILENAME )
    return;


  const otb::GDALOverviewsBuilder::Pointer & builder =
    GetBuilder( item->index() );

  if( !builder.IsNull() )
    {
    builder->SetBypassEnabled( item->checkState()!=Qt::Checked );

    UpdateFileSize( index.row() );
    }

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
