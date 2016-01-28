#include "otbTestMain.h"
void RegisterTests()
{
  //##>REGISTER_TEST(otbMeanShiftSmoothingImageFilter);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterSpatialStability);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterNew);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterThreading);
}
