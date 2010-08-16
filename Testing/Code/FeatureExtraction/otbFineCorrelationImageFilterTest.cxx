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
#include "otbFineCorrelationImageFilter.h"
#include "otbStandardFilterWatcher.h"
#include "itkTimeProbe.h"

int otbFineCorrelationImageFilterTest( int argc, char * argv[] )
{
  if(argc!=9)
    {
    std::cerr<<"Usage: "<<argv[0]<<" fixed_fname moving_fname output_correl output_field radius search_radius refineMethod subpixPrecision"<<std::endl;
    return EXIT_FAILURE;
    }
  const char * fixedFileName  = argv[1];
  const char * movingFileName = argv[2];
  const char * correlFileName = argv[3];
  const char * fieldFileName  = argv[4];
  const unsigned int radius   = atoi(argv[5]);
  const unsigned int sradius  = atoi(argv[6]);
  const int          refine   = atoi(argv[7]);
  const unsigned int precision = atoi(argv[8]);

  typedef double      PixelType;
  const unsigned int  Dimension = 2;

  typedef itk::FixedArray<PixelType,Dimension>                                DeformationValueType;
  typedef otb::Image< PixelType,  Dimension >                                 ImageType;
  typedef otb::Image<DeformationValueType,Dimension>                          FieldImageType;
  typedef otb::ImageFileReader< ImageType >                                   ReaderType;
  typedef otb::ImageFileWriter< ImageType >                                   CorrelWriterType;
  typedef otb::ImageFileWriter< FieldImageType>                               FieldWriterType;
  typedef otb::FineCorrelationImageFilter<ImageType,ImageType,FieldImageType> CorrelationFilterType;
  
  ReaderType::Pointer freader = ReaderType::New();
  freader->SetFileName(fixedFileName);
  //freader->Update();
  
  ReaderType::Pointer mreader = ReaderType::New();
  mreader->SetFileName(movingFileName);
  //mreader->Update();

  CorrelationFilterType::Pointer correlation = CorrelationFilterType::New();
  correlation->SetFixedInput(freader->GetOutput());
  correlation->SetMovingInput(mreader->GetOutput());
  correlation->SetRadius(radius);
  correlation->SetSearchRadius(sradius);
  if(refine == 0)
    {
    correlation->SetRefinementModeToCoarse();
    }
  else if(refine == 1)
    {
    correlation->SetRefinementModeToLSQRQuadFit();
    }
  else
    {
    correlation->SetRefinementModeToSubPixel();
    }

  correlation->SetSubPixelPrecision(precision);
  correlation->SetNumberOfThreads(1);
  //  otb::StandardFilterWatcher watcher(correlation,"Correlation");
  itk::TimeProbe chrono;
  chrono.Start();
  correlation->Update();
  chrono.Stop();
  std::cout<<chrono.GetMeanTime()<<"\t";

  CorrelWriterType::Pointer correlWriter = CorrelWriterType::New();
  correlWriter->SetFileName(correlFileName);
  correlWriter->SetInput(correlation->GetOutput());
  correlWriter->Update();

  FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
  fieldWriter->SetFileName(fieldFileName);
  fieldWriter->SetInput(correlation->GetOutputDeformationField());
  fieldWriter->Update();

  return EXIT_SUCCESS;
}
