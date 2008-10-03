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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbDisparityMapEstimationMethodNew);
REGISTER_TEST(otbDisparityMapEstimationMethod);
REGISTER_TEST(otbPointSetToDeformationFieldGeneratorNew);
REGISTER_TEST(otbNearestPointDeformationFieldGeneratorNew);
REGISTER_TEST(otbNearestPointDeformationFieldGenerator);
REGISTER_TEST(otbNNearestPointsLinearInterpolateDeformationFieldGeneratorNew);
REGISTER_TEST(otbNNearestPointsLinearInterpolateDeformationFieldGenerator);
REGISTER_TEST(otbBSplinesInterpolateDeformationFieldGeneratorNew);
REGISTER_TEST(otbBSplinesInterpolateDeformationFieldGenerator);
REGISTER_TEST(otbPointSetWithTransformToDeformationFieldGeneratorNew);
REGISTER_TEST(otbNCCRegistrationFilterNew);
REGISTER_TEST(otbMIRegistrationFilterNew);  
}
