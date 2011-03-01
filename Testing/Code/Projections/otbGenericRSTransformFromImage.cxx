/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbGenericRSTransform.h"
#include <ogr_spatialref.h>
#include <fstream>

typedef otb::VectorImage<double, 2>       ImageType;
typedef otb::ImageFileReader<ImageType>   ReaderType;
typedef otb::GenericRSTransform<>         TransformType;
typedef TransformType::InputPointType     PointType;

int otbGenericRSTransformFromImage(int argc, char* argv[])
{
  /*
   * This test checks if we can instanciate a GenericRSTransform from the image information
   * without throwing an exception
   */

  // Reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  
  // Build wgs ref
  OGRSpatialReference oSRS;
  oSRS.SetWellKnownGeogCS("WGS84");
  char * wgsRef = NULL;
  oSRS.exportToWkt(&wgsRef);

  // Instanciate WGS->Image transform
  TransformType::Pointer wgs2img = TransformType::New();
  wgs2img->SetInputProjectionRef(wgsRef);
  wgs2img->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
  wgs2img->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  wgs2img->InstanciateTransform();

  // Instanciate Image->WGS transform
  TransformType::Pointer img2wgs = TransformType::New();
  img2wgs->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
  img2wgs->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
  img2wgs->SetOutputProjectionRef(wgsRef);
  img2wgs->InstanciateTransform();

  return EXIT_SUCCESS;
}
