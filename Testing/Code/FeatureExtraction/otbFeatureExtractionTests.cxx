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
REGISTER_TEST(otbComplexMomentPathNew);
REGISTER_TEST(otbComplexMomentPath);
REGISTER_TEST(otbHuPathNew);
REGISTER_TEST(otbHuPath);
REGISTER_TEST(otbFlusserPathNew);
REGISTER_TEST(otbFlusserPath);
REGISTER_TEST(otbTouziEdgeDetectorNew);
REGISTER_TEST(otbTouziEdgeDetector);
REGISTER_TEST(otbTouziEdgeDetectorDirection);
REGISTER_TEST(otbLineRatioDetectorNew);
REGISTER_TEST(otbLineRatioDetector);
REGISTER_TEST(otbLineRatioDetectorLinear);
REGISTER_TEST(otbLineCorrelationDetectorNew);
REGISTER_TEST(otbLineCorrelationDetectorLinear);
REGISTER_TEST(otbLineCorrelationDetector);
REGISTER_TEST(otbAssociativeSymmetricalSumNew);
REGISTER_TEST(otbAssociativeSymmetricalSum);
REGISTER_TEST(otbAssymmetricFusionOfLineDetectorNew);
REGISTER_TEST(otbAssymmetricFusionOfLineDetector);
REGISTER_TEST(otbHarrisImage);
REGISTER_TEST(otbMultiplyByScalarImageFilterTest);
REGISTER_TEST(otbThresholdImageToPointSetTest);
REGISTER_TEST(otbHarrisImageToPointSet);
REGISTER_TEST(otbPixelSuppressionByDirectionNew);
}
