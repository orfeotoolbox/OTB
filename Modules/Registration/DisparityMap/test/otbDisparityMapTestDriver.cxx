#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbDisparityMapEstimationMethod);
  REGISTER_TEST(otbDisparityMapToDEMFilterNew);
  REGISTER_TEST(otbDisparityMapToDEMFilter);
  REGISTER_TEST(otbDisparityMapMedianFilterNew);
  REGISTER_TEST(otbDisparityMapMedianFilter);
  REGISTER_TEST(otbDisparityTranslateFilterNew);
  REGISTER_TEST(otbDisparityTranslateFilter);
  REGISTER_TEST(otbSubPixelDisparityImageFilterNew);
  REGISTER_TEST(otbSubPixelDisparityImageFilter);
  REGISTER_TEST(otbDisparityMapEstimationMethodNew);
  REGISTER_TEST(otbDisparityMapTo3DFilterNew);
  REGISTER_TEST(otbDisparityMapTo3DFilter);
  REGISTER_TEST(otbMultiDisparityMapTo3DFilterNew);
  REGISTER_TEST(otbMultiDisparityMapTo3DFilter);
  REGISTER_TEST(otbFineRegistrationImageFilterNew);
  REGISTER_TEST(otbFineRegistrationImageFilterTest);
  REGISTER_TEST(otbNCCRegistrationFilter);
  REGISTER_TEST(otbNCCRegistrationFilterNew);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilter);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilterNew);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilterNCC);
}
