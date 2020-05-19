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


#include "otbImage.h"
#include "otbChangeNoDataValueFilter.h"
#include "itkImageRegionIterator.h"

int otbChangeNoDataValueFilter(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  // Build an image
  typedef otb::Image<double> ImageType;
  ImageType::Pointer         img = ImageType::New();

  ImageType::SizeType size;
  size.Fill(20);

  ImageType::RegionType region;
  region.SetSize(size);

  // Fill it with a default value
  img->SetRegions(region);
  img->Allocate();
  img->FillBuffer(10);

  // Write no-data flags to it
  std::vector<bool>   flags(1, true);
  std::vector<double> values(1, -10.);
  otb::WriteNoDataFlags(flags, values, img->GetMetaDataDictionary());

  // Fill half of the pixels with no-data values
  itk::ImageRegionIterator<ImageType> it(img, region);
  unsigned int                        count = 0;
  for (it.GoToBegin(); !it.IsAtEnd(); ++it, ++count)
  {
    if (count % 2 == 0)
      it.Set(-10.);
  }

  // Instantiate filter
  typedef otb::ChangeNoDataValueFilter<ImageType, ImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();

  std::vector<double> newValues(1, -20.);

  filter->SetInput(img);
  filter->SetNewNoDataValues(newValues);
  filter->Update();

  // Check output
  it    = itk::ImageRegionIterator<ImageType>(filter->GetOutput(), region);
  count = 0;

  bool failed = false;

  for (it.GoToBegin(); !it.IsAtEnd(); ++it, ++count)
  {
    if (count % 2 == 0 && it.Get() != -20.)
    {
      std::cerr << "Pixel should have new no-data value" << std::endl;
      failed = true;
    }
    else if (count % 2 == 1 && it.Get() != 10.)
    {
      std::cerr << "Pixel value should be unchanged" << std::endl;
      failed = true;
    }
  }

  otb::ReadNoDataFlags(filter->GetOutput()->GetMetaDataDictionary(), flags, values);

  if (flags.empty() || !flags[0])
  {
    std::cerr << "Output no-data flag should be [1] " << std::endl;
    failed = true;
  }

  if (values.empty() || values[0] != -20)
  {
    std::cerr << "Output no-data value should be [-20.]" << std::endl;
    failed = true;
  }

  if (failed)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
