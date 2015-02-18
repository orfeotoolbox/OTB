#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSavitzkyGolayInterpolationFunctorNew);
  REGISTER_TEST(otbTimeSeriesLeastSquareFittingFunctorTest);
  REGISTER_TEST(otbPolynomialTimeSeriesTest);
  REGISTER_TEST(otbEnvelopeSavitzkyGolayInterpolationFunctorTest);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilter);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterSpatialStability);
  REGISTER_TEST(otbSavitzkyGolayInterpolationFunctorTest);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterNew);
  REGISTER_TEST(otbMeanShiftSmoothingImageFilterThreading);
  REGISTER_TEST(otbTimeSeriesLeastSquareFittingFunctorNew);
  REGISTER_TEST(otbTimeSeriesLeastSquareFittingFunctorWeightsTest);
}
