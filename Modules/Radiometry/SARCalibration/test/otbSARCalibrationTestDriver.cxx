#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSarBrightnessFunction);
  REGISTER_TEST(otbSarBrightnessToImageFilterTestWithoutNoise);
  REGISTER_TEST(otbTerraSarBrightnessImageFilterTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithRealPixelTest);
  REGISTER_TEST(otbTerraSarBrightnessImageFilterNew);
  REGISTER_TEST(otbSarParametricMapFunctionToImageFilter);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise);
  REGISTER_TEST(otbSarBrightnessFunctorWithoutNoise);
  REGISTER_TEST(otbTerraSarBrightnessFunctor);
  REGISTER_TEST(otbSarBrightnessToImageWithComplexPixelFilterTest);
  REGISTER_TEST(otbSarParametricMapFunctionTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterCompareTest);
  REGISTER_TEST(otbSarBrightnessFunctor);
  REGISTER_TEST(otbSarBrightnessFunctionWithoutNoise);
  REGISTER_TEST(otbSarRadiometricCalibrationFunction);
  REGISTER_TEST(otbSarRadiometricCalibrationFunctionWithoutNoise);
  REGISTER_TEST(otbTerraSarBrightnessImageComplexFilterTest);
  REGISTER_TEST(otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest);
  REGISTER_TEST(otbSarBrightnessToImageFilterTest);
}
