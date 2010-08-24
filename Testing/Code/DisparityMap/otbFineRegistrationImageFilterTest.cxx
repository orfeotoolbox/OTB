/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkFixedArray.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbFineRegistrationImageFilter.h"
#include "otbStandardFilterWatcher.h"
#include "itkTimeProbe.h"

#include "itkNormalizedCorrelationImageToImageMetric.h"
#include "itkMeanReciprocalSquareDifferenceImageToImageMetric.h"
#include "itkMeanSquaresImageToImageMetric.h"

int otbFineRegistrationImageFilterTest( int argc, char * argv[] )
{
  if(argc!=9)
    {
    std::cerr<<"Usage: "<<argv[0]<<" fixed_fname moving_fname output_correl output_field radius search_radius subpixPrecision metric(0=CC,1=NCC,2=MeanSquare,3=Mean reciprocal square difference)"<<std::endl;
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

  typedef double      PixelType;
  const unsigned int  Dimension = 2;

  typedef itk::FixedArray<PixelType,Dimension>                                DeformationValueType;
  typedef otb::Image< PixelType,  Dimension >                                 ImageType;
  typedef otb::Image<DeformationValueType,Dimension>                          FieldImageType;
  typedef otb::ImageFileReader< ImageType >                                   ReaderType;
  typedef otb::ImageFileWriter< ImageType >                                   CorrelWriterType;
  typedef otb::ImageFileWriter< FieldImageType>                               FieldWriterType;
  typedef otb::FineRegistrationImageFilter<ImageType,ImageType,FieldImageType> RegistrationFilterType;
  
  ReaderType::Pointer freader = ReaderType::New();
  freader->SetFileName(fixedFileName);
  //freader->Update();
  
  ReaderType::Pointer mreader = ReaderType::New();
  mreader->SetFileName(movingFileName);
  //mreader->Update();

  RegistrationFilterType::Pointer registration = RegistrationFilterType::New();
  registration->SetFixedInput(freader->GetOutput());
  registration->SetMovingInput(mreader->GetOutput());
  registration->SetRadius(radius);
  registration->SetSearchRadius(sradius);
  registration->SetSubPixelAccuracy(precision);

  switch(metric)
  {
  case 0:
    {
      std::cout<<"Metric: correlation"<<std::endl;
      typedef itk::NormalizedCorrelationImageToImageMetric<ImageType,ImageType> NCCType;
      NCCType::Pointer metricPtr = NCCType::New();
      metricPtr->SubtractMeanOff();
      registration->SetMetric(metricPtr);
      registration->MinimizeOn();
      break;
    }
  case 1:
    {
      std::cout<<"Metric: normalized correlation"<<std::endl;
      typedef itk::NormalizedCorrelationImageToImageMetric<ImageType,ImageType> NCCType;
      NCCType::Pointer metricPtr = NCCType::New();
      metricPtr->SubtractMeanOn();
      registration->SetMetric(metricPtr);
      registration->MinimizeOn();
      break;
      }
  case 2:
    {
      std::cout<<"Metric: mean squares"<<std::endl;
      typedef itk::MeanSquaresImageToImageMetric<ImageType,ImageType> MeanSquareType;
      MeanSquareType::Pointer metricPtr = MeanSquareType::New();
      registration->SetMetric(metricPtr);
      registration->MinimizeOn();
      break;
      }
  case 3:
    {
      std::cout<<"Metric: mean reciprocal square difference"<<std::endl;
      typedef itk::MeanReciprocalSquareDifferenceImageToImageMetric<ImageType,ImageType> MRSDType;
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
  itk::TimeProbe chrono;
  chrono.Start();
  registration->Update();
  chrono.Stop();
  std::cout<<chrono.GetMeanTime()<<"\t";

  CorrelWriterType::Pointer correlWriter = CorrelWriterType::New();
  correlWriter->SetFileName(correlFileName);
  correlWriter->SetInput(registration->GetOutput());
  correlWriter->Update();

  FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
  fieldWriter->SetFileName(fieldFileName);
  fieldWriter->SetInput(registration->GetOutputDeformationField());
  fieldWriter->Update();

  return EXIT_SUCCESS;
}
