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
   REGISTER_TEST(otbLabelImageToVectorDataFilterNew);
   REGISTER_TEST(otbLabelImageToVectorDataFilter);
   REGISTER_TEST(otbLabelImageToOGRDataSourceFilterNew);
   REGISTER_TEST(otbLabelImageToOGRDataSourceFilter);
   REGISTER_TEST(otbVectorDataRasterizeFilterNew);
   REGISTER_TEST(otbVectorDataRasterizeFilter);
   REGISTER_TEST(otbVectorDataToLabelImageFilterNew);
   REGISTER_TEST(otbVectorDataToLabelImageFilter);
   REGISTER_TEST(otbOGRDataSourceToLabelImageFilterNew);
   REGISTER_TEST(otbOGRDataSourceToLabelImageFilter);
   REGISTER_TEST(otbPolygonizationRasterizationTest);
   REGISTER_TEST(otbImageListToVectorImageFilterNew);
   REGISTER_TEST(otbImageListToVectorImageFilter);
   REGISTER_TEST(otbImageListToVectorImageFilter2);
}
