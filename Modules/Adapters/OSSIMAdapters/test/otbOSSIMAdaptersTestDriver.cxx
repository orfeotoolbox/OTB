#include "otbTestMain.h"
void RegisterTests()
{
  //##>REGISTER_TEST(otbImageKeywordlist);
  REGISTER_TEST(otbTestImageKeywordlist);
  REGISTER_TEST(otbOssimJpegFileRessourceLeakTest);
  REGISTER_TEST(otbMapProjectionAdapterTest);
  REGISTER_TEST(otbOssimElevManagerTest2);
  REGISTER_TEST(otbOssimElevManagerTest4);
  REGISTER_TEST(otbGeometricSarSensorModelAdapterNewTest);
  REGISTER_TEST(otbGeometricSarSensorModelAdapterTest);
  REGISTER_TEST(otbPlatformPositionComputeBaselineNewTest);
  REGISTER_TEST(otbPlatformPositionComputeBaselineTest);
  REGISTER_TEST(otbDEMHandlerTest);
  REGISTER_TEST(otbRPCSolverAdapterTest);
}
