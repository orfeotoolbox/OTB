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
#include "itkMacro.h"

#include "otbStreamingResampleImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkTranslationTransform.h"

int otbStreamingResampleImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char* inputFilename = argv[1];
  const char* outputFilename = argv[2];

  const unsigned int Dimension = 2;
  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  typedef double        InterpolatorPrecisionType;

  typedef otb::Image<InputPixelType, Dimension>                InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>               OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                 ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>       WriterType;
  typedef itk::TranslationTransform<InputPixelType, Dimension> TransformType;
  typedef otb::StreamingResampleImageFilter<InputImageType, OutputImageType,
      InterpolatorPrecisionType> StreamingResampleImageFilterType;

  // Instantiating object
  ReaderType::Pointer                       reader = ReaderType::New();
  WriterType::Pointer                       writer = WriterType::New();
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
