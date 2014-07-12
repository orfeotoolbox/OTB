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
#include "otbISRAUnmixingImageFilter.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbStandardWriterWatcher.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::ISRAUnmixingImageFilter<ImageType, ImageType, PixelType> UnmixingImageFilterType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;
typedef otb::ImageFileWriter<ImageType> WriterType;

int otbISRAUnmixingImageFilterNewTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  UnmixingImageFilterType::Pointer filter = UnmixingImageFilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbISRAUnmixingImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputImage = argv[1];
  const char * inputEndmembers = argv[2];
  const char * outputImage = argv[3];
  int maxIter = atoi(argv[4]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);

  ReaderType::Pointer readerEndMembers = ReaderType::New();
  readerEndMembers->SetFileName(inputEndmembers);
  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(readerEndMembers->GetOutput());

  endMember2Matrix->Update();

  typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;
  MatrixType endMembers = endMember2Matrix->GetMatrix();
  MatrixType pinv = vnl_matrix_inverse<PixelType>(endMembers);

  UnmixingImageFilterType::Pointer unmixer = UnmixingImageFilterType::New();

  unmixer->SetInput(readerImage->GetOutput());
  unmixer->SetMaxIteration(maxIter);
  //unmixer->SetNumberOfThreads(1);
  unmixer->SetEndmembersMatrix(endMember2Matrix->GetMatrix());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(unmixer->GetOutput());
  writer->SetNumberOfDivisionsStrippedStreaming(10);

  otb::StandardWriterWatcher w4(writer, unmixer,"ISRAUnmixingImageFilter");

  writer->Update();

  return EXIT_SUCCESS;
}
