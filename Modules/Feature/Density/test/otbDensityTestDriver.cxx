#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbPointSetDensityFunctionNew);
  REGISTER_TEST(otbPointSetDensityGaussianFunctionNew);
  REGISTER_TEST(otbPointSetToDensityImageFilterNew);
  REGISTER_TEST(otbPointSetDensityGaussianFunctionTest);
  REGISTER_TEST(otbPointSetDensityEpanechnikovFunctionNew);
  REGISTER_TEST(otbPointSetDensityEpanechnikovFunctionTest);
  REGISTER_TEST(otbKeyPointDensityImageFilterTest);
  REGISTER_TEST(otbKeyPointDensityImageFilterNew);
  REGISTER_TEST(otbPointSetToDensityImageFilterTest);
  REGISTER_TEST(otbPointSetDensityFunctionTest);
}
