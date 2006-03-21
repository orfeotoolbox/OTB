// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbAlignImageToPath);
REGISTER_TEST(otbFlstTest);
REGISTER_TEST(otbDrawPathList);
REGISTER_TEST(otbDrawPathListTracerLignes);
REGISTER_TEST(otbDrawPathDessinCarre);
REGISTER_TEST(otbDrawPathAlign);
REGISTER_TEST(otbComplexMomentImage);
REGISTER_TEST(otbHuImage);
REGISTER_TEST(otbFlusserImage);
REGISTER_TEST(otbTouziEdgeDetector);
REGISTER_TEST(otbTouziEdgeDetectorDirection);
REGISTER_TEST(otbTupinEdgeDetector);
}
