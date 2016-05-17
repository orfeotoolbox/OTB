/*=========================================================================

   Program:   ORFEO Toolbox
   Language:  C++
   Date:      $Date$
   Version:   $Revision$


   Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
   See OTBCopyright.txt for details.

   Some parts of this code are derived from ITK. See ITKCopyright.txt
   for details.


      This software is distributed WITHOUT ANY WARRANTY; without even
      the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
      PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Includes
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMPIConfig.h"
#include <cstdlib>
#include <iostream>
#include <boost/chrono/thread_clock.hpp>

// Test
int otbMPIReadWriteTest(int argc, char * argv[]) 
{
  // Mono-thread execution
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);

  // Start chrono
  boost::chrono::thread_clock::time_point startTimer = boost::chrono::thread_clock::now();
  
  // MPI Initialization
  typedef otb::MPIConfig    MPIConfigType;
  MPIConfigType::Pointer config = MPIConfigType::Instance();
  config->Init(argc,argv,true);

  // Verify the number of parameters in the command line
  if (argc != 3)
  {
    std::stringstream message;
    message << "Usage: " << std::endl;
    message << argv[0] << " inputImageFile outputImageFile " << std::endl;
    config->logError(message.str());
    config->abort(EXIT_FAILURE);
  } 

  // Image types
  typedef float PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType,  Dimension> ImageType;

  // Reader
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  // Reader configuration
  std::string inputFilename = std::string(argv[1]);
  reader->SetFileName(inputFilename);

  // Update MPI Pipeline
  std::string outputFilename = std::string(argv[2]);
  //config->UpdateMPI(reader->GetOutput(),outputFilename, false, true);

  // End chrono
  boost::chrono::thread_clock::time_point stopTimer = boost::chrono::thread_clock::now();
  std::stringstream message;
  message << "Duration = " << boost::chrono::duration_cast<boost::chrono::milliseconds>(stopTimer-startTimer).count() <<" ms\n";
  config->logInfo(message.str());
  
  return EXIT_SUCCESS;
}

