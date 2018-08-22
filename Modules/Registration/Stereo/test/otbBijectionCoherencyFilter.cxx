/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbBijectionCoherencyFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"

typedef otb::Image<float, 2>                    FloatImageType;

typedef otb::VectorImage<float, 2>              FloatVectorImageType;

typedef otb::BijectionCoherencyFilter
  <FloatImageType,
    FloatImageType>                             BijectionFilterType;



int otbBijectionCoherencyFilter(int argc, char* argv[])
{

  typedef otb::ImageFileReader
    <FloatVectorImageType>                        ReaderVectorType;

  typedef otb::ImageFileWriter
    <FloatImageType>                              WriterScalarType;

  typedef otb::ImageList<FloatImageType>          ImageListType;

  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType,
     ImageListType>                               VectorToListFilterType;

  if (argc < 8)
    {
    std::cout << "Usage: "<<argv[0]<<" left2right right2left minHDisp maxHDisp minVDisp maxVDisp output" << std::endl;
    return EXIT_FAILURE;
    }

  ReaderVectorType::Pointer l2rReader = ReaderVectorType::New();
  l2rReader->SetFileName(argv[1]);

  ReaderVectorType::Pointer r2lReader = ReaderVectorType::New();
  r2lReader->SetFileName(argv[2]);

  VectorToListFilterType::Pointer vectorToListFilter1 = VectorToListFilterType::New();
  vectorToListFilter1->SetInput(l2rReader->GetOutput());
  vectorToListFilter1->UpdateOutputInformation();

  VectorToListFilterType::Pointer vectorToListFilter2 = VectorToListFilterType::New();
  vectorToListFilter2->SetInput(r2lReader->GetOutput());
  vectorToListFilter2->UpdateOutputInformation();

  BijectionFilterType::Pointer bijectFilter = BijectionFilterType::New();
  bijectFilter->SetDirectHorizontalDisparityMapInput(vectorToListFilter1->GetOutput()->GetNthElement(0));
  bijectFilter->SetReverseHorizontalDisparityMapInput(vectorToListFilter2->GetOutput()->GetNthElement(0));
  bijectFilter->SetMinHDisp(atoi(argv[3]));
  bijectFilter->SetMaxHDisp(atoi(argv[4]));
  bijectFilter->SetMinVDisp(atoi(argv[5]));
  bijectFilter->SetMaxVDisp(atoi(argv[6]));

  WriterScalarType::Pointer writer = WriterScalarType::New();
  writer->SetInput(bijectFilter->GetOutput());
  writer->SetFileName(argv[7]);
  writer->Update();

  return EXIT_SUCCESS;
}
