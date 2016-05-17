#include "otbTestMain.h"

void RegisterTests()
{
   REGISTER_TEST(otbReadWriteTest);
   REGISTER_TEST(otbMPIReadWriteTest);
   REGISTER_TEST(otbMPIBundleToPerfectSensorTest);
}

