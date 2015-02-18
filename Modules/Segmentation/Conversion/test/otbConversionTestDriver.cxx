#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbVectorDataToLabelMapFilter);
  REGISTER_TEST(otbOGRDataSourceToLabelImageFilterNew);
  REGISTER_TEST(otbOGRDataSourceToLabelImageFilter);
  REGISTER_TEST(otbVectorDataToLabelMapFilterNew);
  REGISTER_TEST(otbLabelImageToVectorDataFilterNew);
  REGISTER_TEST(otbLabelImageToVectorDataFilter);
  REGISTER_TEST(otbLabelImageToOGRDataSourceFilterNew);
  REGISTER_TEST(otbLabelImageToOGRDataSourceFilter);
  REGISTER_TEST(otbVectorDataToLabelImageFilterNew);
  REGISTER_TEST(otbVectorDataToLabelImageFilter);
  REGISTER_TEST(otbPolygonizationRasterizationTest);
  REGISTER_TEST(otbVectorDataRasterizeFilterNew);
  REGISTER_TEST(otbVectorDataRasterizeFilter);
  REGISTER_TEST(otbLabelImageRegionPruningFilter);
  REGISTER_TEST(otbLabelImageRegionMergingFilter);
  REGISTER_TEST(otbLabelMapToVectorDataFilter);
  REGISTER_TEST(otbLabelMapToVectorDataFilterNew);
}
