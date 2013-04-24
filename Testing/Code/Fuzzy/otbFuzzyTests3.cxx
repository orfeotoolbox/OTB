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
  REGISTER_TEST(otbDempsterShaferFusionTest);
  REGISTER_TEST(otbDempsterShaferFusionConfMatFileTest);
  REGISTER_TEST(otbDempsterShaferFusionOptTest);
  REGISTER_TEST(otbDempsterShaferFusionOptRecTest);
  REGISTER_TEST(otbDempsterShaferFusionOptRecConfMatTest);
  REGISTER_TEST(otbDempsterShaferFusionOptRecConfMatFileTest);
  REGISTER_TEST(otbConfusionMatrixToMassOfBeliefNew);
  REGISTER_TEST(otbConfusionMatrixToMassOfBeliefTest);
  REGISTER_TEST(otbDSFusionOfClassifiersImageFilterNew);
  REGISTER_TEST(otbDSFusionOfClassifiersImageFilterTest);
}
