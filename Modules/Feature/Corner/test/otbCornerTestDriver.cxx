#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilter);
  REGISTER_TEST(otbVectorDataToRightAngleVectorDataFilterNew);
  REGISTER_TEST(otbVectorDataToRightAngleVectorDataFilter);
  REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii);
  REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilterNew);
  REGISTER_TEST(otbHarrisImage);
  REGISTER_TEST(otbHarrisToPointSet);
}
