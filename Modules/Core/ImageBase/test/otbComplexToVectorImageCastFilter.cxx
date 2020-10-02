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

#include "otbComplexToVectorImageCastFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


int otbComplexToVectorImageCastFilterTest(int itkNotUsed(argc), char* argv[])
{
  typedef std::complex<float> ComplexType;
  typedef otb::Image<ComplexType, 2>       CplxImageType;
  typedef otb::VectorImage<ComplexType, 2> VCplxImageType;
  typedef otb::VectorImage<float, 2>       VectorImageType;

  typedef otb::ComplexToVectorImageCastFilter<VCplxImageType, VectorImageType> VFilterType;
  typedef otb::ComplexToVectorImageCastFilter<CplxImageType, VectorImageType>  FilterType;

  typedef otb::ImageFileReader<CplxImageType>  ReaderType;
  typedef otb::ImageFileReader<VCplxImageType> VReaderType;

  typedef otb::ImageFileWriter<VectorImageType> WriterType;


  // Instantiating objects
  ReaderType::Pointer  reader  = ReaderType::New();
  VReaderType::Pointer vreader = VReaderType::New();
  WriterType::Pointer  writer  = WriterType::New();
  WriterType::Pointer  vwriter = WriterType::New();
  FilterType::Pointer  caster  = FilterType::New();
  VFilterType::Pointer vcaster = VFilterType::New();

  reader->SetFileName(argv[1]);
  vreader->SetFileName(argv[1]);

  caster->SetInput(reader->GetOutput());
  vcaster->SetInput(vreader->GetOutput());

  writer->SetFileName(argv[2]);
  writer->SetInput(caster->GetOutput());
  vwriter->SetFileName(argv[3]);
  vwriter->SetInput(vcaster->GetOutput());

  writer->Update();
  vwriter->Update();

  return EXIT_SUCCESS;
}
