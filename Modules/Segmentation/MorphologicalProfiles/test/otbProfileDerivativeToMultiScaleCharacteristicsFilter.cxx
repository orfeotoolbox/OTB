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

#include "otbMorphologicalOpeningProfileFilter.h"
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbProfileDerivativeToMultiScaleCharacteristicsFilter(int itkNotUsed(argc), char* argv[])
{
  const char*        inputFilename   = argv[1];
  const char*        outputFilename1 = argv[2];
  const char*        outputFilename2 = argv[3];
  const unsigned int profileSize     = atoi(argv[4]);
  const unsigned int initialValue    = atoi(argv[5]);
  const unsigned int step            = atoi(argv[6]);

  const unsigned int     Dimension = 2;
  typedef double         InputPixelType;
  typedef double         OutputPixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<LabeledPixelType, 2>        LabeledImageType;

  typedef otb::ImageFileReader<InputImageType>   ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;
  typedef otb::ImageFileWriter<LabeledImageType> LabeledWriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InputImageType, StructuringElementType> OpeningProfileFilterType;
  typedef otb::ProfileToProfileDerivativeFilter<InputImageType, InputImageType> DerivativeFilterType;
  typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InputImageType, OutputImageType, LabeledImageType> MultiScaleCharacteristicsFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  OpeningProfileFilterType::Pointer profileFilter = OpeningProfileFilterType::New();
  profileFilter->SetInput(reader->GetOutput());
  profileFilter->SetProfileSize(profileSize);
  profileFilter->SetInitialValue(initialValue);
  profileFilter->SetStep(step);

  DerivativeFilterType::Pointer derivativeFilter = DerivativeFilterType::New();
  derivativeFilter->SetInput(profileFilter->GetOutput());

  MultiScaleCharacteristicsFilterType::Pointer msCharFilter = MultiScaleCharacteristicsFilterType::New();
  msCharFilter->SetInput(derivativeFilter->GetOutput());
  msCharFilter->SetInitialValue(initialValue);
  msCharFilter->SetStep(step);

  WriterType::Pointer        writer        = WriterType::New();
  LabeledWriterType::Pointer labeledWriter = LabeledWriterType::New();

  writer->SetFileName(outputFilename1);
  writer->SetInput(msCharFilter->GetOutput());
  writer->Update();

  labeledWriter->SetFileName(outputFilename2);
  labeledWriter->SetInput(msCharFilter->GetOutputCharacteristics());
  labeledWriter->Update();

  return EXIT_SUCCESS;
}
