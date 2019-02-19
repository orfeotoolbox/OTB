/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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



#include <iostream>

//  This example shows how to read a list of images and concatenate
//  them into a vector image. We will write a program which is able to
//  perform this operation taking advantage of the streaming
//  functionnalities of the processing pipeline. We will assume that
//  all the input images have the same size and a single band.
//
//  The following header files will be needed:

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageFileWriter.h"

int main(int argc, char** argv)
{
  if (argc < 4)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "outputImage image1 image2 ... " << std::endl;
    }

  const unsigned int NbImages = argc - 2;

  std::cout << "Concat of " << NbImages <<
  " images into a multi-band image " <<
  std::endl;

//  We will start by defining the types for the input images and the
//  associated readers.

  typedef unsigned short int PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

//  We will use a list of image file readers in order to open all the
//  input images at once. For this, we use the
//  \doxygen{otb}{ObjectList} object and we template it over the type
//  of the readers.

  typedef otb::ObjectList<ImageReaderType> ReaderListType;

  ReaderListType::Pointer readerList = ReaderListType::New();

//  We will also build a list of input images in order to store the
//  smart pointers obtained at the output of each reader. This allows
//  us to build a pipeline without really reading the images and using
//  lots of RAM. The \doxygen{otb}{ImageList} object will be used.

  typedef otb::ImageList<InputImageType> ImageListType;

  ImageListType::Pointer imageList = ImageListType::New();

//  We can now loop over the input image list in order to populate the
//  reader list and the input image list.

  for (unsigned int i = 0; i < NbImages; ++i)
    {

    ImageReaderType::Pointer imageReader = ImageReaderType::New();

    imageReader->SetFileName(argv[i + 2]);

    std::cout << "Adding image " << argv[i + 2] << std::endl;

    imageReader->UpdateOutputInformation();

    imageList->PushBack(imageReader->GetOutput());

    readerList->PushBack(imageReader);

    }

//  All the input images will be concatenated into a single output
//  vector image. For this matter, we will use the
//  \doxygen{otb}{ImageListToVectorImageFilter} which is templated
//  over the input image list type and the output vector image type.

  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType>
  ImageListToVectorImageFilterType;

  ImageListToVectorImageFilterType::Pointer iL2VI =
    ImageListToVectorImageFilterType::New();

//  We plug the image list as input of the filter and use a
//  \doxygen{otb}{ImageFileWriter} to write the result image
//  to a file, so that the streaming capabilities of all the readers
//  and the filter are used.

  iL2VI->SetInput(imageList);

  typedef otb::ImageFileWriter<VectorImageType> ImageWriterType;

  ImageWriterType::Pointer imageWriter = ImageWriterType::New();

  imageWriter->SetFileName(argv[1]);

//  We can tune the size of the image tiles, so that the
//  total memory footprint of the pipeline is constant
//  for any execution of the program.

  unsigned long memoryConsumptionInMB = 10;

  std::cout << "Memory consumption: " << memoryConsumptionInMB << std::endl;

  imageWriter->SetAutomaticTiledStreaming(memoryConsumptionInMB);

  imageWriter->SetInput(iL2VI->GetOutput());

  imageWriter->Update();

  return EXIT_SUCCESS;
}
