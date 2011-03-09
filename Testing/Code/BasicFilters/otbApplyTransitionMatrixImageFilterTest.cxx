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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbApplyTransitionMatrixImageFilter.h"

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"


int otbApplyTransitionMatrixImageFilterTest(int argc, char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double PixelType;

  typedef otb::VectorImage<PixelType> ImageType;

  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::ApplyTransitionMatrixImageFilter<ImageType, ImageType> FilterType;

  FilterType::MatrixType mat(2, 3, 0.);

  double val = 0;
  for(unsigned int i=0; i<mat.rows(); i++)
    {
      for(unsigned int j=0; j<mat.cols(); j++)
        {
          mat[i][j] = val;
          val += 0.5;
        }
    }


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filter->SetInput(reader->GetOutput());
  filter->SetTransitionMatrix(mat);

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
