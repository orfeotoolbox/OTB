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
#include "otbImageFileWriter.h"
#include "itkTranslationTransform.h"
#include "otbStreamingTraits.h"
#include "itkResampleImageFilter.h"


int otbStreamingResampleImageFilterCompareWithITK(int itkNotUsed(argc), char* argv[])
{
  const char*  inputFilename     = argv[1];
  unsigned int sizeXOutputImage  = atoi(argv[2]);
  unsigned int sizeYOutputImage  = atoi(argv[3]);
  const char*  outputITKFilename = argv[4];
  const char*  outputOTBFilename = argv[5];

  const unsigned int Dimension = 2;
  typedef double     InputPixelType;
  typedef double     OutputPixelType;
  typedef double     InterpolatorPrecisionType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  //      typedef otb::VectorImage<OutputPixelType, Dimension> InputVectorImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ImageFileWriter<OutputImageType> StreamingWriterType;
  typedef itk::TranslationTransform<InputPixelType, Dimension>                                    TransformType;
  typedef itk::NearestNeighborInterpolateImageFunction<InputImageType, InterpolatorPrecisionType> NNInterpolatorType;

  typedef itk::ResampleImageFilter<InputImageType, OutputImageType, InterpolatorPrecisionType>          ITKResampleImageFilterType;
  typedef otb::StreamingResampleImageFilter<InputImageType, OutputImageType, InterpolatorPrecisionType> OTBResampleImageFilterType;

  // Instantiating object
  ReaderType::Pointer          reader          = ReaderType::New();
  WriterType::Pointer          writerITKFilter = WriterType::New();
  StreamingWriterType::Pointer writerOTBFilter = StreamingWriterType::New();

  ITKResampleImageFilterType::Pointer resamplerITK = ITKResampleImageFilterType::New();
  OTBResampleImageFilterType::Pointer resamplerOTB = OTBResampleImageFilterType::New();

  TransformType::Pointer transform = TransformType::New();
  // Transformation creation
  TransformType::OutputVectorType translation;
  translation[0] = 30;
  translation[1] = 50;
  transform->SetOffset(translation);

  // Input Image
  reader->SetFileName(inputFilename);

  // Resamplers connected to input image
  resamplerITK->SetInput(reader->GetOutput());
  resamplerOTB->SetInput(reader->GetOutput());

  // Size of output resamplers result
  OTBResampleImageFilterType::SizeType size;
  size[0] = sizeXOutputImage;
  size[1] = sizeYOutputImage;
  resamplerITK->SetSize(size);
  resamplerOTB->SetOutputSize(size);

  // Set Interpolation
  NNInterpolatorType::Pointer interpolator = NNInterpolatorType::New();

  // Resamplers are updated with new interpolation (default is linear interpolation)
  resamplerITK->SetInterpolator(interpolator);
  resamplerOTB->SetInterpolator(interpolator);

  // Resamplers are updated with new transformation (default is identity)
  resamplerITK->SetTransform(transform);
  resamplerOTB->SetTransform(transform);

  // Result of resamplers is written
  writerITKFilter->SetInput(resamplerITK->GetOutput());
  writerITKFilter->SetFileName(outputITKFilename);
  writerITKFilter->Update();

  writerOTBFilter->SetInput(resamplerOTB->GetOutput());
  //      writerOTBFilter->SetAutomaticTiledStreaming();
  writerOTBFilter->SetFileName(outputOTBFilename);
  writerOTBFilter->SetNumberOfDivisionsStrippedStreaming(10);
  writerOTBFilter->Update();

  return EXIT_SUCCESS;
}
