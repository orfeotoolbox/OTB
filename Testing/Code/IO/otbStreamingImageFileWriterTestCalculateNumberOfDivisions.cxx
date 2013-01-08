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

int otbImageFileWriterTestCalculateNumberOfDivisions(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  std::string  MethodCalculateNumberOfStreamDivision(argv[3]);

  typedef unsigned int PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType,  Dimension>        ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType> StreamingWriterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  std::cout << "Streaming writing test" << std::endl;
  StreamingWriterType::Pointer writer = StreamingWriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());

  if (MethodCalculateNumberOfStreamDivision == "SetNumberOfDivisionsStrippedStreaming")
    {
    writer->SetNumberOfDivisionsStrippedStreaming(::atoi(argv[4]));
    }
  else if (MethodCalculateNumberOfStreamDivision == "SetNumberOfDivisionsTiledStreaming")
    {
    writer->SetNumberOfDivisionsTiledStreaming(::atoi(argv[4]));
    }
  else if (MethodCalculateNumberOfStreamDivision == "SetNumberOfLinesStrippedStreaming")
    {
    writer->SetNumberOfLinesStrippedStreaming(::atoi(argv[4]));
    }
  else if (MethodCalculateNumberOfStreamDivision == "SetAutomaticStrippedStreaming")
    {
    writer->SetAutomaticStrippedStreaming(::atoi(argv[4]));
    }
  else if (MethodCalculateNumberOfStreamDivision == "SetTileDimensionTiledStreaming")
    {
    writer->SetTileDimensionTiledStreaming(::atoi(argv[4]));
    }
  else if (MethodCalculateNumberOfStreamDivision == "SetAutomaticTiledStreaming")
    {
    writer->SetAutomaticTiledStreaming(::atoi(argv[4]));
    }
  else if (MethodCalculateNumberOfStreamDivision == "DEFAULT")
    {

    }
  else
    {
    itkGenericExceptionMacro(<< "Parameter value not authorized !!!");
    }
  writer->Update();

  return EXIT_SUCCESS;
}
