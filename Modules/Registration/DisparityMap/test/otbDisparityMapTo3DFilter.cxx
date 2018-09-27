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


#include "otbDisparityMapTo3DFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"

typedef otb::Image<float, 2>                    FloatImageType;

typedef otb::VectorImage<float, 2>              FloatVectorImageType;

typedef otb::DisparityMapTo3DFilter
  <FloatImageType,
    FloatVectorImageType,
    FloatVectorImageType,
    FloatImageType>                              StereoFilterType;


int otbDisparityMapTo3DFilter(int argc, char* argv[])
{
  typedef otb::ImageFileReader<FloatImageType>    ReaderType;

  typedef otb::ImageFileReader
    <FloatVectorImageType>                        ReaderVectorType;

  typedef otb::ImageFileWriter
    <FloatVectorImageType>                        WriterType;

  typedef otb::ImageList<FloatImageType>          ImageListType;

  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType,
     ImageListType>                               VectorToListFilterType;

  if (argc < 7)
    {
    std::cout << "Usage: "<<argv[0]<<" dispMap leftImage rightImage leftGrid rightGrid output3D [disparityMask]" << std::endl;
    return EXIT_FAILURE;
    }

  ReaderVectorType::Pointer dispReader = ReaderVectorType::New();
  dispReader->SetFileName(argv[1]);

  ReaderType::Pointer leftReader = ReaderType::New();
  leftReader->SetFileName(argv[2]);
  leftReader->UpdateOutputInformation();

  ReaderType::Pointer rightReader = ReaderType::New();
  rightReader->SetFileName(argv[3]);
  rightReader->UpdateOutputInformation();

  ReaderVectorType::Pointer leftGridReader = ReaderVectorType::New();
  leftGridReader->SetFileName(argv[4]);

  ReaderVectorType::Pointer rightGridReader = ReaderVectorType::New();
  rightGridReader->SetFileName(argv[5]);

  ReaderType::Pointer maskReader = ReaderType::New();
  if (argc == 8)
    {
    maskReader->SetFileName(argv[7]);
    }

  VectorToListFilterType::Pointer vectorToListFilter = VectorToListFilterType::New();
  vectorToListFilter->SetInput(dispReader->GetOutput());
  vectorToListFilter->UpdateOutputInformation();

  StereoFilterType::Pointer filter = StereoFilterType::New();
  filter->SetHorizontalDisparityMapInput(vectorToListFilter->GetOutput()->GetNthElement(0));
  filter->SetVerticalDisparityMapInput(vectorToListFilter->GetOutput()->GetNthElement(1));
  filter->SetLeftKeywordList(leftReader->GetOutput()->GetImageKeywordlist());
  filter->SetRightKeywordList(rightReader->GetOutput()->GetImageKeywordlist());
  filter->SetLeftEpipolarGridInput(leftGridReader->GetOutput());
  filter->SetRightEpipolarGridInput(rightGridReader->GetOutput());

  if (argc == 8)
    {
    filter->SetDisparityMaskInput(maskReader->GetOutput());
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[6]);
  writer->Update();

  return EXIT_SUCCESS;
}
