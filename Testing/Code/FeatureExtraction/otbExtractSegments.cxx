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



#include "itkMacro.h"
#include <iostream>

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractSegmentsImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int otbExtractSegments(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * outputFilename = argv[3];

  unsigned int PixelSuppressionRadiusX((unsigned int) ::atoi(argv[4]));
  float        PixelSuppressionAngularBeam((float) ::atof(argv[5]));

  unsigned int LocalHoughRadiusX((unsigned int) ::atoi(argv[6]));
  unsigned int LocalHoughRadiusY((unsigned int) ::atoi(argv[7]));
  unsigned int LocalHoughNumberOfLines((unsigned int) ::atoi(argv[8]));

  float FillGapsRadius((float) ::atof(argv[9]));
  float FillGapsAngularBeam((float) ::atof(argv[10]));

  typedef double InputPixelType;
  typedef double OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>                               ReaderType1;
  typedef otb::ImageFileReader<InputImageType>                               ReaderType2;
  typedef otb::ImageFileWriter<OutputImageType>                              WriterType;
  typedef itk::RescaleIntensityImageFilter<OutputImageType, OutputImageType> RescalerType;

  typedef otb::ExtractSegmentsImageFilter<InputImageType, OutputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  // filter parameters

  FilterType::SizeType PixelSuppressionRadius;
  PixelSuppressionRadius[0] = PixelSuppressionRadiusX;
  PixelSuppressionRadius[1] = PixelSuppressionRadiusX;

  filter->SetPixelSuppressionRadius(PixelSuppressionRadius);
  filter->SetPixelSuppressionAngularBeam(PixelSuppressionAngularBeam);

  FilterType::SizeType LocalHoughRadius;
  LocalHoughRadius[0] = LocalHoughRadiusX;
  LocalHoughRadius[1] = LocalHoughRadiusY;

  filter->SetLocalHoughRadius(LocalHoughRadius);
  filter->SetLocalHoughNumberOfLines(LocalHoughNumberOfLines);

  filter->SetFillGapsRadius(FillGapsRadius);
  filter->SetFillGapsAngularBeam(FillGapsAngularBeam);

  // Reader / Writer

  ReaderType1::Pointer  reader1 = ReaderType1::New();
  ReaderType2::Pointer  reader2 = ReaderType2::New();
  WriterType::Pointer   writer = WriterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);

  filter->SetInputImage(reader1->GetOutput());
  filter->SetInputImageDirection(reader2->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
