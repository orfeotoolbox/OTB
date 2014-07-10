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
#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbMDMDNMFImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbStandardWriterWatcher.h"

int otbMDMDNMFImageFilterNewTest(int argc, char * argv[])
{
  typedef double                                                    PixelType;
  typedef otb::VectorImage<PixelType, 2>                            VectorImageType;
  typedef otb::MDMDNMFImageFilter<VectorImageType, VectorImageType> MDMDNMFImageFilterType;

  MDMDNMFImageFilterType::Pointer mdmd = MDMDNMFImageFilterType::New();
  return EXIT_SUCCESS;
}

int otbMDMDNMFImageFilterTest(int argc, char * argv[])
{
  typedef double                                         PixelType;
  typedef otb::VectorImage<PixelType, 2>                 ImageType;
  typedef otb::MDMDNMFImageFilter<ImageType, ImageType>  MDMDNMFImageFilterType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;

  const char * inputImage = argv[1];
  const char * outputImage = argv[2];
  const unsigned int maxIter = atoi(argv[3]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);
  readerImage->UpdateOutputInformation();

  MDMDNMFImageFilterType::Pointer unmixer = MDMDNMFImageFilterType::New();

  unmixer->SetInput(readerImage->GetOutput());

  MDMDNMFImageFilterType::MatrixType A;
  A.set_size(readerImage->GetOutput()->GetNumberOfComponentsPerPixel(), 5);
  A.fill(100.);
  A.set_column(1, 200.);
  A.set_column(2, 300.);
  A.set_column(3, 400.);
  A.set_column(4, 500.);
  unmixer->SetEndmembersMatrix(A);
  unmixer->SetMaxIter(maxIter);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(unmixer->GetOutput());

  otb::StandardWriterWatcher w4(writer, unmixer,"MDMDNMFImageFilter");
  writer->Update();

  return EXIT_SUCCESS;
}

int otbMDMDNMFImageFilterTest2(int argc, char * argv[])
{
  typedef double                                         PixelType;
  typedef otb::VectorImage<PixelType, 2>                 ImageType;
  typedef otb::MDMDNMFImageFilter<ImageType, ImageType>  MDMDNMFImageFilterType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;

  const char * inputImage = argv[1];
  const char * inputEndmembers = argv[2];
  const char * outputImage = argv[3];
  const unsigned int maxIter = atoi(argv[4]);

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);
  readerImage->UpdateOutputInformation();

  ReaderType::Pointer readerEndMembers = ReaderType::New();
  readerEndMembers->SetFileName(inputEndmembers);
  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(readerEndMembers->GetOutput());

  endMember2Matrix->Update();

  typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;
  MatrixType endMembers = endMember2Matrix->GetMatrix();

  MDMDNMFImageFilterType::Pointer unmixer = MDMDNMFImageFilterType::New();

  unmixer->SetInput(readerImage->GetOutput());

  unmixer->SetEndmembersMatrix(endMember2Matrix->GetMatrix());
  unmixer->SetMaxIter(maxIter);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(unmixer->GetOutput());

  otb::StandardWriterWatcher w4(writer, unmixer,"MDMDNMFImageFilter");
  writer->Update();

  return EXIT_SUCCESS;
}
