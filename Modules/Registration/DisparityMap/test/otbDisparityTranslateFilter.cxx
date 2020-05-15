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

#include "otbDisparityTranslateFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"

typedef otb::Image<float, 2> FloatImageType;

typedef otb::VectorImage<float, 2> FloatVectorImageType;

typedef otb::ImageFileReader<FloatImageType> ReaderType;

typedef otb::ImageFileReader<FloatVectorImageType> ReaderVectorType;

typedef otb::ImageFileWriter<FloatVectorImageType> WriterType;

typedef otb::ImageFileWriter<FloatImageType> WriterScalarType;

typedef otb::ImageList<FloatImageType> ImageListType;

typedef otb::VectorImageToImageListFilter<FloatVectorImageType, ImageListType> VectorToListFilterType;

typedef otb::ImageListToVectorImageFilter<ImageListType, FloatVectorImageType> ListToVectorFilterType;

typedef otb::DisparityTranslateFilter<FloatImageType, FloatVectorImageType, FloatImageType, FloatImageType> TranslateFilter;


int otbDisparityTranslateFilter(int argc, char* argv[])
{
  if (argc < 6)
  {
    std::cout << "Usage: " << argv[0] << " dispMap_epi leftInvGrid rightDirectGrid left_sensor dispMap_sensor" << std::endl;
    return EXIT_FAILURE;
  }

  ReaderVectorType::Pointer dispReader = ReaderVectorType::New();
  dispReader->SetFileName(argv[1]);

  ReaderVectorType::Pointer leftGridReader = ReaderVectorType::New();
  leftGridReader->SetFileName(argv[2]);

  ReaderVectorType::Pointer rightGridReader = ReaderVectorType::New();
  rightGridReader->SetFileName(argv[3]);

  ReaderType::Pointer leftSensor = ReaderType::New();
  leftSensor->SetFileName(argv[4]);

  VectorToListFilterType::Pointer vectorToListFilter = VectorToListFilterType::New();
  vectorToListFilter->SetInput(dispReader->GetOutput());
  vectorToListFilter->UpdateOutputInformation();

  TranslateFilter::Pointer translator = TranslateFilter::New();
  translator->SetHorizontalDisparityMapInput(vectorToListFilter->GetOutput()->GetNthElement(0));
  translator->SetVerticalDisparityMapInput(vectorToListFilter->GetOutput()->GetNthElement(1));
  translator->SetInverseEpipolarLeftGrid(leftGridReader->GetOutput());
  translator->SetDirectEpipolarRightGrid(rightGridReader->GetOutput());
  translator->SetLeftSensorImageInput(leftSensor->GetOutput());

  ImageListType::Pointer outputList = ImageListType::New();
  outputList->PushBack(translator->GetHorizontalDisparityMapOutput());
  outputList->PushBack(translator->GetVerticalDisparityMapOutput());

  ListToVectorFilterType::Pointer listToVectorFilter = ListToVectorFilterType::New();
  listToVectorFilter->SetInput(outputList);

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(listToVectorFilter->GetOutput());
  writer->SetFileName(argv[5]);
  writer->Update();

  return EXIT_SUCCESS;
}
