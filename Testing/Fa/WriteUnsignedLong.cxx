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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//TODO change the bug tracker url
//http://bugs.orfeo-toolbox.org/view.php?id=406

int main(int argc, char *argv[])
{
  char * filename = argv[1];

  typedef unsigned long                   PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  // check for input images
  ImageType::Pointer image = ImageType::New();
  WriterType::Pointer writer = WriterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  ImageType::IndexType start;

  start[0] =   0;  // first index on X
  start[1] =   0;  // first index on Y

  ImageType::SizeType size;

  size[0]  = 10;  // size along X
  size[1]  = 10;

  ImageType::RegionType region;

  region.SetSize(size);
  region.SetIndex(start);
  
  image->SetRegions(region);
  image->Allocate();

  ImageType::PixelType initialValue = 0;
  image->FillBuffer(initialValue);
  
  ImageType::IndexType pixelIndex;

  pixelIndex[0] = 1;   // x position
  pixelIndex[1] = 1;
  
  image->SetPixel(pixelIndex, initialValue + 1);

  writer->SetInput(image);
  writer->SetFileName(filename);
  writer->Update();

  reader->SetFileName(filename);
  reader->Update();

  ImageType::RegionType outputRegion;
  outputRegion = reader->GetOutput()->GetLargestPossibleRegion();
  
  ImageType::SizeType outputSize;
  outputSize = outputRegion.GetSize();
  
  if (reader->GetOutput()->GetPixel(pixelIndex) != initialValue + 1 )
    {
    return EXIT_FAILURE;
    }
  else
    {
    return EXIT_SUCCESS;
    }
}
