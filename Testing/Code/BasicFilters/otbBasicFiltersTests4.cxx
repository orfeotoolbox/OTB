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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbImageListToImageListApplyFilterNew);
  REGISTER_TEST(otbImageListToImageListApplyFilter);
  REGISTER_TEST(otbPerBandVectorImageFilterNew);
  REGISTER_TEST(otbPerBandVectorImageFilterWithMeanFilter);
  REGISTER_TEST(otbPerBandVectorImageFilterWithSobelFilter);
  REGISTER_TEST(otbStreamingStatisticsImageFilterNew);
  REGISTER_TEST(otbStreamingStatisticsImageFilter);
  REGISTER_TEST(otbStreamingStatisticsMapFromLabelImageFilterTest);
  REGISTER_TEST(otbStreamingCompareImageFilterNew);
  REGISTER_TEST(otbStreamingCompareImageFilter);
  REGISTER_TEST(otbVectorImageTo3DScalarImageFilterNew);
  REGISTER_TEST(otbVectorImageTo3DScalarImageFilter);
}
