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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(ImageRegistration1ExampleTest);
  REGISTER_TEST(ImageRegistration2ExampleTest);
  REGISTER_TEST(ImageRegistration5ExampleTest);
}

#undef main
#define main ImageRegistration1ExampleTest
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate1
#include "ImageRegistration1.cxx"

#undef main
#define main ImageRegistration2ExampleTest
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate2
#include "ImageRegistration2.cxx"

#undef main
#define main ImageRegistration5ExampleTest
#undef CommandIterationUpdate
#define CommandIterationUpdate CommandIterationUpdate5
#include "ImageRegistration5.cxx"

