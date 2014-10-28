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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLineRatioDetectorImageFilter.h"


int otbLineRatioDetectorLinear(int itkNotUsed(argc), char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename1 = argv[2];
  const char * outputFilename2 = argv[3];

  // Width of the linear feature = 2*WidthLine+1
  unsigned int WidthLine((unsigned int) ::atoi(argv[4]));
  // Length of the linear feature = 2*LengthLine+1
  unsigned int LengthLine((unsigned int) ::atoi(argv[5]));

  typedef unsigned char InputPixelType;
  typedef double        OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageDirectionType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::LinearInterpolateImageFunction<InputImageType,
      double>         InterpolatorType;
  typedef otb::LineRatioDetectorImageFilter<InputImageType, OutputImageType,  OutputImageDirectionType,
      InterpolatorType> FilterType;

  FilterType::Pointer FilterLineRatio = FilterType::New();

  FilterLineRatio->SetWidthLine(WidthLine);
  FilterLineRatio->SetLengthLine(LengthLine);

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();

  reader->SetFileName(inputFilename);
  writer1->SetFileName(outputFilename1);
  writer2->SetFileName(outputFilename2);

  FilterLineRatio->SetInput(reader->GetOutput());
  writer1->SetInput(FilterLineRatio->GetOutput());
  writer2->SetInput(FilterLineRatio->GetOutputDirection());

  writer1->Update();
  writer2->Update();

  return EXIT_SUCCESS;
}
