#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbWindowedSincInterpolateImageCosineFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageLanczosFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageLanczosFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageFunctionBaseNew);
  REGISTER_TEST(otbProlateInterpolateImageFunctionNew);
  REGISTER_TEST(otbBSplineDecompositionImageFilterNew);
  REGISTER_TEST(otbBSplineInterpolateImageFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageBlackmanFunction);
  REGISTER_TEST(otbBSplineDecompositionImageFilter);
  REGISTER_TEST(otbWindowedSincInterpolateImageWelchFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageGaussianFunction);
  REGISTER_TEST(otbGenericInterpolateImageFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageBlackmanFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageCosineFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageHammingFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageWelchFunction);
  REGISTER_TEST(otbBSplineInterpolateImageFunction);
  REGISTER_TEST(otbWindowedSincInterpolateImageGaussianFunctionNew);
  REGISTER_TEST(otbWindowedSincInterpolateImageHammingFunctionNew);
  REGISTER_TEST(otbStreamingTraitsImage);
  REGISTER_TEST(otbStreamingTraitsVectorImage);
}
