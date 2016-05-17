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
#include <boost/chrono/thread_clock.hpp>

int otbMPISPTWReadWriteTest(int argc, char* argv[])
{
  // Mono-thread execution
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
 
  if (argc != 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile " << std::endl;
    return EXIT_SUCCESS;
    }

  // Initialize MPI environment
  int myrank, nprocs;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

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
  writer->SetMyRank(myrank);
  writer->SetNProcs(nprocs);

  // Execute the pipeline
  try{
    writer->Update();
  }
  catch (std::exception & err) {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err.what() << std::endl;
    MPI_Abort(MPI_COMM_WORLD, MPI_ERR_UNKNOWN);
    return EXIT_FAILURE;
  }

  // Terminate MPI session
  MPI_Finalize();

  return EXIT_SUCCESS;

}
