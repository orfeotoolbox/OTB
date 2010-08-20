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

#include "otbMapFileProductWriter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int otbMapFileProductWriter(int argc, char* argv[])
{
  const char * infname = argv[1];
  const char * path = argv[2];
  unsigned int tileSize = atoi(argv[3]);

  typedef otb::VectorImage<double>               ImageType;
  typedef otb::MapFileProductWriter<ImageType>   MapFileProductWriterType;
  typedef otb::ImageFileReader<ImageType>        ImageFileReaderType;

  MapFileProductWriterType::Pointer    mapWriter  = MapFileProductWriterType::New();
  ImageFileReaderType::Pointer         reader     = ImageFileReaderType::New();
  
  // Read the image
  reader->SetFileName(infname);
  
  mapWriter->SetInput(reader->GetOutput());
  mapWriter->SetFileName(path);
  mapWriter->SetTileSize(tileSize);
  
  mapWriter->Update();


  return EXIT_SUCCESS;
}

