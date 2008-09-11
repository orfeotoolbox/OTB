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
#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(ossimIntegrationTest);
REGISTER_TEST(ossimKeywordlistTest);
REGISTER_TEST(ossimElevManagerTest);
REGISTER_TEST(ijBSplineScatteredDataPointSetToImageFilterTest);
REGISTER_TEST(svmGenericKernelFunctor);
REGISTER_TEST(svmTest);
REGISTER_TEST(svmGenericKernelTest);
REGISTER_TEST(SIXSFunctionMainOtbTest);
REGISTER_TEST(tinyXMLlibTest);
REGISTER_TEST(svmGenericKernelBasicOperationsTest);
REGISTER_TEST(otbSVMComposedKernelFunctorTest);
REGISTER_TEST(ossimRadarSatSupport);
REGISTER_TEST(itk2DScaleInvariantFeatureImageFilterTest);
// register openJpeg tests if necessary
#ifdef OTB_COMPILE_JPEG2000 
 REGISTER_TEST(openJpegEncoder);
 REGISTER_TEST(openJpegDecoder);
#endif
// register OPENTHREADS testing
#ifdef OTB_USE_INTERNAL_OPENTHREADS
 REGISTER_TEST(openthreadsSimpleThreader);
 REGISTER_TEST(openthreadsWorkCrew);
#endif
#ifdef OTB_USE_INTERNAL_EXPAT
 REGISTER_TEST(expatruntests);
#endif
}

