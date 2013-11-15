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
  REGISTER_TEST(otbSOMClassifierNew);
  REGISTER_TEST(otbSOMClassifier);
  REGISTER_TEST(otbSOMImageClassificationFilterNew);
  REGISTER_TEST(otbSOMImageClassificationFilter);
  REGISTER_TEST(otbSEMClassifierNew);
  REGISTER_TEST(otbSVMImageClassificationFilterNew);
  REGISTER_TEST(otbSVMImageClassificationFilter);
  REGISTER_TEST(otbSVMImageClassificationWithRuleFilterNew);
  REGISTER_TEST(otbSVMImageClassificationWithRuleFilter);
  REGISTER_TEST(otbSVMModelGenericKernelsTest);
  REGISTER_TEST(otbSVMKernelsTest);
  REGISTER_TEST(otbSVMModelCopyTest);
  REGISTER_TEST(otbSVMModelCopyGenericKernelTest);
  REGISTER_TEST(otbSVMModelCopyComposedKernelTest);
  REGISTER_TEST(otbKMeansImageClassificationFilterNew);
  REGISTER_TEST(otbKMeansImageClassificationFilter);
  REGISTER_TEST(otbSVMInverseCosSpectralAngleKernelFunctorImageModelEstimatorTest);
  REGISTER_TEST(otbSVMInverseCosSpectralAngleKernelFunctorImageClassificationTest);
  REGISTER_TEST(otbSVMMarginSamplerNew);
}
