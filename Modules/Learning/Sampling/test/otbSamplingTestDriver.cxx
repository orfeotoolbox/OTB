#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSamplingRateCalculatorNew);
  REGISTER_TEST(otbSamplingRateCalculatorUI);
  REGISTER_TEST(otbSamplingRateCalculatorS);
  REGISTER_TEST(otbSamplingRateCalculatorSmallest);
  REGISTER_TEST(otbOGRDataToResampledOGRData);
}
