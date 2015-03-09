#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbJPEG2000ImageIOTestCanWrite);
  REGISTER_TEST(otbJPEG2000ImageIOTestCanRead);
  REGISTER_TEST(otbGenerateClassicalQLWithJPEG2000);
}
