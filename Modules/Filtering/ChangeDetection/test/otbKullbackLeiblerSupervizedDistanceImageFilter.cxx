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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbKullbackLeiblerSupervizedDistanceImageFilter.h"
#include "otbCommandProgressUpdate.h"

int otbKullbackLeiblerSupervizedDistanceImageFilter(int argc, char* argv[])
{
  const unsigned int Dimension = 2;

  typedef double        InputPixelType;
  typedef unsigned char TrainingPixelType;

  typedef otb::Image<InputPixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef otb::Image<TrainingPixelType, Dimension> TrainingImageType;
  typedef otb::ImageFileReader<TrainingImageType> TrainingReaderType;

  if (argc != 6)
  {
    std::cout << "Invalid number of arguiments !!" << std::endl;
    return EXIT_FAILURE;
  }
  const char* inputImageFileName1        = argv[1];
  const char* inputImageFileName2        = argv[2];
  const char* inputTrainingImageFileName = argv[3];
  const int   winSize(atoi(argv[4]));
  const char* outputImageFileName = argv[5];

  ImageType::Pointer  img1    = ImageType::New();
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName(inputImageFileName1);
  // reader1->Update();
  img1 = reader1->GetOutput();

  ImageType::Pointer  img2    = ImageType::New();
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(inputImageFileName2);
  // reader2->Update();
  img2 = reader2->GetOutput();

  TrainingImageType::Pointer  imgRoi    = TrainingImageType::New();
  TrainingReaderType::Pointer readerRoi = TrainingReaderType::New();
  readerRoi->SetFileName(inputTrainingImageFileName);
  // readerRoi->Update();
  imgRoi = readerRoi->GetOutput();

  typedef otb::KullbackLeiblerSupervizedDistanceImageFilter<ImageType, ImageType, TrainingImageType, ImageType> FilterType;

  FilterType::Pointer changeDetector = FilterType::New();
  changeDetector->SetRadius((winSize - 1) / 2);
  changeDetector->SetInput1(img1);
  changeDetector->SetInput2(img2);

  // Once img1 and img2 are connected, it is time to connect imgRoi
  // to perform the parameters estimation of the non-changed area.

  changeDetector->SetTrainingArea(imgRoi);

  typedef otb::CommandProgressUpdate<FilterType> CommandType;
  CommandType::Pointer                           observer = CommandType::New();
  changeDetector->AddObserver(itk::ProgressEvent(), observer);

  typedef unsigned char OutputPixelType;
  typedef otb::Image<OutputPixelType, Dimension>                       OutputImageType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(changeDetector->GetOutput());

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer                           writer = WriterType::New();
  writer->SetFileName(outputImageFileName);
  writer->SetInput(rescaler->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
