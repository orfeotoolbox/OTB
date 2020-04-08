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

#include "itkShiftScaleImageFilter.h"
#include "otbStreamingCompareImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include <fstream>
#include "otbStreamingTraits.h"


int otbStreamingCompareImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef double     PixelType;

  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::StreamingCompareImageFilter<ImageType> StreamingCompareImageFilterType;
  typedef itk::ShiftScaleImageFilter<ImageType, ImageType> ShiftFilterType;

  // Instantiating object
  StreamingCompareImageFilterType::Pointer filter = StreamingCompareImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ShiftFilterType::Pointer shiftFilter = ShiftFilterType::New();
  shiftFilter->SetInput(reader->GetOutput());
  shiftFilter->SetScale(1);
  shiftFilter->SetShift(10);

  filter->SetInput1(reader->GetOutput());
  filter->SetInput2(shiftFilter->GetOutput());
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "MSE: " << filter->GetMSE() << std::endl;
  file << "MAE: " << filter->GetMAE() << std::endl;
  file << "PSNR: " << filter->GetPSNR() << std::endl;
  file << "Count: " << filter->GetDiffCount() << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
