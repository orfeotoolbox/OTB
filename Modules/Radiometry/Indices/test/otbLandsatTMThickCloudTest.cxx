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
#include "otbLandsatTMIndices.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include <vector>
#include <algorithm>

int otbLandsatTMThickCloudTest(int itkNotUsed(argc), char* argv[])
{

  typedef double InputPixelType;
  typedef double OutputPixelType;

  typedef otb::VectorImage<InputPixelType, 2> InputImageType;
  typedef otb::Image<OutputPixelType, 2>      OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);

  typedef otb::Functor::LandsatTM::ThickCloudsSpectralRule<InputImageType::PixelType, OutputPixelType> R1FunctorType;

  typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, R1FunctorType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  (filter->GetFunctor()).SetDegree(otb::Functor::LandsatTM::HundredsKelvin);
  (filter->GetFunctor()).SetReflectance(otb::Functor::LandsatTM::Thousands);
  (filter->GetFunctor()).SetSAT(otb::Functor::LandsatTM::L5);

  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());

  writer->Update();


  return EXIT_SUCCESS;
}
