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
#include "otbVectorImageToMatrixImageFilter.h"

#include "otbVectorImage.h"
#include "itkImageRegionIteratorWithIndex.h"

const unsigned int Dimension = 2;
typedef double PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;
typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;

int otbVectorImageToMatrixNewTest(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  VectorImageToMatrixImageFilterType::Pointer vim2matrix = VectorImageToMatrixImageFilterType::New();
  std::cout << vim2matrix << std::endl;
  return EXIT_SUCCESS;
}

int otbVectorImageToMatrixTest(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  ImageType::Pointer image = ImageType::New();

  const unsigned int Size = 10;
  const unsigned int NbComponent = 4;
  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, Size);
  region.SetSize(1, Size);
  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(NbComponent);
  image->Allocate();

  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetLargestPossibleRegion());
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    ImageType::IndexType idx = it.GetIndex();
    ImageType::PixelType value;
    value.SetSize(NbComponent);

    for (unsigned int k = 0; k < NbComponent; ++k)
      {
      value[k] = idx[0] + idx[1] * Size + k;
      }

    it.Set(value);
    }

  VectorImageToMatrixImageFilterType::Pointer vim2matrix = \
      VectorImageToMatrixImageFilterType::New();

  vim2matrix->SetInput(image);
  vim2matrix->Update();

  MatrixType m = vim2matrix->GetMatrix();

  std::cout << m << std::endl;

  for (unsigned int j = 0; j < m.cols(); ++j)
    {
    unsigned int x = j % Size;
    unsigned int y = j / Size;

    for (unsigned int i = 0; i < m.rows(); ++i)
      {
      if ( m(i, j) !=  x + y * Size + i )
        {
        return EXIT_FAILURE;
        }
      }
    }

  return EXIT_SUCCESS;
}
