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

#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(ScalarImageMarkovRandomField1Test);
  REGISTER_TEST(ScalarImageKmeansClassifierTest);
  REGISTER_TEST(ClassificationMapRegularizationExampleTest);
}

#undef main
#define main ScalarImageMarkovRandomField1Test
#include "ScalarImageMarkovRandomField1.cxx"

#undef main
#define main ScalarImageKmeansClassifierTest
#include "ScalarImageKmeansClassifier.cxx"

#undef main
#define main ClassificationMapRegularizationExampleTest
#include "ClassificationMapRegularizationExample.cxx"
