#include "otbTestMain.h"
void RegisterTests()
{
#ifdef OTB_USE_SHARK
  REGISTER_TEST(otbSharkRFMachineLearningModelNew);
  REGISTER_TEST(otbSharkRFMachineLearningModel);
  REGISTER_TEST(otbSharkRFMachineLearningModelCanRead);
#endif
  
}
