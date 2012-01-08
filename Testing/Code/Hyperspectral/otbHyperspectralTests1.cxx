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
  REGISTER_TEST(otbVirtualDimensionalityNewTest);
  REGISTER_TEST(otbVirtualDimensionalityTest);
  REGISTER_TEST(otbUnConstrainedLeastSquareImageFilterNewTest);
  REGISTER_TEST(otbUnConstrainedLeastSquareImageFilterTest);
  REGISTER_TEST(otbISRAUnmixingImageFilterNewTest);
  REGISTER_TEST(otbISRAUnmixingImageFilterTest);
  REGISTER_TEST(otbNCLSUnmixingImageFilterNewTest);
  REGISTER_TEST(otbNCLSUnmixingImageFilterTest);
  REGISTER_TEST(otbFCLSUnmixingImageFilterNewTest);
  REGISTER_TEST(otbFCLSUnmixingImageFilterTest);
  REGISTER_TEST(otbVCAImageFilterNew);
  REGISTER_TEST(otbVCAImageFilterTestHighSNR);
  REGISTER_TEST(otbSparseUnmixingImageFilterNew);
  //REGISTER_TEST(otbSparseUnmixingImageFilterTest);
}
