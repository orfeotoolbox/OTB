#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbMapFileProductWriterNew);
  REGISTER_TEST(otbImageToOSMVectorDataGeneratorNew);
  REGISTER_TEST(otbImageToOSMVectorDataGenerator);
  REGISTER_TEST(otbMapFileProductWriter);
  REGISTER_TEST(otbOSMToVectorDataGeneratorNew);
  REGISTER_TEST(otbOSMToVectorDataGeneratorTest);
  REGISTER_TEST(otbOSMToVectorDataGeneratorByName);
  REGISTER_TEST(otbCoordinateToNameTest);
}
