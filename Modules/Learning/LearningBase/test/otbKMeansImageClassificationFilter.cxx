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

#include "otbKMeansImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbKMeansImageClassificationFilter(int itkNotUsed(argc), char* argv[])
{
  const char*        infname   = argv[1];
  const char*        outfname  = argv[2];
  const unsigned int nbClasses = atoi(argv[3]);

  const unsigned int     Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType, Dimension>                            ImageType;
  typedef otb::Image<LabeledPixelType, Dimension>                           LabeledImageType;
  typedef otb::KMeansImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
  typedef ClassificationFilterType::KMeansParametersType KMeansParametersType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType>         WriterType;

  // Instantiating object
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();

  const unsigned int   sampleSize    = ClassificationFilterType::MaxSampleDimension;
  const unsigned int   parameterSize = nbClasses * sampleSize;
  KMeansParametersType parameters;

  parameters.SetSize(parameterSize);
  parameters.Fill(0);

  for (unsigned int i = 0; i < nbClasses; ++i)
  {
    for (unsigned int j = 0; j < reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++j)
    {
      parameters[i * sampleSize + j] = atof(argv[4 + i * reader->GetOutput()->GetNumberOfComponentsPerPixel() + j]);
    }
  }

  std::cout << "Parameters: " << parameters << std::endl;

  filter->SetCentroids(parameters);
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
