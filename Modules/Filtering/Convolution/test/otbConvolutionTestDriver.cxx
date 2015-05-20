#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbConvolutionImageFilter);
  REGISTER_TEST(otbConvolutionImageFilterNew);
  REGISTER_TEST(otbOverlapSaveConvolutionImageFilterNew);
#if defined(ITK_USE_FFTWD)
  REGISTER_TEST(otbOverlapSaveConvolutionImageFilter);
  REGISTER_TEST(otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter);
#endif
  REGISTER_TEST(otbGaborFilterGenerator);
  REGISTER_TEST(otbGaborFilterGeneratorNew);
}
