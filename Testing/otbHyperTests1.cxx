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
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbEigenvalueLikelihoodMaximizationNewTest);
  REGISTER_TEST(otbEigenvalueLikelihoodMaximizationTest);
  REGISTER_TEST(otbStreamingStatisticsVectorImageFilterTest);
  REGISTER_TEST(otbStreamingStatisticsVectorImageFilter2NewTest);
  REGISTER_TEST(otbStreamingStatisticsVectorImageFilter2Test);
  REGISTER_TEST(otbMatrixMultiplyImageFilterNewTest);
  REGISTER_TEST(otbMatrixMultiplyImageFilterTest);
  REGISTER_TEST(otbUnConstrainedLeastSquareImageFilterNewTest);
  REGISTER_TEST(otbUnConstrainedLeastSquareImageFilterTest);
  REGISTER_TEST(otbISRAUnmixingImageFilterNewTest);
  REGISTER_TEST(otbISRAUnmixingImageFilterTest);
  REGISTER_TEST(otbNCLSUnmixingImageFilterNewTest);
  REGISTER_TEST(otbNCLSUnmixingImageFilterTest);
  REGISTER_TEST(otbFCLSUnmixingImageFilterNewTest);
  REGISTER_TEST(otbFCLSUnmixingImageFilterTest);
  REGISTER_TEST(otbCLSPSTOUnmixingImageFilterNewTest);
  REGISTER_TEST(otbCLSPSTOUnmixingImageFilterTest);
  REGISTER_TEST(otbFullyConstrainedLeastSquareImageFilterNewTest);
  REGISTER_TEST(otbFullyConstrainedLeastSquareImageFilterTest);
  REGISTER_TEST(otbVectorImageToMatrixNewTest);
  REGISTER_TEST(otbVectorImageToMatrixTest);
  REGISTER_TEST(vahineVCA);
  REGISTER_TEST(vahineElmVCA);
  REGISTER_TEST(otbProjectiveProjectionTestHighSNR);
  REGISTER_TEST(otbVCATestHighSNR);
  REGISTER_TEST(otbVCAImageFilterTestHighSNR);
  REGISTER_TEST(otbHorizontalSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbHorizontalSobelVectorImageFilterTest);
  REGISTER_TEST(otbVerticalSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbVerticalSobelVectorImageFilterTest);
  REGISTER_TEST(otbSobelVectorImageFilterNewTest);
  REGISTER_TEST(otbSobelVectorImageFilterTest);
  REGISTER_TEST(otbLocalActivityVectorImageFilterNewTest);
  REGISTER_TEST(otbLocalActivityVectorImageFilterTest);
  REGISTER_TEST(otbNormalizeVectorImageFilterNewTest);
  REGISTER_TEST(otbNormalizeVectorImageFilterTest);
  REGISTER_TEST(otbPCAImageFilterNewTest);
  REGISTER_TEST(otbPCAImageFilterTest);
  REGISTER_TEST(otbMNFImageFilterNewTest);
  REGISTER_TEST(otbMNFImageFilterTest);
  REGISTER_TEST(otbNAPCAImageFilterNewTest);
  REGISTER_TEST(otbNAPCAImageFilterTest);
}
