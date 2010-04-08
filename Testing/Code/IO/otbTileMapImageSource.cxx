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
#include "itkExceptionObject.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbTileMapImageSource.h"

int otbTileMapImageSource(int argc, char * argv[])
{
  if (argc != 6)
    {
    std::cout << "<latitude> <longitude> <server> <output filename> <output size>" << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    const unsigned int Dimension = 2;

    typedef unsigned char                      PixelType;
    typedef otb::Image<PixelType, Dimension>   ImageType;
    typedef ImageType::SizeType                SizeType;
    typedef otb::ImageFileWriter<ImageType>    WriterType;
    typedef otb::TileMapImageSource<ImageType> TileMapImageSourceType;

    double             latitude = static_cast<double>(atof(argv[1]));
    double             longitude = static_cast<double>(atof(argv[2]));
    char *             serverName = argv[3];
    char *             outputFilename = argv[4];
    const unsigned int outputSize = atoi(argv[5]);
    SizeType           size;
    size[0] = outputSize;
    size[1] = outputSize;

    // Instantiating object
    TileMapImageSourceType::Pointer tileMap = TileMapImageSourceType::New();
    tileMap->SetServerName(serverName);
    tileMap->SetLatitude(latitude);
    tileMap->SetLongitude(longitude);
    tileMap->SetSize(size);

    tileMap->UpdateOutputInformation();

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);

    writer->SetInput(tileMap->GetOutput());
    writer->Update();
    }
  return EXIT_SUCCESS;
}
