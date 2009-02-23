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


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbSVMModelNew);
  REGISTER_TEST(otbSVMModelAllocateProblem);
  REGISTER_TEST(otbSVMModelLoad);
  REGISTER_TEST(otbSVMModelAccessor);
  REGISTER_TEST(otbSVMModelLoadSave);
  REGISTER_TEST(otbSVMModelGenericKernelLoadSave);
//REGISTER_TEST(otbSVMMembershipFunctionNew);
//REGISTER_TEST(otbSVMMembershipFunctionLoadModel);
  REGISTER_TEST(otbSVMImageModelEstimatorNew);
  REGISTER_TEST(otbSVMImageModelEstimatorTrain);
  REGISTER_TEST(otbSVMImageModelEstimatorTrainOneClass);
  REGISTER_TEST(otbSVMImageModelEstimatorModelAccessor);
}
