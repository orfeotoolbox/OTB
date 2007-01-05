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
REGISTER_TEST(SVMImageClassificationExampleTest);
REGISTER_TEST(SVMImageEstimatorClassificationMultiExampleTest);
REGISTER_TEST(GenerateTrainingImageExampleTest);
REGISTER_TEST(SVMImageModelEstimatorExampleTest);
}

#undef main
#define main SVMImageClassificationExampleTest
#include "SVMImageClassificationExample.cxx"

#undef main
#define main SVMImageEstimatorClassificationMultiExampleTest
#include "SVMImageEstimatorClassificationMultiExample.cxx"

#undef main
#define main GenerateTrainingImageExampleTest
#include "GenerateTrainingImageExample.cxx"

#undef main
#define main SVMImageModelEstimatorExampleTest
#include "SVMImageModelEstimatorExample.cxx"

