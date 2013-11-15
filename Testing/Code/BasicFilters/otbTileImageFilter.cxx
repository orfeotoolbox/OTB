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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbTileImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

typedef otb::Image<unsigned char>                     ImageType;
typedef otb::VectorImage<unsigned char>               VectorImageType;
typedef otb::TileImageFilter<ImageType>                TileImageFilterType;
typedef otb::TileImageFilter<VectorImageType>          TileVectorImageFilterType;
typedef otb::ImageFileReader<VectorImageType>          VectorImageReaderType;
typedef otb::ImageFileWriter<VectorImageType> VectorImageWriterType;


int otbTileImageFilterNew(int argc, char * argv[])
{
  TileImageFilterType::Pointer tileFilter = TileImageFilterType::New();
  TileVectorImageFilterType::Pointer tileVectorFilter = TileVectorImageFilterType::New();

  return EXIT_SUCCESS;
}

int otbTileImageFilter(int argc, char * argv[])
{
  TileVectorImageFilterType::SizeType layout;
  layout[0]=atoi(argv[1]);
  layout[1]=atoi(argv[2]);

  const char * outfname = argv[3];

  unsigned int numberOfImages = layout[0] * layout[1];

  std::vector<VectorImageReaderType::Pointer> readersVector;

  TileVectorImageFilterType::Pointer tileFilter = TileVectorImageFilterType::New();
  tileFilter->SetLayout(layout);

  for(unsigned int i = 0; i<numberOfImages; ++i)
    {
    if(i+4 > (unsigned int)argc)
      {
      std::cerr<<"Not enough images to support layout!"<<std::endl;
      return EXIT_FAILURE;
      }

    VectorImageReaderType::Pointer reader = VectorImageReaderType::New();
    reader->SetFileName(argv[i+4]);

    readersVector.push_back(reader);

    tileFilter->SetInput(i,reader->GetOutput());
    }


  VectorImageWriterType::Pointer writer = VectorImageWriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(tileFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
