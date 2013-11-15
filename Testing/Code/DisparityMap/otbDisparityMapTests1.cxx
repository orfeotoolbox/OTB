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

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbDisparityMapEstimationMethodNew);
  REGISTER_TEST(otbDisparityMapEstimationMethod);
  REGISTER_TEST(otbPointSetToDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNearestPointDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNearestPointDisplacementFieldGenerator);
  REGISTER_TEST(otbNNearestPointsLinearInterpolateDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNNearestPointsLinearInterpolateDisplacementFieldGenerator);
  REGISTER_TEST(otbBSplinesInterpolateDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbBSplinesInterpolateDisplacementFieldGenerator);
  REGISTER_TEST(otbPointSetWithTransformToDisplacementFieldGeneratorNew);
  REGISTER_TEST(otbNCCRegistrationFilterNew);
  REGISTER_TEST(otbNCCRegistrationFilter);
}
