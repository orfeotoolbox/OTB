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
  REGISTER_TEST(otbPhysicalToRPCSensorModelImageFilter);
  REGISTER_TEST(otbGenericRSResampleImageFilterNew);
  REGISTER_TEST(otbGenericRSResampleImageFilter);
  REGISTER_TEST(otbGenericRSResampleImageFilterFromMap);
  REGISTER_TEST(otbImageToEnvelopeVectorDataFilterNew);
  REGISTER_TEST(otbImageToEnvelopeVectorDataFilter);
  REGISTER_TEST(otbImageToGenericRSOutputParametersNew);
  REGISTER_TEST(otbImageToGenericRSOutputParameters);
  REGISTER_TEST(otbRationalTransformNew);
  REGISTER_TEST(otbRationalTransform);
  REGISTER_TEST(otbRationalTransformToDisplacementFieldSourceTest);
  REGISTER_TEST(otbGeographicalDistanceNew);
  REGISTER_TEST(otbGeographicalDistance);
  REGISTER_TEST(otbVectorDataTransformFilterNew);
  REGISTER_TEST(otbVectorDataTransformFilter);
  REGISTER_TEST(otbGenericRSTransformGenericTest);
}
