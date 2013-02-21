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
::virtual_BuildModel()
{

  std::cout << "QuicklookModel::virtual_BuildModel "<< std::endl;
 //
  // Step #1: Perform pre-process of AbstractModel::BuildModel()
  // pattern.

  //
  // Step #2: Perform standard AbstractModel::BuildModel()
  // pattern. Call parent virtual method.

  // Empty step #2 in case of Quicklook model.

  //
  // Step #3: Post-process of the BuildModel() pattern.
  VectorImageModel * viModel = qobject_cast< VectorImageModel* >( parent() );
  
  // if multi-resolution file
  if ( viModel->GetNbLod() > 1 )
    {
    std::cout <<"QuicklookModel::virtual_BuildModel -> multi-res file " << std::endl;
    // get the filename and use it to compose the quicklook filename
    std::string fnameNoExt = itksys::SystemTools::GetFilenameWithoutExtension( 
      viModel->GetFilename().toStdString() );
    
    std::string path  = itksys::SystemTools::GetFilenamePath( viModel->GetFilename().toStdString());
    std::string ext   = itksys::SystemTools::GetFilenameExtension( viModel->GetFilename().toStdString());

    std::ostringstream qlfname;
    qlfname << path<<"/"<<fnameNoExt<<"_quicklook.tif";

    // check if the file exists
    if (!itksys::SystemTools::FileExists(qlfname.str().c_str()))
      {
      // write the file on the disk
      VectorImageFileWriterType::Pointer writer = VectorImageFileWriterType::New();
      writer->SetFileName(qlfname.str());
      writer->SetInput(viModel->ToImage());
      writer->Update();
      }
    else
      {
      std::cout <<"file "<< qlfname.str() << " exists on disk" << std::endl;
      }
    
    // reload the quicklook
    QString  qlname(qlfname.str().c_str());
    SetFilename(qlname, 512, 512);
    }
  else
    {
    std::cout <<"QuicklookModel::virtual_BuildModel -> NO multires file " << std::endl;
    //TODO:  shrink to get a quicklook
    SetFilename( viModel->GetFilename(), 512, 512 );
    }

  // Initialize RgbaImageModel.
  InitializeRgbaPipeline();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
