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
typedef double PrecisionType;

typedef otb::VectorImage<PrecisionType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::MatrixMultiplyImageFilter<ImageType,ImageType,double> MatrixMultiplyImageFilterType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;
typedef otb::ImageFileWriter<ImageType> WriterType;
typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;

int otbMatrixMultiplyImageFilterNewTest(int argc, char * argv[])
{
  MatrixMultiplyImageFilterType::Pointer filter = MatrixMultiplyImageFilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbMatrixMultiplyImageFilterTest(int argc, char * argv[])
{
  ImageType::Pointer image = ImageType::New();

  const unsigned int Size = 10;
  const unsigned int NbComponentIn = 4;
  const unsigned int NbComponentOut = 2;
  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, Size);
  region.SetSize(1, Size);
  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(NbComponentIn);
  image->Allocate();


  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetLargestPossibleRegion());
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    ImageType::IndexType idx = it.GetIndex();
    ImageType::PixelType value;
    value.SetSize(NbComponentIn);

    for (unsigned int k = 0; k < NbComponentIn; ++k)
      {
      value[k] = idx[0] + idx[1] * Size + k;
      }

    it.Set(value);
    }

  MatrixType matrix(NbComponentOut,NbComponentIn);
  for (unsigned int i = 0; i < matrix.rows(); ++i)
    {
    for (unsigned int j = 0; j < matrix.cols(); ++j)
      {
      matrix(i, j) = (i + j) * j;
      }
    }

  MatrixMultiplyImageFilterType::Pointer mul = MatrixMultiplyImageFilterType::New();
  mul->SetInput(image);
  mul->SetMatrix(matrix);

  mul->Update();

  ImageType::Pointer outImage = mul->GetOutput();

  itk::ImageRegionIteratorWithIndex<ImageType> outIt(outImage, outImage->GetLargestPossibleRegion());
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    ImageType::IndexType idx = outIt.GetIndex();
    ImageType::PixelType value = outIt.Get();

    vnl_vector<PrecisionType> vectorValue(value.GetDataPointer(), value.GetSize());

    vnl_vector<PrecisionType> input(NbComponentIn);
    for (unsigned int k = 0; k < NbComponentIn; ++k)
       {
       input[k] = idx[0] + idx[1] * Size + k;
       }
    vnl_vector<PrecisionType> expected = matrix * input;
    vnl_vector<PrecisionType> diff = expected - vectorValue;

    if ( diff.magnitude() > 1E-10 )
      {
      std::cerr << "At index " << idx << ". Expected " << expected << " , got " << vectorValue << std::endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}


int otbMatrixMultiplyImageFilterRealImageTest(int argc, char * argv[])
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
