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


#include "otbMatrixImageFilter.h"

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include <complex>


int otbMatrixImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char*  inputFilename  = argv[1];
  const char*  outputFilename = argv[2];
  unsigned int nbCol          = atoi(argv[3]);

  typedef double PixelType;

  typedef otb::VectorImage<PixelType> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::MatrixImageFilter<ImageType, ImageType> FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);


  FilterType::MatrixType mat(reader->GetOutput()->GetNumberOfComponentsPerPixel(), nbCol, 0.);

  double val = 0;
  for (unsigned int i = 0; i < mat.rows(); ++i)
  {
    for (unsigned int j = 0; j < mat.cols(); ++j)
    {
      mat[i][j] = val;
      val += 0.5;
    }
  }


  filter->SetInput(reader->GetOutput());
  filter->SetMatrix(mat);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
