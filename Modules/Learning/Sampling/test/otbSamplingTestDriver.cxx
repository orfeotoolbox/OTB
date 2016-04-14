#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSamplingRateCalculatorNew);
  REGISTER_TEST(otbSamplingRateCalculator);
  REGISTER_TEST(otbOGRDataToSamplePositionFilterNew);
  REGISTER_TEST(otbOGRDataToSamplePositionFilter);
  REGISTER_TEST(otbOGRDataToSamplePositionFilterXML);
  REGISTER_TEST(otbOGRDataToClassStatisticsFilterNew);
  REGISTER_TEST(otbOGRDataToClassStatisticsFilter);
}
