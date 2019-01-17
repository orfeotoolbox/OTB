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

#include "otbTwoNRIBandsImageToNComplexBandsImage.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbTwoNRIBandsImageToNComplexBandsImage(int itkNotUsed(argc), char * argv[])
{

  typedef double PixelType;
  typedef otb::VectorImage<PixelType,  2> InputImageType;
    
  typedef std::complex<double> OutputPixelType;
  typedef otb::VectorImage<OutputPixelType, 2> OutputImageType;
 
 
  typedef otb::TwoNRIBandsImageToNComplexBandsImage<InputImageType, OutputImageType> FilterType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  WriterType::Pointer writer = WriterType::New();
  
  
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  filter->SetInput(0,reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();



  return EXIT_SUCCESS;
}
