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
#include "otbMPIVrtWriter.h"

#include <cstdlib>
#include <iostream>

// Test
int otbMPIReadWriteTest(int argc, char * argv[]) 
{

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

  otb::mpi::WriteMPI(reader->GetOutput(),outputFilename, false, true);
  
  return EXIT_SUCCESS;
}

