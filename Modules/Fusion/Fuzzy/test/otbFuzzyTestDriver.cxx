#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbFuzzyVariableSetValue);
  REGISTER_TEST(otbFuzzyVariable2Values);
  REGISTER_TEST(otbFuzzyVariableNew);
  REGISTER_TEST(otbFuzzyVariableDSApplied);
  REGISTER_TEST(otbFuzzyVariableGetMaxVar);
  REGISTER_TEST(otbFuzzyDescriptorsModelManagerNew);
  REGISTER_TEST(otbFuzzyDescriptorsModelManagerTest);
}
