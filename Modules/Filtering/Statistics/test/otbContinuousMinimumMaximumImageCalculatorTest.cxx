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


#include "otbImage.h"
#include "otbContinuousMinimumMaximumImageCalculator.h"
#include "itkSize.h"

int
otbContinuousMinimumMaximumImageCalculatorTest(int, char *[])
{

  typedef short int                PixelType;
  typedef itk::Size<2>             SizeType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef otb::ContinuousMinimumMaximumImageCalculator<ImageType> MinMaxCalculatorType;

  /* Define the image size and physical coordinates */
  SizeType size = {{20, 20}};
  double   origin[2] = { 0.0, 0.0};
  double   spacing[2] = { 1, 1};

  int flag = 0;           /* Did this test program work? */

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
  image->SetSpacing(spacing);

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
