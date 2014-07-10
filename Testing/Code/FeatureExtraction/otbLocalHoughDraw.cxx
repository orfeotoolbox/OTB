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
#include <list>

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLineSpatialObjectList.h"

#include "otbLocalHoughFilter.h"
#include "otbDrawLineSpatialObjectListFilter.h"

#include "otbImageToLineSpatialObjectListFilter.h"

#include "itkRescaleIntensityImageFilter.h"

int otbLocalHoughDraw(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  unsigned int RadiusX((unsigned int) ::atoi(argv[3]));
  unsigned int RadiusY((unsigned int) ::atoi(argv[4]));
  unsigned int NumberOfLines((unsigned int) ::atoi(argv[5]));

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::LocalHoughFilter<InputImageType> FilterType;
  //typedef otb::ImageToLineSpatialObjectListFilter< InputImageType >   FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::RescaleIntensityImageFilter<InputImageType> RescalerType;

  ReaderType::Pointer   reader = ReaderType::New();
  WriterType::Pointer   writer = WriterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  reader->SetFileName(inputFilename);
  reader->Update();

  writer->SetFileName(outputFilename);

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  filter->SetRadius(Radius);
  filter->SetNumberOfLines(NumberOfLines);

  rescaler->SetInput(reader->GetOutput());
  filter->SetInput(rescaler->GetOutput());
  filter->Update();

  typedef otb::DrawLineSpatialObjectListFilter<InputImageType, OutputImageType> DrawFilterType;

  DrawFilterType::Pointer drawfilter = DrawFilterType::New();

  drawfilter->SetInputLineSpatialObjectList(filter->GetOutput());

  drawfilter->SetInput(reader->GetOutput());
  writer->SetInput(drawfilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
