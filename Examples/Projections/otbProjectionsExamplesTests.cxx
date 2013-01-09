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
  REGISTER_TEST(OrthoRectificationExampleTest);
  REGISTER_TEST(MapProjectionExampleTest);
  REGISTER_TEST(VectorDataProjectionExampleTest);
  REGISTER_TEST(GeometriesProjectionTest);
  REGISTER_TEST(EstimateRPCSensorModelExample);
}

#undef main
#define main OrthoRectificationExampleTest
#include "OrthoRectificationExample.cxx"

#undef main
#define main MapProjectionExampleTest
#include "MapProjectionExample.cxx"

#undef main
#define main VectorDataProjectionExampleTest
#include "VectorDataProjectionExample.cxx"

#undef main
#define main GeometriesProjectionTest
#include "GeometriesProjectionExample.cxx"

#undef main
#define main EstimateRPCSensorModelExample
#include "EstimateRPCSensorModelExample.cxx"
