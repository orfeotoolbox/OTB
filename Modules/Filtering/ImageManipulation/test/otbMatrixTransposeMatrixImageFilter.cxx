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
#include "otbStreamingMatrixTransposeMatrixImageFilter.h"

int otbMatrixTransposeMatrixImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char* infname1 = argv[1];
  const char* infname2 = argv[2];
  const char* outfname = argv[3];

  const unsigned int Dimension = 2;
  // typedef double InputPixelType;
  // typedef double OutputPixelType;
  typedef unsigned char InputPixelType;

  typedef otb::VectorImage<InputPixelType, Dimension>                                      InputImage1Type;
  typedef otb::VectorImage<InputPixelType, Dimension>                                      InputImage2Type;
  typedef otb::StreamingMatrixTransposeMatrixImageFilter<InputImage1Type, InputImage2Type> MatrixTransposeMatrixImageFilterType;
  typedef otb::ImageFileReader<InputImage1Type> ReaderType1;
  typedef otb::ImageFileReader<InputImage2Type> ReaderType2;

  // Instantiation
  MatrixTransposeMatrixImageFilterType::Pointer filter  = MatrixTransposeMatrixImageFilterType::New();
  ReaderType1::Pointer                          reader1 = ReaderType1::New();
  ReaderType2::Pointer                          reader2 = ReaderType2::New();

  reader1->SetFileName(infname1);
  reader2->SetFileName(infname2);

  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(10);
  filter->SetFirstInput(reader1->GetOutput());
  filter->SetSecondInput(reader2->GetOutput());
  filter->SetUsePadFirstInput(true);
  filter->SetUsePadSecondInput(true);

  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "transpose : " << filter->GetResult() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
