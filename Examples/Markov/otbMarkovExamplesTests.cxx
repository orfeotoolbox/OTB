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
  REGISTER_TEST(MarkovRandomField1ExampleTest);
  REGISTER_TEST(MarkovRandomField2ExampleTest);
  REGISTER_TEST(MarkovRestaurationExampleTest);
  REGISTER_TEST(MarkovRegularizationExampleTest);
}

#undef main
#define main MarkovRandomField1ExampleTest
#include "MarkovRandomField1Example.cxx"
 
#undef main
#define main MarkovRandomField2ExampleTest
#include "MarkovRandomField2Example.cxx"
 
#undef main
#define main MarkovRestaurationExampleTest
#include "MarkovRestaurationExample.cxx"
 
#undef main
#define main MarkovRegularizationExampleTest
#include "MarkovRegularizationExample.cxx"


