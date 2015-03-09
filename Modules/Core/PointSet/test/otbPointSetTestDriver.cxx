#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbPointSetSourceTest);
  REGISTER_TEST(otbPointSetExtractROITest);
  REGISTER_TEST(otbSimplePointCountStrategyTest);
  REGISTER_TEST(otbThresholdImageToPointSetFilterNew);
  REGISTER_TEST(otbRandomPointSetSourceTest);
  REGISTER_TEST(otbImageToPointSetFilterTest);
  REGISTER_TEST(otbTransformPointSetFilterTest);
  REGISTER_TEST(otbThresholdImageToPointSetTest);
}
