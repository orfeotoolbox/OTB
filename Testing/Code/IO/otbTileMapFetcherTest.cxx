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
#include "otbTileMapFetcher.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include <stdio.h>

int otbTileMapFetcherTest(int argc, char * argv[])
{
  if(argc != 8)
  {
    std::cout<<"<x> <y> <z> <server name> <cache directory> <cache size> <output directory>"<<std::endl;
    return EXIT_FAILURE;
  }
  else
  {
    typedef unsigned char                       PixelType;
    typedef otb::VectorImage<PixelType,2>       ImageType;
    typedef otb::ImageFileWriter<ImageType>     WriterType;
    typedef otb::TileMapFetcher                 TileMapFetcherType;
    typedef TileMapFetcherType::Pointer         TileMapFetcherPointerType;
    
    unsigned int x = static_cast<unsigned int>(atoi(argv[1]));
    unsigned int y = static_cast<unsigned int>(atoi(argv[2]));
    unsigned int z = static_cast<unsigned int>(atoi(argv[3]));
    std::string serverName = argv[4];
    std::string cacheDirectory = argv[5];
    unsigned int cacheSize = static_cast<unsigned int>(atoi(argv[6]));
    std::string directory = argv[7];
    
    std::ostringstream filename;
    std::ostringstream filename2;
    std::ostringstream filename3;
    
    TileMapFetcherPointerType tmf     = TileMapFetcherType::New();
    ImageType::Pointer        img     = ImageType::New();
    WriterType::Pointer       writer  = WriterType::New();
    
    tmf->SetServerName(serverName);
    tmf->SetCacheDirectory(cacheDirectory);
    tmf->SetCacheSize(cacheSize);
    
    /** Test fetching tile */
    img = tmf->FetchTile(x,y,z);
    
    filename<<directory;
    filename<<"otTileMapFetcherTest_Toulouse.png";
    
    writer->SetFileName(filename.str());
    writer->SetInput(img);
    writer->Update();
    
    /** Test out of range */
    img = tmf->FetchTile(x,y,1);
    
    filename2<<directory;
    filename2<<"otTileMapFetcherTest_OutOfRange.png";
    
    writer->SetFileName(filename2.str());
    writer->SetInput(img);
    writer->Update();
    
    /* Test Cache */
    for(unsigned int i=x; i<x+(5*cacheSize); i++)
    {
      img = tmf->FetchTile(i,y,z);
    }
  }
  return EXIT_SUCCESS;
}
