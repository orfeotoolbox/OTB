/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbChangeLabelImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"

int otbChangeLabelImageFilter(int itkNotUsed(argc), char* argv[])
{
  const unsigned int     ImageDimension = 2;
  const char*            inputFilename(argv[1]);
  const char*            outFilename(argv[2]);
  typedef unsigned short PixelType;
  typedef otb::Image<PixelType, ImageDimension>       InputImageType;
  typedef otb::VectorImage<PixelType, ImageDimension> OutputImageType;
  typedef InputImageType::PixelType  InputPixelType;
  typedef OutputImageType::PixelType OutputPixelType;
  typedef otb::ChangeLabelImageFilter<InputImageType, OutputImageType> FilterType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;

  FilterType::Pointer     filter    = FilterType::New();
  InputImageType::Pointer vectImage = InputImageType::New();

  WriterType::Pointer writer = WriterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outFilename);
  InputPixelType lower = static_cast<PixelType>(atoi(argv[3]));
  InputPixelType upper = static_cast<PixelType>(atoi(argv[4]));

  filter->SetNumberOfComponentsPerPixel(3);
  OutputPixelType background;
  background.SetSize(filter->GetNumberOfComponentsPerPixel());
  background[0] = itk::NumericTraits<PixelType>::max();
  background[1] = itk::NumericTraits<PixelType>::max();
  background[2] = 0;

  OutputPixelType zero = itk::NumericTraits<OutputPixelType>::ZeroValue(background);
  filter->SetChange(0, zero);
  for (InputPixelType i = lower; i <= upper; ++i)
  {
    filter->SetChange(i, background);
  }

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
