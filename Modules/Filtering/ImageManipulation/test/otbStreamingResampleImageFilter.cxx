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


#include "otbStreamingResampleImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkTranslationTransform.h"

int otbStreamingResampleImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  const unsigned int    Dimension = 2;
  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  typedef double        InterpolatorPrecisionType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef itk::TranslationTransform<InputPixelType, Dimension> TransformType;
  typedef otb::StreamingResampleImageFilter<InputImageType, OutputImageType, InterpolatorPrecisionType> StreamingResampleImageFilterType;

  // Instantiating object
  ReaderType::Pointer                       reader    = ReaderType::New();
  WriterType::Pointer                       writer    = WriterType::New();
  StreamingResampleImageFilterType::Pointer resampler = StreamingResampleImageFilterType::New();
  TransformType::Pointer                    transform = TransformType::New();

  // Input Image
  reader->SetFileName(inputFilename);

  // Resampler connected to input image
  resampler->SetInput(reader->GetOutput());

  // Size of output resampler result
  StreamingResampleImageFilterType::SizeType size;
  size[0] = 600;
  size[1] = 600;
  resampler->SetOutputSize(size);

  // Transformation creation
  TransformType::OutputVectorType translation;
  translation[0] = 10;
  translation[1] = 20;
  transform->SetOffset(translation);

  // Resampler is updated with new transformation (default is identity)
  resampler->SetTransform(transform);

  // Result of resampler is written
  writer->SetInput(resampler->GetOutput());
  writer->SetNumberOfDivisionsStrippedStreaming(1);
  writer->SetFileName(outputFilename);

  writer->Update();

  return EXIT_SUCCESS;
}
