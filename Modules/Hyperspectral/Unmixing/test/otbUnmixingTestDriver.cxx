#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbMDMDNMFImageFilterNewTest);
  REGISTER_TEST(otbMDMDNMFImageFilterTest);
  REGISTER_TEST(otbMDMDNMFImageFilterTest2);
  REGISTER_TEST(otbNCLSUnmixingImageFilterNewTest);
  REGISTER_TEST(otbNCLSUnmixingImageFilterTest);
  REGISTER_TEST(otbISRAUnmixingImageFilterNewTest);
  REGISTER_TEST(otbISRAUnmixingImageFilterTest);
  REGISTER_TEST(otbUnConstrainedLeastSquareImageFilterNewTest);
  REGISTER_TEST(otbUnConstrainedLeastSquareImageFilterTest);
  REGISTER_TEST(otbSparseUnmixingImageFilterNew);
  REGISTER_TEST(otbSparseUnmixingImageFilterTest);
}
