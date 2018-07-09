/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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




#include "itkMacro.h"
#include "otbImage.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMultiplyByScalarImageFilter.h"

int otbMultiplyByScalarImageFilterTest(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Define the dimension of the images
  const unsigned int ImageDimension = 2;
  // Declare the types of the images
  typedef otb::Image<float, ImageDimension> InputImageType;
  typedef otb::Image<float, ImageDimension> OutputImageType;

  // Declare Iterator types apropriated for each image
  typedef itk::ImageRegionIteratorWithIndex<
      InputImageType>  InputIteratorType;

  typedef itk::ImageRegionIteratorWithIndex<
      OutputImageType>  OutputIteratorType;

  // Declare the type of the index to access images
  typedef itk::Index<ImageDimension> IndexType;

  // Declare the type of the size
  typedef itk::Size<ImageDimension> SizeType;

  // Declare the type of the Region
  typedef itk::ImageRegion<ImageDimension> RegionType;

  // Create two images
  InputImageType::Pointer inputImage  = InputImageType::New();

  // Define their size, and start index
  SizeType size;
  size[0] = 2;
  size[1] = 2;

  IndexType start;
  start[0] = 0;
  start[1] = 0;

  RegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  // Initialize Image A
  inputImage->SetRegions(region);
  inputImage->Allocate();
  // Create one iterator for the Input Image (this is a light object)
  InputIteratorType it(inputImage, inputImage->GetBufferedRegion());

  // Initialize the content of Image A
  const double pi    = std::atan(1.0) * 4.0;
  const double value = pi / 6.0;
  std::cout << "Content of the Input " << std::endl;
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    it.Set(value);
    std::cout << it.Get() << std::endl;
    ++it;
    }

  // Declare the type for the Acos filter
  typedef otb::MultiplyByScalarImageFilter<InputImageType, OutputImageType> FilterType;

  // Create a MultiplyScalarImage Filter
  FilterType::Pointer filter = FilterType::New();

  // Connect the input images
  filter->SetInput(inputImage);

  // Get the Smart Pointer to the Filter Output
  OutputImageType::Pointer outputImage = filter->GetOutput();

  // Execute the filter
  filter->SetCoef(10.);
  filter->Update();

  // Create an iterator for going through the image output
  OutputIteratorType ot(outputImage, outputImage->GetRequestedRegion());

  //  Check the content of the result image
  std::cout << "Verification of the output " << std::endl;
  const OutputImageType::PixelType epsilon = 1e-6;
  ot.GoToBegin();
  it.GoToBegin();
  while (!ot.IsAtEnd())
    {
    std::cout <<  ot.Get() << " = ";
    std::cout <<  10.0 * (it.Get())  << std::endl;
    const InputImageType::PixelType  input  = it.Get();
    const OutputImageType::PixelType output = ot.Get();
    const OutputImageType::PixelType multiplyByScal  = 10.0 * input;
    if (std::abs(multiplyByScal - output) > epsilon)
      {
      std::cerr << "Error in otbMultiplyScalarImageFilterTest " << std::endl;
      std::cerr << " 10.0 * " << input << ") = " << multiplyByScal << std::endl;
      std::cerr << " differs from " << output;
      std::cerr << " by more than " << epsilon << std::endl;
      return 1;
      }
    ++ot;
    ++it;
    }

  return EXIT_SUCCESS;
}
