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

#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbFourierMellinDescriptorsNew);
  REGISTER_TEST(otbFourierMellinDescriptors);
  REGISTER_TEST(otbFourierMellinDescriptorsScaleInvariant);
  REGISTER_TEST(otbFourierMellinDescriptorsRotationInvariant);
  REGISTER_TEST(otbLocalHistogramImageFunctionNew);
  REGISTER_TEST(otbLocalHistogramImageFunctionTest);
  REGISTER_TEST(otbImageFunctionAdapterNew);
  REGISTER_TEST(otbImageFunctionAdapter);
  REGISTER_TEST(otbMetaImageFunctionNew);
  REGISTER_TEST(otbMetaImageFunction);
}
