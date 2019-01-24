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

#include "otbRAndNIRIndexImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbWDVIRAndNIRVegetationIndexImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                           PixelType;
  typedef otb::Image<PixelType, Dimension> InputRImageType;
  typedef otb::Image<PixelType, Dimension> InputNIRImageType;
  typedef otb::Image<double, Dimension>    OutputImageType;

  typedef otb::ImageFileReader<InputRImageType>   RReaderType;
  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  typedef otb::Functor::WDVI<InputRImageType::PixelType,
      InputNIRImageType::PixelType,
      OutputImageType::PixelType> FunctorType;

  typedef otb::RAndNIRIndexImageFilter<InputRImageType,
      InputNIRImageType,
      OutputImageType,
      FunctorType> RAndNIRIndexImageFilterType;

  // Instantiating object
  RAndNIRIndexImageFilterType::Pointer filter = RAndNIRIndexImageFilterType::New();
  RReaderType::Pointer                 readerR = RReaderType::New();
  NIRReaderType::Pointer               readerNIR = NIRReaderType::New();
  WriterType::Pointer                  writer = WriterType::New();

  const char * inputFilenameR  = argv[1];
  const char * inputFilenameNIR  = argv[2];
  const char * outputFilename = argv[3];

  double s(::atof(argv[4]));

  readerR->SetFileName(inputFilenameR);
  readerNIR->SetFileName(inputFilenameNIR);
  writer->SetFileName(outputFilename);
  filter->SetInputR(readerR->GetOutput());
  filter->SetInputNIR(readerNIR->GetOutput());

  filter->GetFunctor().SetS(s);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
