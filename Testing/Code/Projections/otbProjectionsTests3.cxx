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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbCompositeTransformNew);
  REGISTER_TEST(otbCompositeTransform);
  REGISTER_TEST(otbGenericMapProjectionNew);
  REGISTER_TEST(otbGenericMapProjection);
  REGISTER_TEST(otbGenericRSTransformNew);
  REGISTER_TEST(otbGenericRSTransform);
  REGISTER_TEST(otbGenericRSTransformWithSRID);
  REGISTER_TEST(otbGenericRSTransformFromImage);
  REGISTER_TEST(otbGenericRSTransformImageAndMNTToWGS84ConversionChecking);
  REGISTER_TEST(otbVectorDataProjectionFilterNew);
  REGISTER_TEST(otbVectorDataProjectionFilter);
  REGISTER_TEST(otbVectorDataProjectionFilterFromMapToSensor);
  REGISTER_TEST(otbVectorDataProjectionFilterFromMapToGeo);
  REGISTER_TEST(otbVectorDataProjectionFilterFromMapToImage);
  REGISTER_TEST(otbVectorDataProjectionFilterFromGeoToMap);
  REGISTER_TEST(otbGeometriesProjectionFilter);
  REGISTER_TEST(otbGeocentricTransformNew);
  REGISTER_TEST(otbGeocentricTransform);
  REGISTER_TEST(otbTileMapTransform);
  REGISTER_TEST(otbVectorDataExtractROIandProjection);
  REGISTER_TEST(otbVectorDataIntoImageProjectionFilterTest);
  REGISTER_TEST(otbGenericRSTransformGenericConversionCheckingFromGCP);
  REGISTER_TEST(otbForwardBackwardProjectionConsistency);
  REGISTER_TEST(otbGeometriesProjectionFilterFromMapToEPSG);

}
