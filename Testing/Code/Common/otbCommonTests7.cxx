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
  REGISTER_TEST(otbGenericInterpolateImageFunctionNew);
  REGISTER_TEST(otbMirrorBoundaryConditionTest);
  REGISTER_TEST(otbStandardFilterWatcherNew);
  REGISTER_TEST(otbDataNodeTest);
  REGISTER_TEST(otbVectorDataNew);
  REGISTER_TEST(otbVectorData);
  REGISTER_TEST(otbShiftScaleImageAdaptorNew);
  REGISTER_TEST(otbShiftScaleImageAdaptor);
  REGISTER_TEST(otbStandardWriterWatcher);
  REGISTER_TEST(otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew);
  REGISTER_TEST(otbUnaryFunctorNeighborhoodWithOffsetImageFilter);
  REGISTER_TEST(otbRemoteSensingRegionNew);
  REGISTER_TEST(otbRemoteSensingRegion);
}
