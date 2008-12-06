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

// this file defines the otbSpatialReasoningTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(otbImageToImageRCC8CalculatorNew);
REGISTER_TEST(otbImageToImageRCC8Calculator);
REGISTER_TEST(otbPolygonToPolygonRCC8CalculatorNew);
REGISTER_TEST(otbPolygonToPolygonRCC8Calculator);
REGISTER_TEST(otbRCC8VertexBaseNew);
REGISTER_TEST(otbRCC8VertexBase);
REGISTER_TEST(otbRCC8VertexWithCompacityNew);
REGISTER_TEST(otbRCC8VertexWithCompacity);
REGISTER_TEST(otbRCC8EdgeNew);
REGISTER_TEST(otbRCC8Edge);
REGISTER_TEST(otbRCC8GraphNew);
REGISTER_TEST(otbRCC8Graph);
}
