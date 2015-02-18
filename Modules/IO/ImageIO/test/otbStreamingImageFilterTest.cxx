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
#include "itkStreamingImageFilter.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbStreamingImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::StreamingImageFilter<InputImageType, OutputImageType> StreamingImageFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  StreamingImageFilterType::Pointer streamer = StreamingImageFilterType::New();

  streamer->SetInput(reader->GetOutput());
  streamer->SetNumberOfStreamDivisions(10);
  streamer->Update();

  writer->SetInput(streamer->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
