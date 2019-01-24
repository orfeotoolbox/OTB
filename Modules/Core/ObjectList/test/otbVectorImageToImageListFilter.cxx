/*
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

#include "itkMacro.h"

#include "otbVectorImageToImageListFilter.h"
#include "otbVectorImage.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbVectorImageToImageListFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  char *             infname   = argv[1];
  char *             outfname1 = argv[2];
  char *             outfname2 = argv[3];
  char *             outfname3 = argv[4];

  typedef unsigned char                          PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::ImageList<ImageType>              ImageListType;

  // IO
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;

  // Instantiating object
  VectorImageToImageListFilterType::Pointer filter = VectorImageToImageListFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());

  filter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname1);
  writer->SetInput(filter->GetOutput()->GetNthElement(0));
  writer->Update();

  writer = WriterType::New();
  writer->SetFileName(outfname2);
  writer->SetInput(filter->GetOutput()->GetNthElement(1));
  writer->Update();

  writer = WriterType::New();
  writer->SetFileName(outfname3);
  writer->SetInput(filter->GetOutput()->GetNthElement(2));
  writer->Update();

  return EXIT_SUCCESS;
}

int otbVectorImageToImageListFilterIterator(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  const char *       infname   = argv[1];
  const char *       outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];

  typedef unsigned char                          PixelType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::ImageList<ImageType>              ImageListType;

  // IO
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  typedef otb::VectorImageToImageListFilter<VectorImageType, ImageListType> VectorImageToImageListFilterType;

  typedef VectorImageToImageListFilterType::OutputImageListType::Iterator ImageListIterator;

  // Instantiating object

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  VectorImageToImageListFilterType::Pointer filter = VectorImageToImageListFilterType::New();
  filter->SetInput(reader->GetOutput());
  // We need to do that to fill the Imagelist before update and use it as input of other filter
  filter->UpdateOutputInformation();

  ImageListIterator itOutput = filter->GetOutput()->Begin();

  WriterType::Pointer writer = WriterType::New();

  int i = 1;

  while ((itOutput != filter->GetOutput()->End()) )
    {
    std::stringstream oss;
    oss << outputFilenamePrefix << "Band" << i << "." << outputFilenameSuffix;

    writer->SetInput(itOutput.Get());

    writer->SetFileName(oss.str());
    writer->SetNumberOfDivisionsTiledStreaming(8);
    writer->Update();

    ++itOutput;
    ++i;
    }

  /*
  while ((itOutput != filter->GetOutput()->End()) )
    {
    std::stringstream oss;
    WriterType::Pointer writer = WriterType::New();

    oss << outputFilenamePrefix << "Band" << i << "." << outputFilenameSuffix;

    writer->SetInput(itOutput.Get());

    writer->SetFileName(oss.str());

    writer->Update();

    ++itOutput;
    ++i;
    }
    */


  return EXIT_SUCCESS;
}
