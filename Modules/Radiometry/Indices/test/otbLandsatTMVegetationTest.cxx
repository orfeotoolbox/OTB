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

#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include <vector>
#include <algorithm>

int otbLandsatTMVegetationTest(int itkNotUsed(argc), char* argv[])
{

  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::VectorImage<InputPixelType, 2> InputImageType;
  typedef otb::Image<OutputPixelType, 2>      OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);

  typedef otb::Functor::LandsatTM::VegetationSpectralRule<InputImageType::PixelType, OutputPixelType> R1FunctorType;
  R1FunctorType r1Funct = R1FunctorType();
  r1Funct.SetDegree(otb::Functor::LandsatTM::HundredsKelvin);
  r1Funct.SetReflectance(otb::Functor::LandsatTM::Thousands);

  typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, R1FunctorType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());

  writer->Update();


  return EXIT_SUCCESS;
}
