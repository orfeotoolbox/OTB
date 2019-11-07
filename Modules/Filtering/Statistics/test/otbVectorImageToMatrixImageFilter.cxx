/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbVectorImageToMatrixImageFilter.h"

#include "otbVectorImage.h"
#include "itkImageRegionIteratorWithIndex.h"

const unsigned int Dimension = 2;
typedef double     PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::VectorImageToMatrixImageFilter<ImageType> VectorImageToMatrixImageFilterType;
typedef VectorImageToMatrixImageFilterType::MatrixType MatrixType;

int otbVectorImageToMatrixTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ImageType::Pointer image = ImageType::New();

  const unsigned int    Size        = 10;
  const unsigned int    NbComponent = 4;
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

  VectorImageToMatrixImageFilterType::Pointer vim2matrix = VectorImageToMatrixImageFilterType::New();

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
      if (m(i, j) != x + y * Size + i)
      {
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
