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


#include "otbHooverMatrixFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkLabelImageToLabelMapFilter.h"

int otbHooverMatrixFilter(int argc, char* argv[])
{
  typedef itk::LabelObject<unsigned int, 2> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>        LabelMapType;
  typedef otb::HooverMatrixFilter<LabelMapType> HooverMatrixFilterType;
  typedef otb::Image<unsigned int, 2>                              ImageType;
  typedef itk::LabelImageToLabelMapFilter<ImageType, LabelMapType> ImageToLabelMapFilterType;
  typedef otb::ImageFileReader<ImageType>    ImageReaderType;
  typedef HooverMatrixFilterType::MatrixType MatrixType;

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " segmentationGT segmentationMS HooverMatrix.txt" << std::endl;
    return EXIT_FAILURE;
  }

  ImageReaderType::Pointer gt_reader = ImageReaderType::New();
  gt_reader->SetFileName(argv[1]);

  ImageReaderType::Pointer ms_reader = ImageReaderType::New();
  ms_reader->SetFileName(argv[2]);

  ImageToLabelMapFilterType::Pointer gt_filter = ImageToLabelMapFilterType::New();
  gt_filter->SetInput(gt_reader->GetOutput());
  gt_filter->SetBackgroundValue(0);

  ImageToLabelMapFilterType::Pointer ms_filter = ImageToLabelMapFilterType::New();
  ms_filter->SetInput(ms_reader->GetOutput());
  ms_filter->SetBackgroundValue(0);

  HooverMatrixFilterType::Pointer hooverFilter = HooverMatrixFilterType::New();
  hooverFilter->SetGroundTruthLabelMap(gt_filter->GetOutput());
  hooverFilter->SetMachineSegmentationLabelMap(ms_filter->GetOutput());

  hooverFilter->Update();

  std::ofstream outputFile;
  outputFile.open(argv[3]);

  MatrixType&  mat = hooverFilter->GetHooverConfusionMatrix();
  unsigned int n = mat.Rows(), p = mat.Cols();
  for (unsigned int i = 0; i < n; i++)
  {
    for (unsigned int j = 0; j < p; j++)
    {
      outputFile << mat(i, j);
      if ((j + 1) == p)
      {
        outputFile << "\n";
      }
      else
      {
        outputFile << "\t";
      }
    }
  }

  return EXIT_SUCCESS;
}
