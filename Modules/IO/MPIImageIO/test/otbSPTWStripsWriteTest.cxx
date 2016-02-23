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

#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSimpleParallelTiffWriter.h"

#include <mpi.h>

#include "otbBCOInterpolateImageFunction.h"
#include "otbStreamingResampleImageFilter.h"

/*
 * Execute the MPIised pipeline
 * Reader ---> Resample filter ---> MPIWriter
 */
void exec(char * inputImage, char * outputImage, int myrank, int nprocs)
{

  const float upsample = 2.0;

  /** Image typedefs */
  typedef float PixelType;
  typedef otb::VectorImage<PixelType>	ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::BCOInterpolateImageFunction<ImageType> InterpolatorType;
  typedef otb::StreamingResampleImageFilter<ImageType, ImageType, double> FilterType;
  typedef otb::SimpleParallelTiffWriter<ImageType> WriterType;

  /* Read */
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImage);
  reader->GenerateOutputInformation();

  /* Filter */
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetOutputOrigin(reader->GetOutput()->GetOrigin());
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  ImageType::SizeType size = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  spacing[0] /= upsample;
  spacing[1] /= upsample;
  size[0] *= upsample;
  size[1] *= upsample;
  filter->SetOutputSpacing(spacing);
  filter->SetOutputSize(size);
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  interpolator->SetRadius(2);
  filter->SetInterpolator(interpolator);

  /* Write */
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(filter->GetOutput());
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

  return EXIT_SUCCESS;

}
