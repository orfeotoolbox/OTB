#include "otbTestMain.h"
void RegisterTests()
{
#ifdef OTB_USE_SHARK
  REGISTER_TEST(otbSharkKMeansMachineLearningModelCanRead);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelNew);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelTrain);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelPredict);
#endif
}
