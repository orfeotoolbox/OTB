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
#include "otbVectorImageToMatrixImageFilter.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;


int otbVectorImageToMatrixNewTest(int argc, char * argv[])
{
  VectorImageToMatrixImageFilterType::Pointer vim2matrix = VectorImageToMatrixImageFilterType::New();
  std::cout << vim2matrix << std::endl;
  return EXIT_SUCCESS;
}


int otbVectorImageToMatrixTest(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  VectorImageToMatrixImageFilterType::Pointer vim2matrix = \
      VectorImageToMatrixImageFilterType::New();

  vim2matrix->SetInput(reader->GetOutput());
  vim2matrix->Update();

  std::ofstream file;
  file.open(outfname);
  file.precision(5);
  file << vim2matrix->GetMatrix() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
