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


//
#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbLeeFilter);
  REGISTER_TEST(otbFrostFilterNew);
  REGISTER_TEST(otbFrostFilter);
  REGISTER_TEST(otbImageToPointSetFilterTest);
  REGISTER_TEST(otbOpeningClosingMorphologicalFilterNew);
  REGISTER_TEST(otbOpeningClosingMorphologicalFilter);
  REGISTER_TEST(otbClosingOpeningMorphologicalFilterNew);
  REGISTER_TEST(otbClosingOpeningMorphologicalFilter);
  REGISTER_TEST(otbBinaryImageMinimalBoundingRegionCalculatorNew);
  REGISTER_TEST(otbBinaryImageMinimalBoundingRegionCalculator);
  REGISTER_TEST(otbMeanFunctorImageTest);
  REGISTER_TEST(otbTransformPointSetFilterTest);
  REGISTER_TEST(otbRandomPointSetSourceTest);
  REGISTER_TEST(otbPointSetExtractROITest);
}
