#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbVectorDataToMapFilterWorld);
  REGISTER_TEST(otbVectorDataToMapFilter);
  REGISTER_TEST(otbVectorDataToMapFilterBinary);
  REGISTER_TEST(otbVectorDataToMapFilterNew);
  REGISTER_TEST(otbVectorDataToMapFilterNoFonts);
  REGISTER_TEST(otbVectorDataToImageFilter);
  REGISTER_TEST(otbVectorDataToImageFilterBinary);
  REGISTER_TEST(otbVectorDataToMapFilterSensorModel);
}
