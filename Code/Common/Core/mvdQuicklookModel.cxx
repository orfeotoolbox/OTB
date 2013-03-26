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
#include "mvdQuicklookModel.h"


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
#include "otbStreamingShrinkImageFilter.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"


namespace mvd
{
/*
  TRANSLATOR mvd::QuicklookModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

const char* QuicklookModel::IMAGE_FILE_EXT = ".ql.tif";

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
QuicklookModel
::QuicklookModel( QObject* parent ) :
  VectorImageModel( parent )
{
}

/*******************************************************************************/
QuicklookModel
::~QuicklookModel()
{
}

/*******************************************************************************/
void
QuicklookModel
::virtual_BuildModel( void* context )
{
  //
  // Get build-context.
  BuildContext* buildContext = static_cast< BuildContext* >( context );

  //
  // Filename & rendering settings.
  QString quicklookFilename;

  if( buildContext!=NULL )
    {
    quicklookFilename = buildContext->m_Quicklook;

    // TODO: Remove temporary hack (Quicklook rendering settings).
    if( buildContext->m_Settings!=NULL )
      {
      Settings * const settings =
	static_cast< Settings * const >( buildContext->m_Settings );

      SetSettings( *settings );
      }
    }

  //
  // Quicklook file.
  if( quicklookFilename.isEmpty() )
    {
    //
    // get the parent vector image model
    const VectorImageModel* viModel = GetImageModel< VectorImageModel >();
    assert( viModel!=NULL );

    const DatasetModel* datasetModel = viModel->GetDatasetModel();
    assert( datasetModel!=NULL );

    // Source image file information.
    QFileInfo imageFileInfo( viModel->GetFilename() );

    // Quicklook file information.
    QFileInfo quicklookFileInfo(
      datasetModel->GetDirectory().path(),
      imageFileInfo.completeBaseName()
      + QuicklookModel::IMAGE_FILE_EXT
    );

    // Quicklook filename.
    quicklookFilename = quicklookFileInfo.filePath();

    // First time?
    if( !quicklookFileInfo.exists() )
      {
      // Instanciate a quicklook file writer.
      VectorImageFileWriterType::Pointer fileWriter(
	VectorImageFileWriterType::New()
      );

      // Write quicklook file on the disk.
      fileWriter->SetFileName( ToStdString( quicklookFilename ) );
      fileWriter->SetInput( viModel->ToImage() );
      fileWriter->Update();
      }
    }

  // Source stored quicklook image-file.
  // TODO: Remove hard-coded 512x512 px size.
  SetFilename( quicklookFilename, 512, 512 );

  // Initialize RgbaImageModel.
  InitializeRgbaPipeline();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
