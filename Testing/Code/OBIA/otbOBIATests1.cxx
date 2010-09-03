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

//
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(otbAttributesMapLabelObjectNew);
REGISTER_TEST(otbAttributesMapLabelObjectWithClassLabelNew);
REGISTER_TEST(otbAttributesMapOpeningLabelMapFilterNew);
REGISTER_TEST(otbImageToLabelMapWithAttributesFilter);
REGISTER_TEST(otbImageToLabelMapWithAttributesFilterNew);
REGISTER_TEST(otbLabelMapSourceNew);
REGISTER_TEST(otbLabelMapToSampleListFilterNew);
REGISTER_TEST(otbLabelMapToSampleListFilter);
REGISTER_TEST(otbLabelMapToVectorDataFilter);
REGISTER_TEST(otbLabelMapToVectorDataFilterNew);
REGISTER_TEST(otbLabelMapWithClassLabelToLabeledSampleListFilterNew);
REGISTER_TEST(otbLabelMapWithClassLabelToLabeledSampleListFilter);
REGISTER_TEST(otbLabelObjectMapVectorizer);
REGISTER_TEST(otbLabelObjectToPolygonFunctorNew);
REGISTER_TEST(otbRadiometricAttributesLabelMapFilterNew);
REGISTER_TEST(otbShapeAttributesLabelMapFilterNew);
REGISTER_TEST(otbStatisticsAttributesLabelMapFilterNew);
REGISTER_TEST(otbVectorDataToLabelMapFilterNew);
REGISTER_TEST(otbVectorDataToLabelMapFilter);
}
