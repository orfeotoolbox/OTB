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
  REGISTER_TEST(otbExtractROIResample);
  REGISTER_TEST(otbPointSetFunctionNew);
  REGISTER_TEST(otbPointSetDensityFunctionNew);
  REGISTER_TEST(otbPointSetDensityFunctionTest);
  REGISTER_TEST(otbPointSetToDensityImageFilterNew);
  REGISTER_TEST(otbPointSetToDensityImageFilterTest);
  REGISTER_TEST(otbKeyPointDensityImageFilterNew);
  REGISTER_TEST(otbKeyPointDensityImageFilterTest);
  REGISTER_TEST(otbImagePCAShapeModelEstimatorTest);
  REGISTER_TEST(otbFunctionWithNeighborhoodToImageFilterNew);
  REGISTER_TEST(otbStreamingInnerProductVectorImageFilterNew);
  REGISTER_TEST(otbStreamingInnerProductVectorImageFilter);
  REGISTER_TEST(otbEstimateInnerProductPCAImageFilterNew);
  REGISTER_TEST(otbEstimateInnerProductPCAImageFilter);
  REGISTER_TEST(otbNormalizeInnerProductPCAImageFilterNew);
  REGISTER_TEST(otbNormalizeInnerProductPCAImageFilter);
  REGISTER_TEST(otbInnerProductPCAImageFilterNew);
  REGISTER_TEST(otbInnerProductPCAImageFilter);
  REGISTER_TEST(otbListSampleToVariableDimensionHistogramGeneratorNew);
  REGISTER_TEST(otbListSampleToVariableDimensionHistogramGenerator);
}
