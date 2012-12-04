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



#include "otbImage.h"
#include "itkMacro.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageFileReaderTest(int argc, char* argv[])
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

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbImageFileReaderWithExtendedFilename(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename1 = argv[2];
  const char * outputFilename2 = argv[3];

  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  std::ofstream file1;
  file1.open(outputFilename1);

  std::ofstream file2;
  file2.open(outputFilename2);

  reader->SetFileName(inputFilename);
  reader->Update();

  file1 << reader->GetOutput()->GetImageKeywordlist();

  file2 << "ProjRef: " << reader->GetOutput()->GetProjectionRef() << std::endl;
  file2 << "Origin: "  << reader->GetOutput()->GetOrigin() << std::endl;
  file2 << "Spacing: " << reader->GetOutput()->GetSpacing() << std::endl;

  return EXIT_SUCCESS;
}
