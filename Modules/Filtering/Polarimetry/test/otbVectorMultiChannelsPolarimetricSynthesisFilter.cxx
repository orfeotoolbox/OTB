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
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelsPolarimetricSynthesisFilter.h"

int otbVectorMultiChannelsPolarimetricSynthesisFilter(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  double PsiI = strtod(argv[3], nullptr);
  double KhiI = strtod(argv[4], nullptr);
  double PsiR = strtod(argv[5], nullptr);
  double KhiR = strtod(argv[6], nullptr);

  typedef std::complex<double> InputPixelType;
  typedef double               OutputPixelType;
  const unsigned int           Dimension = 2;
  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>      OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::MultiChannelsPolarimetricSynthesisFilter<InputImageType, OutputImageType> FilterType;

  FilterType::Pointer polarimetricSynthesis = FilterType::New();

  polarimetricSynthesis->SetPsiI(PsiI);
  polarimetricSynthesis->SetKhiI(KhiI);
  polarimetricSynthesis->SetPsiR(PsiR);
  polarimetricSynthesis->SetKhiR(KhiR);
  polarimetricSynthesis->SetEmissionH(true);
  polarimetricSynthesis->SetEmissionV(true);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  polarimetricSynthesis->SetInput(reader->GetOutput());
  writer->SetInput(polarimetricSynthesis->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
