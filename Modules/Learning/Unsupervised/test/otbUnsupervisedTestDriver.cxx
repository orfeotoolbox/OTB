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
#include "otbTestMain.h"
void RegisterTests()
{
#ifdef OTB_USE_SHARK
  REGISTER_TEST(otbSharkKMeansMachineLearningModelCanRead);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelNew);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelTrain);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelPredict);
  REGISTER_TEST(otbSharkUnsupervisedImageClassificationFilter);
#endif
}
