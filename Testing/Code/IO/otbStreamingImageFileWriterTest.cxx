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
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"

int otbStreamingImageFileWriterTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  int          iStreaming(::atoi(argv[3]));
  bool         streaming = (bool) (iStreaming);
  int          NumberOfStreamDivisions(::atoi(argv[4]));

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>           ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType> StreamingWriterType;
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
    if (NumberOfStreamDivisions > 1)
      {
      std::cout << "Streaming testing of otb::ImageFileWriter " << std::endl;
      std::cout << "NumberOfStreamDivisions: " << NumberOfStreamDivisions << std::endl;
      writer->SetNumberOfDivisionsStrippedStreaming(NumberOfStreamDivisions);
      }

    writer->SetFileName(outputFilename);
    writer->SetInput(reader->GetOutput());
    writer->Update();
    }

  return EXIT_SUCCESS;
}
