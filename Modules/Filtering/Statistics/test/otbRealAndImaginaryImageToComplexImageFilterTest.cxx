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


#include "otbRealAndImaginaryImageToComplexImageFilter.h"
#include "itkMacro.h"
#include "otbImageFileReader.h"
#include "otbImage.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "otbStreamingCompareImageFilter.h"

int otbRealAndImaginaryImageToComplexImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char* inputFilenameRealPart = argv[1];
  const char* inputFilenameImagPart = argv[2];

  typedef double                  PixelType;
  typedef std::complex<PixelType> ComplexPixelType;
  const unsigned int              Dimension = 2;

  typedef otb::Image<PixelType, Dimension>        ImageType;
  typedef otb::Image<ComplexPixelType, Dimension> ComplexImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::RealAndImaginaryImageToComplexImageFilter<ImageType, ImageType, ComplexImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  ReaderType::Pointer readerRealPart = ReaderType::New();
  ReaderType::Pointer readerImagPart = ReaderType::New();

  readerRealPart->SetFileName(inputFilenameRealPart);
  readerImagPart->SetFileName(inputFilenameImagPart);
  readerRealPart->UpdateOutputInformation();
  readerImagPart->UpdateOutputInformation();


  filter->SetInputRealPart(readerRealPart->GetOutput());
  filter->SetInputImaginaryPart(readerImagPart->GetOutput());
  filter->Update();

  typedef itk::ComplexToRealImageFilter<ComplexImageType, ImageType> RealImageFilterType;

  RealImageFilterType::Pointer filterRealPart = RealImageFilterType::New();
  filterRealPart->SetInput(filter->GetOutput());

  typedef otb::StreamingCompareImageFilter<ImageType> CompareImageFilterType;
  CompareImageFilterType::Pointer                     compareRealImageFilter = CompareImageFilterType::New();

  compareRealImageFilter->SetInput1(filterRealPart->GetOutput());
  compareRealImageFilter->SetInput2(readerRealPart->GetOutput());
  compareRealImageFilter->Update();

  if (compareRealImageFilter->GetMSE() > 0.0000001)
  {
    std::cout << " Invalid RealPart ! MSE: " << compareRealImageFilter->GetMSE() << std::endl;
    return EXIT_FAILURE;
  }

  typedef itk::ComplexToImaginaryImageFilter<ComplexImageType, ImageType> ImaginaryImageFilterType;

  ImaginaryImageFilterType::Pointer filterImagPart = ImaginaryImageFilterType::New();
  filterImagPart->SetInput(filter->GetOutput());

  CompareImageFilterType::Pointer compareImagImageFilter = CompareImageFilterType::New();

  compareImagImageFilter->SetInput1(filterImagPart->GetOutput());
  compareImagImageFilter->SetInput2(readerImagPart->GetOutput());
  compareImagImageFilter->Update();

  if (compareImagImageFilter->GetMSE() > 0.0000001)
  {
    std::cout << " Invalid ImagPart ! MSE: " << compareImagImageFilter->GetMSE() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
