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
  REGISTER_TEST(otbLibSVMMachineLearningModelNew);
  REGISTER_TEST(otbLibSVMMachineLearningModel);
  REGISTER_TEST(otbSVMMachineLearningModelNew);
  REGISTER_TEST(otbSVMMachineLearningModel);
  REGISTER_TEST(otbKNearestNeighborsMachineLearningModelNew);
  REGISTER_TEST(otbKNearestNeighborsMachineLearningModel);
  REGISTER_TEST(otbRandomForestsMachineLearningModelNew);
  REGISTER_TEST(otbRandomForestsMachineLearningModel);
  REGISTER_TEST(otbBoostMachineLearningModelNew);
  REGISTER_TEST(otbBoostMachineLearningModel);
  REGISTER_TEST(otbImageClassificationFilterNew);
  REGISTER_TEST(otbImageClassificationFilter);
  REGISTER_TEST(otbLibSVMMachineLearningModelCanRead);
  REGISTER_TEST(otbSVMMachineLearningModelCanRead);
  REGISTER_TEST(otbRandomForestsMachineLearningModelCanRead);
  REGISTER_TEST(otbBoostMachineLearningModelCanRead);
}
