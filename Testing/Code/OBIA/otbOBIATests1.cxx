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
REGISTER_TEST(otbMinMaxAttributesLabelMapFilterNew);
REGISTER_TEST(otbMinMaxAttributesLabelMapFilter);
REGISTER_TEST(otbNormalizeAttributesLabelMapFilterNew);
REGISTER_TEST(otbNormalizeAttributesLabelMapFilter);
REGISTER_TEST(otbKMeansAttributesLabelMapFilterNew);
REGISTER_TEST(otbKMeansAttributesLabelMapFilter);
REGISTER_TEST(otbBandsStatisticsAttributesLabelMapFilterNew);
REGISTER_TEST(otbBandsStatisticsAttributesLabelMapFilter);
REGISTER_TEST(otbShapeAttributesLabelMapFilterNew);
REGISTER_TEST(otbStatisticsAttributesLabelMapFilterNew);
REGISTER_TEST(otbVectorDataToLabelMapFilterNew);
REGISTER_TEST(otbVectorDataToLabelMapFilter);
REGISTER_TEST(otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew);
REGISTER_TEST(otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter);
REGISTER_TEST(otbHooverMatrixFilterNew);
REGISTER_TEST(otbHooverMatrixFilter);
REGISTER_TEST(otbHooverInstanceFilterNew);
REGISTER_TEST(otbLabelMapToAttributeImageFilterNew);
REGISTER_TEST(otbHooverInstanceFilterToAttributeImage);
}
