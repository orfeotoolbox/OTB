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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"
#include "otbStereoSensorModelToElevationMapFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbVectorImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkMedianImageFilter.h"

typedef otb::Image<short, 2> ImageType;
typedef otb::Image<float, 2> HeightImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> GaussianFilterType;
typedef otb::ImageFileWriter<ImageType> SingleWriterType;
typedef otb::StereoSensorModelToElevationFilter<ImageType, HeightImageType> StereoFilterType;
typedef otb::VectorImage<float, 2> VectorImageType;
typedef otb::ImageList<HeightImageType> ImageListType;
typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> IL2VIFilterType;
typedef itk::MedianImageFilter<HeightImageType, HeightImageType> MedianFilterType;
typedef otb::ImageFileWriter<HeightImageType> WriterType;




int otbStereoSensorModelToElevationMapFilter( int itkNotUsed(argc), char * argv[] )
{
  ReaderType::Pointer masterReader = ReaderType::New();
  masterReader->SetFileName(argv[1]);

  ReaderType::Pointer slaveReader = ReaderType::New();
  slaveReader->SetFileName(argv[2]);

  GaussianFilterType::ArrayType sigma;
  sigma.Fill(0.5*atof(argv[8]));

  GaussianFilterType::Pointer gaussian1 = GaussianFilterType::New();
  gaussian1->SetInput(masterReader->GetOutput());
  gaussian1->SetVariance(sigma);

  GaussianFilterType::Pointer gaussian2 = GaussianFilterType::New();
  gaussian2->SetInput(slaveReader->GetOutput());
  gaussian2->SetVariance(sigma);

  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[4]);
  otb::DEMHandler::Instance()->OpenGeoidFile(argv[5]);

  StereoFilterType::Pointer stereoFilter = StereoFilterType::New();
  stereoFilter->SetMasterInput(gaussian1->GetOutput());
  stereoFilter->SetSlaveInput(gaussian2->GetOutput());
  stereoFilter->SetLowerElevation(atof(argv[6]));
  stereoFilter->SetHigherElevation(atof(argv[7]));
  stereoFilter->SetElevationStep(atof(argv[8]));
  stereoFilter->SetRadius(atoi(argv[9]));

  otb::StandardFilterWatcher watcher(stereoFilter,"Performing stereo-matching");

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(stereoFilter->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  return EXIT_SUCCESS;
}
