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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbNRIBandImagesToOneNComplexBandsImage.h"

#include "otbReciprocalPauliDecompImageFilter.h"


int otbReciprocalPauliDecompImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilenameHH = argv[1];
  const char* inputFilenameHV = argv[2];
  const char* inputFilenameVV = argv[3];
  const char* outputFilename  = argv[4];


  typedef std::complex<double> ComplexPixelType;
  const unsigned int           Dimension = 2;


  typedef otb::VectorImage<ComplexPixelType, Dimension> ComplexVectorImageType;
  typedef otb::VectorImage<double, Dimension>           RealVectorImageType;

  typedef otb::NRIBandImagesToOneNComplexBandsImage<RealVectorImageType, ComplexVectorImageType> NRITOOneCFilterType;

  typedef otb::ImageFileReader<RealVectorImageType>    ReaderType;
  typedef otb::ImageFileWriter<ComplexVectorImageType> WriterType;

  typedef otb::ReciprocalPauliDecompImageFilter<ComplexVectorImageType, ComplexVectorImageType> FilterType;

  ReaderType::Pointer          readerHH        = ReaderType::New();
  ReaderType::Pointer          readerHV        = ReaderType::New();
  ReaderType::Pointer          readerVV        = ReaderType::New();
  NRITOOneCFilterType::Pointer nriToOneCfilter = NRITOOneCFilterType::New();
  WriterType::Pointer          writer          = WriterType::New();
  FilterType::Pointer          paulifilter     = FilterType::New();

  readerHH->SetFileName(inputFilenameHH);
  readerHV->SetFileName(inputFilenameHV);
  readerVV->SetFileName(inputFilenameVV);

  nriToOneCfilter->SetInput(0, readerHH->GetOutput());
  nriToOneCfilter->SetInput(1, readerHV->GetOutput());
  nriToOneCfilter->SetInput(2, readerVV->GetOutput());

  paulifilter->SetInput<0>(nriToOneCfilter->GetOutput());

  writer->SetFileName(outputFilename);
  writer->SetInput(paulifilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
