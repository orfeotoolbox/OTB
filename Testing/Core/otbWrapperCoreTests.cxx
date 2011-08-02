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

// this file defines the otbProjectionsTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbWrapperApplicationNew);

  REGISTER_TEST(otbWrapperParameterNew);
  REGISTER_TEST(otbWrapperParameterTest1);

  REGISTER_TEST(otbWrapperChoiceParameterNew);
  REGISTER_TEST(otbWrapperChoiceParameterTest1);

  REGISTER_TEST(otbWrapperNumericalParameterNew);
  REGISTER_TEST(otbWrapperNumericalParameterTest1);

  REGISTER_TEST(otbWrapperEmptyParameterNew);

  REGISTER_TEST(otbWrapperInputImageParameterNew);
  REGISTER_TEST(otbWrapperInputVectorDataParameterNew);

  REGISTER_TEST(otbWrapperStringParameterNew);
  REGISTER_TEST(otbWrapperStringParameterTest1);

  REGISTER_TEST(otbWrapperParameterListNew);

  REGISTER_TEST(otbWrapperApplicationRegistry);

  REGISTER_TEST(otbParameterKey);
}
