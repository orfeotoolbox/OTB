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


#include "otbHaralickTexturesImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

typedef unsigned short InputPixelType;
const unsigned int     Dimension = 2;

typedef otb::Image<InputPixelType, Dimension> InputImageType;
typedef otb::ImageFileReader<InputImageType> ReaderType;

typedef otb::HaralickTexturesImageFunction<InputImageType, double> HaralickTexturesImageFunctionType;
typedef HaralickTexturesImageFunctionType::PointType  PointType;
typedef HaralickTexturesImageFunctionType::OutputType OutputType;


int otbHaralickTexturesImageFunction(int itkNotUsed(argc), char* argv[])
{
  // Read the input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  std::ofstream outputStream(argv[2]);

  HaralickTexturesImageFunctionType::Pointer haralick = HaralickTexturesImageFunctionType::New();
  haralick->SetInputImage(reader->GetOutput());
  haralick->SetNeighborhoodRadius(10);

  HaralickTexturesImageFunctionType::OffsetType offset;
  offset.Fill(1);
  haralick->SetOffset(offset);

  PointType p;
  p[0] = atof(argv[3]);
  p[1] = atof(argv[4]);

  OutputType output = haralick->Evaluate(p);

  outputStream << "Evaluate(" << p << ") = " << output << std::endl;

  outputStream.close();

  return EXIT_SUCCESS;
}
