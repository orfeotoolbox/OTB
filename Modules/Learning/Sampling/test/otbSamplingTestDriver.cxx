#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSamplingRateCalculatorNew);
  REGISTER_TEST(otbSamplingRateCalculator);
  REGISTER_TEST(otbOGRDataToResampledOGRDataNew);
  REGISTER_TEST(otbOGRDataToResampledOGRData);
  REGISTER_TEST(otbOGRDataToResampledOGRDataXML);
  REGISTER_TEST(otbOGRDataToClassStatisticsFilterNew);
  REGISTER_TEST(otbOGRDataToClassStatisticsFilter);
}
