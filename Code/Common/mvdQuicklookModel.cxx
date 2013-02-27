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
  // get the parent vector image model
  VectorImageModel * viModel = qobject_cast< VectorImageModel* >( parent() );

  // get the filename and use it to compose the quicklook filename
  const char* filename = static_cast<const char*>(viModel->GetFilename().toAscii());
    
  std::string fnameNoExt = itksys::SystemTools::GetFilenameWithoutExtension( filename );    
  std::string path  = itksys::SystemTools::GetFilenamePath( filename );
  std::string ext   = itksys::SystemTools::GetFilenameExtension( filename );

  std::ostringstream qlfname;

  if(path!="")
    {
    qlfname << path<<"/";
    }

    qlfname<<fnameNoExt<<"_quicklook.tif";

  // check if the file exists
  if (!itksys::SystemTools::FileExists(qlfname.str().c_str()))
    {
    // write the file on the disk
    VectorImageFileWriterType::Pointer writer = VectorImageFileWriterType::New();
    writer->SetFileName(qlfname.str());
    writer->SetInput(viModel->ToImage());
    writer->Update();
    }
    
  // reload the quicklook
  QString  qlname(qlfname.str().c_str());
  SetFilename(qlname, 512, 512);

  // Initialize RgbaImageModel.
  InitializeRgbaPipeline();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
