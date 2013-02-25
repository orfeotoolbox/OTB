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
  
  // 
  // if multi-resolution file : write the decompressed resolution to
  // the disk and reload it via VectorImage::SetFilename(....)
  if ( viModel->GetNbLod() > 1 )
    {
    // get the filename and use it to compose the quicklook filename
    const char* filename = static_cast<const char*>(viModel->GetFilename().toAscii());
    
    std::string fnameNoExt = itksys::SystemTools::GetFilenameWithoutExtension( filename );    
    std::string path  = itksys::SystemTools::GetFilenamePath( filename );
    std::string ext   = itksys::SystemTools::GetFilenameExtension( filename );

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
    
    // reload the quicklook
    QString  qlname(qlfname.str().c_str());
    SetFilename(qlname, 512, 512);
    }
  else // if not multi-res, shrink the native image
    {
    // Compute the shrink factor to have 512,512 quicklook size
    SizeType  largestSize =  viModel->GetNativeLargestRegion().GetSize();

    double factorX = (double)(largestSize[0])/512.;
    double factorY = (double)(largestSize[1])/512.;
    double factor = std::max(factorX, factorY);

    // if the image dimensions are lower than 512 in each dimension, 
    // no need to shrink
    if (factor - 1.0 < 0.000000001)
      {
      m_Image = viModel->ToImage();
      }
    else
      {
      // shrink to get a quicklook
      typedef otb::StreamingShrinkImageFilter<SourceImageType, 
                                              SourceImageType>      ShrinkFilterType;

      ShrinkFilterType::Pointer shrinker = ShrinkFilterType::New();
      shrinker->SetInput(viModel->ToImage());
      shrinker->SetShrinkFactor((unsigned int)(factor + 0.5));
      shrinker->Update();
    
      m_Image = shrinker->GetOutput();
      }

    // Remember native largest region.
    m_NativeLargestRegion = m_Image->GetLargestPossibleRegion();
    }

  // Initialize RgbaImageModel.
  InitializeRgbaPipeline();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
