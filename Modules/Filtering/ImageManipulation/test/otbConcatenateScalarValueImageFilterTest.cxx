/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbConcatenateScalarValueImageFilter.h"

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include <complex>

typedef double PixelType;

typedef otb::VectorImage<PixelType> ImageType;
typedef otb::ConcatenateScalarValueImageFilter<ImageType, ImageType> FilterType;


int otbConcatenateScalarValueImageFilterTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ImageType::Pointer image = ImageType::New();

  const PixelType       ScalarValue   = 42;
  const unsigned int    Size          = 10;
  const unsigned int    NbComponentIn = 4;
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

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(image);
  filter->SetScalarValue(ScalarValue);
  ImageType::Pointer outimage = filter->GetOutput();

  filter->UpdateOutputInformation();
  if (outimage->GetNumberOfComponentsPerPixel() != NbComponentIn + 1)
  {
    std::cerr << "Number of components should be " << NbComponentIn + 1 << std::endl;
    return EXIT_FAILURE;
  }

  filter->Update();


  itk::ImageRegionIteratorWithIndex<ImageType> outIt(outimage, outimage->GetLargestPossibleRegion());
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
  {
    ImageType::IndexType idx   = outIt.GetIndex();
    ImageType::PixelType value = outIt.Get();

    for (unsigned int k = 0; k < NbComponentIn; ++k)
    {
      if (value[k] != idx[0] + idx[1] * Size + k)
      {
        std::cerr << "Error at index " << idx << " channel " << k << " : expected " << idx[0] + idx[1] * Size + k << " got " << value[k] << std::endl;
        return EXIT_FAILURE;
      }
    }
    if (value[NbComponentIn] != ScalarValue)
    {
      std::cerr << "Error at index " << idx << " channel " << NbComponentIn << " : expected " << ScalarValue << " got " << value[NbComponentIn] << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
