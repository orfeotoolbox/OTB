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




#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbAmplitudePhaseToRGBFunctor.h"
#include "otbFunctorImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkComplexToPhaseImageFilter.h"
#include "itkShiftScaleImageFilter.h"

int otbAmplitudePhaseToRGBFunctor(int itkNotUsed(argc), char * argv[])
{
  typedef float                    PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef std::complex<PixelType>         ComplexPixelType;
  typedef otb::Image<ComplexPixelType, 2> ComplexImageType;

  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>  RGBImageType;

  typedef otb::ImageFileReader<ComplexImageType>      ReaderType;
  typedef otb::ImageFileWriter<RGBImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  typedef itk::ComplexToModulusImageFilter<ComplexImageType, ImageType> ModulusFilterType;
  ModulusFilterType::Pointer modulusFilter = ModulusFilterType::New();
  modulusFilter->SetInput(reader->GetOutput());

  typedef itk::ComplexToPhaseImageFilter<ComplexImageType, ImageType> PhaseFilterType;
  PhaseFilterType::Pointer phaseFilter = PhaseFilterType::New();
  phaseFilter->SetInput(reader->GetOutput());

  typedef itk::ShiftScaleImageFilter<ImageType, ImageType> ConstFilterType;
  ConstFilterType::Pointer constFilter = ConstFilterType::New();
  constFilter->SetScale(0.0);
  constFilter->SetInput(modulusFilter->GetOutput());

  typedef otb::Functor::AmplitudePhaseToRGBFunctor
  <PixelType, PixelType, PixelType, RGBPixelType> ColorMapFunctorType;
  typedef otb::FunctorImageFilter<ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->GetModifiableFunctor().SetMaximum(4000);
  colormapper->GetModifiableFunctor().SetMinimum(0);

  colormapper->SetInput1(modulusFilter->GetOutput());
  colormapper->SetInput2(constFilter->GetOutput());
  colormapper->SetInput3(phaseFilter->GetOutput());
  colormapper->SetNumberOfThreads(1);

  writer->SetInput(colormapper->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
