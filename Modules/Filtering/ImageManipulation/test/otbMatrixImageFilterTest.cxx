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


#include "otbMatrixImageFilter.h"

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include <complex>

int otbMatrixImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef std::complex<double> PixelType;

  typedef otb::VectorImage<PixelType> ImageType;
  typedef otb::MatrixImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}


int otbMatrixImageFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  unsigned   int nbCol =atoi(argv[3]);

  typedef double PixelType;

  typedef otb::VectorImage<PixelType> ImageType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::MatrixImageFilter<ImageType, ImageType> FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);


  FilterType::MatrixType mat(reader->GetOutput()->GetNumberOfComponentsPerPixel(), nbCol, 0.);

  double val = 0;
  for(unsigned int i=0; i<mat.rows(); ++i)
    {
      for(unsigned int j=0; j<mat.cols(); ++j)
        {
          mat[i][j] = val;
          val += 0.5;
        }
    }


  filter->SetInput(reader->GetOutput());
  filter->SetMatrix(mat);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
