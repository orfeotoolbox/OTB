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
#include <cstdlib>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbSimpleParallelTiffWriter.h"
#include <mpi.h>
#include <cstdlib>
#include <iostream>

int otbMPISPTWReadWriteTest(int argc, char* argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile " << std::endl;
    return EXIT_SUCCESS;
    }

  // Initialize MPI environment
  otb::MPIConfig::Pointer config = otb::MPIConfig::Instance();
  config->Init(argc,argv);
  
  // Image typedefs
  typedef float PixelType;
  typedef otb::VectorImage<PixelType>	ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::SimpleParallelTiffWriter<ImageType> WriterType;

  // Read
  ReaderType::Pointer reader = ReaderType::New();
  std::string inputFilename = std::string(argv[1]);
  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();

  // Write
  WriterType::Pointer writer = WriterType::New();
  std::string outputFilename = std::string(argv[2]);
  writer->SetFileName(outputFilename);
  writer->SetInput(reader->GetOutput());
  
  // Execute the pipeline
  try{
    writer->Update();
  }
  catch (std::exception & err) {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err.what() << std::endl;

    config->abort(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;

}
