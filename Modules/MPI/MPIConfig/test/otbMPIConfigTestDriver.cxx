#include "otbTestMain.h"
void RegisterTests()
{
   REGISTER_TEST(otbMPIEmptyTest);
   REGISTER_TEST(otbMPIHelloTest);
   REGISTER_TEST(otbReadWriteTest);
   REGISTER_TEST(otbMPIReadWriteTest);
   REGISTER_TEST(otbMPISPTWReadWriteTest);
   REGISTER_TEST(otbMPIBundleToPerfectSensorTest);
}

