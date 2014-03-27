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


#include "otbTestMain.h"

void RegisterTests()
{
#ifdef OTB_USE_JPEG2000
  REGISTER_TEST(otbJPEG2000ImageIOTestCanRead);
  REGISTER_TEST(otbJPEG2000ImageIOTestCanWrite);
#endif
  REGISTER_TEST(otbMultiResolutionReadingInfo);
  REGISTER_TEST(otbVectorImageTest);
  REGISTER_TEST(otbVectorImageLegacyTest);
#ifdef OTB_USE_JPEG2000
  REGISTER_TEST(otbGenerateClassicalQLWithJPEG2000);
#endif
}
