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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbStreamingTraitsImage);
  REGISTER_TEST(otbStreamingTraitsVectorImage);
  REGISTER_TEST(otbStreamingResampleImageFilterNew);
  REGISTER_TEST(otbStreamingResampleImageFilter);
  REGISTER_TEST(otbStreamingResampleImageFilterWithAffineTransform);
  REGISTER_TEST(otbStreamingResampleImageFilterCompareWithITK);
  REGISTER_TEST(otbResampleSLCImage);
  REGISTER_TEST(otbStreamingMinMaxImageFilterNew);
  REGISTER_TEST(otbStreamingMinMaxImageFilter);
  REGISTER_TEST(otbStreamingMinMaxVectorImageFilterNew);
  REGISTER_TEST(otbStreamingMinMaxVectorImageFilter);
  REGISTER_TEST(otbStreamingStatisticsVectorImageFilterNew);
  REGISTER_TEST(otbStreamingStatisticsVectorImageFilter);
  REGISTER_TEST(otbMatrixTransposeMatrixImageFilterNew);
  REGISTER_TEST(otbMatrixTransposeMatrixImageFilter);
  REGISTER_TEST(otbUnaryImageFunctorWithVectorImageFilterNew);
  REGISTER_TEST(otbUnaryImageFunctorWithVectorImageFilter);
  REGISTER_TEST(otbUnaryFunctorImageFilterNew);
  REGISTER_TEST(otbPrintableImageFilterNew);
  REGISTER_TEST(otbPrintableImageFilter);
  REGISTER_TEST(otbPrintableImageFilterWithMask);
}
