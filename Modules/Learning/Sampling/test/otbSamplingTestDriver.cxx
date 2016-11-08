#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSamplingRateCalculatorNew);
  REGISTER_TEST(otbSamplingRateCalculator);
  REGISTER_TEST(otbOGRDataToSamplePositionFilterNew);
  REGISTER_TEST(otbOGRDataToSamplePositionFilter);
  REGISTER_TEST(otbOGRDataToSamplePositionFilterPattern);
  REGISTER_TEST(otbOGRDataToClassStatisticsFilterNew);
  REGISTER_TEST(otbOGRDataToClassStatisticsFilter);
  REGISTER_TEST(otbImageSampleExtractorFilterNew);
  REGISTER_TEST(otbImageSampleExtractorFilter);
  REGISTER_TEST(otbImageSampleExtractorFilterUpdate);
  REGISTER_TEST(otbSamplingRateCalculatorListNew);
  REGISTER_TEST(otbSamplingRateCalculatorList);
}
