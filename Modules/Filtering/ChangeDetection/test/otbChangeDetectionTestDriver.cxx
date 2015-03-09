#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbJHMIChangeDetectionTest);
  REGISTER_TEST(otbMeanDiffChangeDetectionTest);
  REGISTER_TEST(otbMultivariateAlterationDetectorImageFilterNew);
  REGISTER_TEST(otbMultivariateAlterationDetectorImageFilter);
  REGISTER_TEST(otbCBAMIChangeDetectionTest);
  REGISTER_TEST(otbKullbackLeiblerSupervizedDistanceImageFilter);
  REGISTER_TEST(otbCorrelChangeDetectionTest);
  REGISTER_TEST(otbKullbackLeiblerProfileImageFilter);
  REGISTER_TEST(otbKullbackLeiblerSupervizedDistanceImageFilterNew);
  REGISTER_TEST(otbKullbackLeiblerProfileImageFilterNew);
  REGISTER_TEST(otbKullbackLeiblerDistanceImageFilter);
  REGISTER_TEST(otbMeanRatioChangeDetectionTest);
  REGISTER_TEST(otbKullbackLeiblerDistanceImageFilterNew);
  REGISTER_TEST(otbLHMIChangeDetectionTest);
}
