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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbMatrixMultiplyImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::MatrixMultiplyImageFilter<ImageType,ImageType,double> MatrixMultiplyImageFilterType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;
typedef otb::ImageFileWriter<ImageType> WriterType;

int otbMatrixMultiplyImageFilterNewTest(int argc, char * argv[])
{
  MatrixMultiplyImageFilterType::Pointer filter = MatrixMultiplyImageFilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbMatrixMultiplyImageFilterTest(int argc, char * argv[])
{
  const char * inputImage = argv[1];
  const char * inputEndmembers = argv[2];
  const char * outputImage = argv[3];

  ReaderType::Pointer readerImage = ReaderType::New();
  readerImage->SetFileName(inputImage);

  ReaderType::Pointer readerEndMembers = ReaderType::New();
  readerEndMembers->SetFileName(inputEndmembers);
  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(readerEndMembers->GetOutput());

  std::cout << "computing endMember2Matrix" << std::endl;
  endMember2Matrix->Update();
  std::cout << "computing endMember2Matrix done" << std::endl;

  typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;
  MatrixType endMembers = endMember2Matrix->GetMatrix();
  std::cout << "endMembers : "  << endMembers.rows() << " " << endMembers.cols() << std::endl;
  MatrixType pinv = vnl_matrix_inverse<double>(endMembers);
  std::cout << "pinv : "  << pinv.rows() << " " << pinv.cols() << std::endl;

  MatrixMultiplyImageFilterType::Pointer mul = MatrixMultiplyImageFilterType::New();
  mul->SetInput(readerImage->GetOutput());
  mul->SetMatrix(pinv);

  std::cout << "computing MatrixMultiplyImageFilterType" << std::endl;
  mul->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImage);
  writer->SetInput(mul->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
