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
#include "otbSimpleParallelTiffWriter.h"
#include <mpi.h>



/*
 * Execute the MPIised pipeline
 * Reader ---> Resample filter ---> MPIWriter
 */
void exec(char * inputImage, char * outputImage, int myrank, int nprocs)
{

  // Image typedefs
  typedef float PixelType;
  typedef otb::VectorImage<PixelType>	ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::SimpleParallelTiffWriter<ImageType> WriterType;

  // Read
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImage);
  reader->GenerateOutputInformation();

  // Write
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(reader->GetOutput());
  writer->SetMyRank(myrank);
  writer->SetNProcs(nprocs);
  writer->Update();

}

int otbSPTWStripsWriteTest(int argc, char* argv[])
{

  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile " << std::endl;
    return -1;
    }

  // Initialize MPI environment
  int myrank, nprocs;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // Execute the pipeline
  try{
    exec(argv[1], argv[2], myrank, nprocs);
  }
  catch (std::exception & err) {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err.what() << std::endl;
    MPI_Abort(MPI_COMM_WORLD, MPI_ERR_UNKNOWN);
    return -1;
  }

  // Terminate MPI session
  MPI_Finalize();

  return 0; //EXIT_SUCCESS;

}
