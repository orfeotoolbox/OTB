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


#include "otbBoxAndWhiskerImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbEuclideanDistanceMetricWithMissingValuePow2.h"
#include "itkImageRegionIterator.h"

int otbBoxAndWhiskerImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  unsigned int RadiusX((unsigned int)::atoi(argv[3]));
  unsigned int RadiusY((unsigned int)::atoi(argv[4]));

  typedef double     PixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::BoxAndWhiskerImageFilter<ImageType> FilterType;

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  FilterType::Pointer filter = FilterType::New();

  filter->SetRadius(Radius);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();
  writer->SetFileName(outputFilename);

  typedef otb::Statistics::EuclideanDistanceMetricWithMissingValuePow2<ImageType::PixelType> OutlierType;

  filter->SetInput(reader->GetOutput());
  filter->Update();

  ImageType::Pointer binaryImage = ImageType::New();

  binaryImage->SetRegions(filter->GetOutput()->GetLargestPossibleRegion());
  binaryImage->SetNumberOfComponentsPerPixel(filter->GetOutput()->GetNumberOfComponentsPerPixel());
  binaryImage->Allocate();
  ImageType::PixelType p;
  p.SetSize(filter->GetOutput()->GetNumberOfComponentsPerPixel());
  p.Fill(0.0);
  binaryImage->FillBuffer(p);

  typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;
  typedef itk::ImageRegionIterator<ImageType>      IteratorType;


  ConstIteratorType inputIt(filter->GetOutput(), filter->GetOutput()->GetRequestedRegion());
  IteratorType      outputIt(binaryImage, filter->GetOutput()->GetRequestedRegion());

  for (inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt, ++outputIt)
  {
    const FilterType::PixelType pixel       = inputIt.Get();
    ImageType::PixelType        outputPixel = outputIt.Get();

    const unsigned int vectorSize = pixel.Size();
    for (unsigned i = 0; i < vectorSize; ++i)
    {
      if (OutlierType::IsMissingValue(pixel[i]))
      {
        outputPixel[i] = 1.0;
      }
    }
    outputIt.Set(outputPixel);
  }
  writer->SetInput(binaryImage);
  writer->Update();

  return EXIT_SUCCESS;
}
