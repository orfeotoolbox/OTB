#include "otbTestMain.h"
void RegisterTests()
{

  REGISTER_TEST(otbContingencyTableCalculatorNew);
  REGISTER_TEST(otbContingencyTableCalculatorSetListSamples);
  REGISTER_TEST(otbContingencyTableCalculatorCompute);
  REGISTER_TEST(otbContingencyTableCalculatorComputeWithBaseline);

#ifdef OTB_USE_SHARK
  REGISTER_TEST(otbSharkKMeansMachineLearningModelCanRead);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelNew);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelTrain);
  REGISTER_TEST(otbSharkKMeansMachineLearningModelPredict);
#endif
}
