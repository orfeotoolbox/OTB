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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbVectorImageTo3DScalarImageFilter.h"

int main(int itkNotUsed(argc), char** argv)
{
  typedef otb::Image<double, 3>       Image3DType;
  typedef otb::VectorImage<double, 2> VectorImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(argv[1]);

  typedef otb::VectorImageTo3DScalarImageFilter<VectorImageType, Image3DType> VectorImageTo3DScalarImageFilterType;
  VectorImageTo3DScalarImageFilterType::Pointer filter1 = VectorImageTo3DScalarImageFilterType::New();

  typedef otb::StreamingStatisticsImageFilter<Image3DType> statFilterType;
  statFilterType::Pointer                                  statFilter = statFilterType::New();

  filter1->SetInput(reader1->GetOutput());
  //   filter1->Update();
  statFilter->SetInput(filter1->GetOutput());
  statFilter->Update();

  std::cout << statFilter->GetMean() << std::endl;
}
