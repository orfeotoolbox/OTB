#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbConvolutionImageFilter);
  REGISTER_TEST(otbConvolutionImageFilterNew);
  REGISTER_TEST(otbOverlapSaveConvolutionImageFilterNew);
  REGISTER_TEST(otbOverlapSaveConvolutionImageFilter);
  REGISTER_TEST(otbCompareOverlapSaveAndClassicalConvolutionWithGaborFilter);
  REGISTER_TEST(otbGaborFilterGenerator);
  REGISTER_TEST(otbGaborFilterGeneratorNew);
}
