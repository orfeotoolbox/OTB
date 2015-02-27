#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbComplexMomentPathFloat);
  REGISTER_TEST(otbRadiometricMomentsImageFunctionNew);
  REGISTER_TEST(otbRadiometricMomentsImageFunction);
  REGISTER_TEST(otbComplexMomentPathNew);
  REGISTER_TEST(otbHuMomentsImageFunctionNew);
  REGISTER_TEST(otbHuMomentsImageFunction);
  REGISTER_TEST(otbHuMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbHuMomentsImageFunctionRotationInvariant);
  REGISTER_TEST(otbRealMomentsImageFunctionNew);
  REGISTER_TEST(otbRealMomentsImageFunction);
  REGISTER_TEST(otbComplexMomentsImageFunction);
  REGISTER_TEST(otbComplexMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbHuPathNew);
  REGISTER_TEST(otbFlusserPathNew);
  REGISTER_TEST(otbFlusserMomentsImageFunctionNew);
  REGISTER_TEST(otbFlusserMomentsImageFunction);
  REGISTER_TEST(otbFlusserMomentsImageFunctionScaleInvariant);
  REGISTER_TEST(otbFlusserMomentsImageFunctionRotationInvariant);
  REGISTER_TEST(otbHuPath);
  REGISTER_TEST(otbFlusserPath);
  REGISTER_TEST(otbComplexMomentPath);
  REGISTER_TEST(otbRadiometricMomentsImageFilterNew);
  REGISTER_TEST(otbRadiometricMomentsImageFilter);
  REGISTER_TEST(otbComplexMomentsImageFunctionNew);
}
