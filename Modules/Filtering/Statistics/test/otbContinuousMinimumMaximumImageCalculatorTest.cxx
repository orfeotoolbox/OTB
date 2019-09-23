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


#include "otbImage.h"
#include "otbContinuousMinimumMaximumImageCalculator.h"
#include "itkSize.h"

int otbContinuousMinimumMaximumImageCalculatorTest(int, char* [])
{

  typedef short int    PixelType;
  typedef itk::Size<2> SizeType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef otb::ContinuousMinimumMaximumImageCalculator<ImageType> MinMaxCalculatorType;

  /* Define the image size and physical coordinates */
  SizeType size       = {{20, 20}};
  double   origin[2]  = {0.0, 0.0};
  double   spacing[2] = {1, 1};

  int flag = 0; /* Did this test program work? */

  std::cout << "Testing Minimum and Maximum Image Calculator:\n";

  // Allocate a simple test image
  ImageType::Pointer    image = ImageType::New();
  ImageType::RegionType region;
  region.SetSize(size);
  image->SetLargestPossibleRegion(region);
  image->SetRequestedRegion(region);
  image->SetBufferedRegion(region);
  image->Allocate();

  // Set origin and spacing of physical coordinates
  image->SetOrigin(origin);
  image->SetSignedSpacing(spacing);

  PixelType minimum = -52;
  PixelType maximum = 103;

  // Initialize the image contents with the minimum value
  itk::Index<2> index;
  for (int row = 0; row < 20; row++)
  {
    index[1] = row;
    for (int col = 0; col < 20; col++)
    {
      index[0] = col;
      image->SetPixel(index, minimum);
    }
  }

  // Set voxel (10, 10) to maximum value
  index[0] = 10;
  index[1] = 10;
  image->SetPixel(index, maximum);

  index[0] = 10;
  index[1] = 11;
  image->SetPixel(index, 41);

  // Create and initialize the calculator
  MinMaxCalculatorType::Pointer calculator = MinMaxCalculatorType::New();
  calculator->SetImage(image);
  calculator->Compute();

  // Return minimum of intensity
  PixelType minimumResult = calculator->GetMinimum();
  std::cout << "The Minimum intensity value is : " << minimumResult << std::endl;
  std::cout << "Its index position is : " << calculator->GetContinuousIndexOfMinimum() << std::endl;

  if (minimumResult != minimum)
  {
    std::cout << "Minimum Value is wrong : " << minimumResult;
    std::cout << " != " << minimum << std::endl;
    flag = 1;
  }

  // Return maximum of intensity
  PixelType maximumResult = calculator->GetMaximum();
  std::cout << "The Maximum intensity value is : " << maximumResult << std::endl;
  std::cout << "Its index position is : " << calculator->GetContinuousIndexOfMaximum() << std::endl;

  if (maximumResult != maximum)
  {
    std::cout << "Maximum Value is wrong : " << maximumResult;
    std::cout << " != " << maximum << std::endl;
    flag = 2;
  }

  // Return results of test
  if (flag != 0)
  {
    std::cout << "*** Some tests failed" << std::endl;
    return flag;
  }
  else
  {
    std::cout << "All tests successfully passed" << std::endl;
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
