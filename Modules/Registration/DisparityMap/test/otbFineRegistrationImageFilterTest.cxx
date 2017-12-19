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



#include "itkFixedArray.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbFineRegistrationImageFilter.h"
#include "otbStandardFilterWatcher.h"
#include "otbStopwatch.h"
#include "otbExtractROI.h"


#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkMeanReciprocalSquareDifferenceImageToImageMetric.h"
#include "itkMeanSquaresImageToImageMetric.h"

int otbFineRegistrationImageFilterTest( int argc, char * argv[] )
{
  if(argc!=16)
    {
    std::cerr<<"Usage: "<<argv[0]<<" fixed_fname moving_fname output_correl output_field radius search_radius ";
    std::cerr<<"subpixPrecision metric(0=CC, 1=NCC, 2=MeanSquare, 3=Mean reciprocal square difference) ";
    std::cerr<<"gridStep offsetX offsetY"<<std::endl;
    std::cerr<<"ROI : indexX, indexY, startX, startY"<<std::endl;
    return EXIT_FAILURE;
    }
  const char * fixedFileName  = argv[1];
  const char * movingFileName = argv[2];
  const char * correlFileName = argv[3];
  const char * fieldFileName  = argv[4];
  const unsigned int radius   = atoi(argv[5]);
  const unsigned int sradius  = atoi(argv[6]);
  const double      precision = atof(argv[7]);
  const unsigned int metric   = atoi(argv[8]);
  const unsigned int gridStep = atoi(argv[9]);
  const double       offsetx  = atof(argv[10]);
  const double       offsety  = atof(argv[11]);
  const unsigned int startX   = atoi(argv[12]);
  const unsigned int startY   = atoi(argv[13]);
  const unsigned int sizeX    = atoi(argv[14]);
  const unsigned int sizeY    = atoi(argv[15]);

  typedef double      PixelType;
  const unsigned int  Dimension = 2;

  typedef itk::FixedArray<PixelType, Dimension>                                 DisplacementValueType;
  typedef otb::Image< PixelType,  Dimension >                                  ImageType;
  typedef otb::Image<DisplacementValueType, Dimension>                           FieldImageType;
  typedef otb::ImageFileReader< ImageType >                                    ReaderType;
  typedef otb::ImageFileWriter< ImageType >                           CorrelWriterType;
  typedef otb::ImageFileWriter< FieldImageType>                       FieldWriterType;
  typedef otb::ExtractROI<PixelType, PixelType>                                ExtractFiltertype;
  typedef otb::FineRegistrationImageFilter<ImageType, ImageType, FieldImageType> RegistrationFilterType;

  ReaderType::Pointer freader = ReaderType::New();
  freader->SetFileName(fixedFileName);
  //freader->Update();

  ReaderType::Pointer mreader = ReaderType::New();
  mreader->SetFileName(movingFileName);
  //mreader->Update();

  ExtractFiltertype::Pointer fextract = ExtractFiltertype::New();
  fextract->SetInput(freader->GetOutput());
  fextract->SetStartX(startX);
  fextract->SetStartY(startY);
  fextract->SetSizeX(sizeX);
  fextract->SetSizeY(sizeY);
  ExtractFiltertype::Pointer mextract = ExtractFiltertype::New();
  mextract->SetInput(mreader->GetOutput());
  mextract->SetStartX(startX);
  mextract->SetStartY(startY);
  mextract->SetSizeX(sizeX);
  mextract->SetSizeY(sizeY);

  RegistrationFilterType::Pointer registration = RegistrationFilterType::New();
  registration->SetFixedInput(/*freader*/fextract->GetOutput());
  registration->SetMovingInput(/*mreader*/mextract->GetOutput());
  registration->SetRadius(radius);
  registration->SetSearchRadius(sradius);
  registration->SetConvergenceAccuracy(precision);
  registration->SetGridStep(gridStep);

  RegistrationFilterType::SpacingType offset;
  offset[0] = offsetx;
  offset[1] = offsety;

  registration->SetInitialOffset(offset);

  switch(metric)
  {
  case 0:
    {
      std::cout<<"Metric: correlation"<<std::endl;
      typedef itk::NormalizedCorrelationImageToImageMetric<ImageType, ImageType> NCCType;
      NCCType::Pointer metricPtr = NCCType::New();
      metricPtr->SubtractMeanOff();
      registration->SetMetric(metricPtr);
      registration->MinimizeOn();
      break;
    }
  case 1:
    {
      std::cout<<"Metric: normalized correlation"<<std::endl;
      typedef itk::NormalizedCorrelationImageToImageMetric<ImageType, ImageType> NCCType;
      NCCType::Pointer metricPtr = NCCType::New();
      metricPtr->SubtractMeanOn();
      registration->SetMetric(metricPtr);
      registration->MinimizeOn();
      break;
      }
  case 2:
    {
      std::cout<<"Metric: mean squares"<<std::endl;
      typedef itk::MeanSquaresImageToImageMetric<ImageType, ImageType> MeanSquareType;
      MeanSquareType::Pointer metricPtr = MeanSquareType::New();
      registration->SetMetric(metricPtr);
      registration->MinimizeOn();
      break;
      }
  case 3:
    {
      std::cout<<"Metric: mean reciprocal square difference"<<std::endl;
      typedef itk::MeanReciprocalSquareDifferenceImageToImageMetric<ImageType, ImageType> MRSDType;
      MRSDType::Pointer metricPtr = MRSDType::New();
      registration->SetMetric(metricPtr);
      registration->MinimizeOff();
      break;
    }
  default:
    {
      std::cerr<<"Metric id should be between 0 and 3"<<std::endl;
      return EXIT_FAILURE;
    }
  }

  //otb::StandardFilterWatcher watcher(registration,"Registration");
  otb::Stopwatch chrono = otb::Stopwatch::StartNew();
  registration->Update();

  std::cout << "Processing time was " << chrono.GetElapsedMilliseconds() << " ms\n";

  CorrelWriterType::Pointer correlWriter = CorrelWriterType::New();
  correlWriter->SetFileName(correlFileName);
  correlWriter->SetInput(registration->GetOutput());
  correlWriter->SetNumberOfDivisionsStrippedStreaming(2);
  correlWriter->Update();

  FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
  fieldWriter->SetFileName(fieldFileName);
  fieldWriter->SetInput(registration->GetOutputDisplacementField());
  fieldWriter->SetNumberOfDivisionsStrippedStreaming(2);
  fieldWriter->Update();

  return EXIT_SUCCESS;
}
