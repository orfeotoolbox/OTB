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

#include "otbVectorImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

int otbStreamingShortImageFileWriterTest(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  int          iStreaming(::atoi(argv[3]));
  bool         streaming = (bool) (iStreaming);
  int          NumberOfStreamDivisions(10);
  if (streaming == true)
    {
    NumberOfStreamDivisions = ::atoi(argv[4]);
    }

  typedef unsigned short InputPixelType;
  typedef unsigned short OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>           ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> StreamingWriterType;
  typedef otb::ImageFileWriter<OutputImageType>          WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  if (streaming == true)
    {
    std::cout << "Streaming writing test" << std::endl;
    StreamingWriterType::Pointer writer = StreamingWriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetNumberOfDivisionsStrippedStreaming(NumberOfStreamDivisions);
    writer->SetInput(reader->GetOutput());
    writer->Update();
    }
  else
    {
    std::cout << "Writing test" << std::endl;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFilename);
    writer->SetInput(reader->GetOutput());
    writer->Update();
    }

  return EXIT_SUCCESS;
}
