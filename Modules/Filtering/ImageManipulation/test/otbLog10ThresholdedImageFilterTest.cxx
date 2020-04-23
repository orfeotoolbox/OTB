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


#include <iostream>
#include <fstream>

#include "otbImage.h"
#include "itkImageRegionIterator.h"
#include "otbLog10ThresholdedImageFilter.h"

int otbLog10ThresholdedImageFilterTest(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Invalid arguments -> need only output filename" << std::endl;
    return EXIT_FAILURE;
  }

  // TYPEDEFS
  typedef double PixelType;
  typedef otb::Image<PixelType, 2> ImageType;
  typedef ImageType::RegionType               RegionType;
  typedef ImageType::SizeType                 SizeType;
  typedef ImageType::IndexType                IndexType;
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  typedef otb::Log10ThresholdedImageFilter<ImageType, ImageType> FilterType;

  // INSTANTIATIONS
  ImageType::Pointer  image    = ImageType::New();
  ImageType::Pointer  imageOut = ImageType::New();
  FilterType::Pointer filter   = FilterType::New();

  std::ofstream file;
  file.open(argv[1]);

  RegionType region;
  SizeType   size;
  size.Fill(5);
  IndexType orig;
  orig.Fill(0);

  region.SetSize(size);
  region.SetIndex(orig);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0.);

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  double val = 0.;
  while (!it.IsAtEnd())
  {
    val = 0.000001 * (double)pow((double)10.0, (int)(it.GetIndex()[0] + it.GetIndex()[1]));
    if (it.GetIndex()[0] + it.GetIndex()[1] > 6)
      it.Set(itk::NumericTraits<PixelType>::quiet_NaN());
    else
      it.Set(static_cast<PixelType>(val));

    ++it;
  }

  filter->SetInput(image);
  filter->SetThresholdedValue(0.002); // -> -26.9897 dB
  filter->Update();

  IteratorType itOut(filter->GetOutput(), image->GetLargestPossibleRegion());
  itOut.GoToBegin();
  it.GoToBegin();
  while (!itOut.IsAtEnd() && !it.IsAtEnd())
  {
    std::cout << "Input Value : " << it.Get() << ", "
              << "Output Value : " << itOut.Get() << " dB" << std::endl;
    file << "val[" << it.GetIndex() << "] = " << itOut.Get() << " dB" << std::endl;
    ++itOut;
    ++it;
  }

  file.close();
  return EXIT_SUCCESS;
}
