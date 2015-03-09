#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbWaveletOperator);
  REGISTER_TEST(otbWaveletPacketTransform);
  REGISTER_TEST(otbWaveletTransform);
  REGISTER_TEST(otbSubsampleImageFilter);
  REGISTER_TEST(otbSubsampleImageFilterNew);
  REGISTER_TEST(otbWaveletInverseFilterBankNew);
  REGISTER_TEST(otbWaveletInverseTransformNew);
  REGISTER_TEST(otbWaveletTransformNew);
  REGISTER_TEST(otbWaveletPacketInverseTransformNew);
  REGISTER_TEST(otbWaveletFilterBank);
  REGISTER_TEST(otbWaveletPacketTransformNew);
  REGISTER_TEST(otbWaveletFilterBankNew);
  REGISTER_TEST(otbWaveletOperatorNew);
}
