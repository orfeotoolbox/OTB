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



#include "otbVectorImage.h"
#include "itkMacro.h"
#include <iostream>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbGDALImageIO.h"

template<class PixelType>
int otbGenericGDALImageIOTest(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  std::vector<std::string> creationOptions;
  for (int curArg = 3; curArg < argc; curArg++)
    {
    creationOptions.push_back(argv[curArg]);
    std::cout << "Creation option : " << argv[curArg] << std::endl;
    }

  typedef PixelType InputPixelType;
  typedef PixelType OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();


  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  // Be sure to use GDALImageIO
  otb::GDALImageIO::Pointer io = otb::GDALImageIO::New();
  io->SetOptions(creationOptions);
  writer->SetImageIO(io.GetPointer());

  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbGDALImageIOTest_uint8(int argc, char* argv[])
{
  return otbGenericGDALImageIOTest<unsigned char>(argc, argv);
}

int otbGDALImageIOTest_uint16(int argc, char* argv[])
{
  return otbGenericGDALImageIOTest<unsigned short>(argc, argv);
}
