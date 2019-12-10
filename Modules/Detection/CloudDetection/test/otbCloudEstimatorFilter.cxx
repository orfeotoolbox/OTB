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

#include "itkMacro.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbCloudEstimatorFunctor.h"
#include "otbCloudEstimatorFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbCloudEstimatorFilter(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
  typedef otb::Image<PixelType, Dimension>       ImageType;
  typedef VectorImageType::PixelType VectorPixelType;
  typedef otb::Functor::CloudEstimatorFunctor<VectorPixelType, PixelType> FunctorType;
  typedef otb::CloudEstimatorFilter<VectorImageType, ImageType, FunctorType> CloudEstimatorFilterType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  // Parameters
  const char*     inputFileName(argv[1]);
  const char*     outputFileName(argv[2]);
  VectorPixelType referencePixel;
  referencePixel.SetSize(4);
  referencePixel.Fill(0.);

  referencePixel[0] = (atof(argv[3]));
  referencePixel[1] = (atof(argv[4]));
  referencePixel[2] = (atof(argv[5]));
  referencePixel[3] = (atof(argv[6]));

  const double variance = (atof(argv[7]));

  // Instantiating object
  ReaderType::Pointer               reader         = ReaderType::New();
  CloudEstimatorFilterType::Pointer cloudEstimator = CloudEstimatorFilterType::New();
  WriterType::Pointer               writer         = WriterType::New();

  // Initialization parameters
  reader->SetFileName(inputFileName);

  cloudEstimator->SetInput(reader->GetOutput());
  cloudEstimator->SetReferencePixel(referencePixel);
  cloudEstimator->SetVariance(variance);

  writer->SetFileName(outputFileName);
  writer->SetInput(cloudEstimator->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
