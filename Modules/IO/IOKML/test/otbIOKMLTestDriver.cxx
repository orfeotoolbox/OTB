#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbKMLVectorDataIONew);
  REGISTER_TEST(otbKMLVectorDataIOTestFileReader);
  REGISTER_TEST(otbKMLVectorDataIOTestCanWrite);
  REGISTER_TEST(otbKMLVectorDataIOTestCanRead);
}
