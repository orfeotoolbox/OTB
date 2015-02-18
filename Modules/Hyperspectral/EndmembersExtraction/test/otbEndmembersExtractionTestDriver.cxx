#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbEigenvalueLikelihoodMaximizationNewTest);
  REGISTER_TEST(otbEigenvalueLikelihoodMaximizationTest);
  REGISTER_TEST(otbVirtualDimensionalityNewTest);
  REGISTER_TEST(otbVirtualDimensionalityTest);
  REGISTER_TEST(otbVCAImageFilterNew);
  REGISTER_TEST(otbVCAImageFilterTestHighSNR);
}
