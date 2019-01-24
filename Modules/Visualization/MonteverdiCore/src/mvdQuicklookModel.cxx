/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
QuicklookModel
::QuicklookModel( QObject* p ) :
  VectorImageModel( p )
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
  assert( context );
  BuildContext* buildContext = static_cast< BuildContext* >( context );

  //
  // Access rendering settings.

  // TODO: Remove temporary hack (Quicklook rendering settings).
  if( buildContext->m_Settings!=NULL )
    {
    VectorImageSettings * const settings =
      static_cast< VectorImageSettings * >( buildContext->m_Settings );

    SetSettings( *settings );
    }

  //
  // Quicklook file.

  // Quicklook file information.
  bool inMemory = buildContext->m_Quicklook.isEmpty();

  QFileInfo quicklookFileInfo(
    inMemory
    ? buildContext->m_Filename
    : buildContext->m_Quicklook
  );

  // Quicklook filename.
  QString quicklookFilename( quicklookFileInfo.filePath() );

  // First time?
  if( buildContext->IsBeingStored() && !inMemory )
    {
    // Instantiate a quicklook file writer.
    VectorImageFileWriterType::Pointer fileWriter(
      VectorImageFileWriterType::New()
    );

    // Access parent image-model.
    VectorImageModel* viModel = GetImageModel< VectorImageModel >();
    assert( viModel!=NULL );

    CountType currentLod = viModel->GetCurrentLod();
    CountType bestLod = viModel->ComputeBestLod( 512, 512 );

    if( bestLod!=currentLod )
      viModel->SetCurrentLod( bestLod );

    // Write quicklook file on the disk.
    fileWriter->SetFileName( QFile::encodeName( quicklookFilename ).constData()  );
    fileWriter->SetInput( viModel->ToImage() );
    fileWriter->Update();

    if( currentLod!=bestLod )
      viModel->SetCurrentLod( currentLod );
    }

  // Source stored quicklook image-file.
  // Best fit to 512x512 px² size.
  SetFilename(
    quicklookFilename,
    VectorImageModel::DEFAULT_LOD_SIZE,
    VectorImageModel::DEFAULT_LOD_SIZE );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
