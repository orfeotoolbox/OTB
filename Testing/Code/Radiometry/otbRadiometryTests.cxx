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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbRAndNIRVegetationIndexImageFilterNew);
REGISTER_TEST(otbRAndNIRVegetationIndexImageFilter);
REGISTER_TEST(otbSetASetBRAndNIRVegetationIndexImageFilter);
REGISTER_TEST(otbMultiChannelRAndNIRVegetationIndexImageFilterNew);
REGISTER_TEST(otbMultiChannelRAndNIRVegetationIndexImageFilter);
REGISTER_TEST(otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter);
REGISTER_TEST(otbRAndBAndNIRVegetationIndexImageFilterNew);
REGISTER_TEST(otbRAndBAndNIRVegetationIndexImageFilter);
REGISTER_TEST(otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew);
REGISTER_TEST(otbMultiChannelRAndBAndNIRVegetationIndexImageFilter);
REGISTER_TEST(otbImageToLuminanceImageFilterNew);
REGISTER_TEST(otbImageToLuminanceImageFilter);
REGISTER_TEST(otbLuminanceToReflectanceImageFilter);
REGISTER_TEST(otbLuminanceToReflectanceImageFilterNew);
REGISTER_TEST(otbImageToReflectanceImageFilter);
REGISTER_TEST(otbImageToReflectanceImageFilterNew);
}
