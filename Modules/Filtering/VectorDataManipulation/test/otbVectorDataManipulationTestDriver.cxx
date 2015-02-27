#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbDBOverlapDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbDBOverlapDataNodeFeatureFunction);
  REGISTER_TEST(otbVectorDataExtractROI);
  REGISTER_TEST(otbVectorDataToSpecificDescriptionFilterBaseNew);
  REGISTER_TEST(otbVectorDataToVectorDataFilterNew);
  REGISTER_TEST(otbPolygonCompacityFunctor);
  REGISTER_TEST(otbVectorDataToRandomLineGeneratorNew);
  REGISTER_TEST(otbVectorDataToRandomLineGenerator);
  REGISTER_TEST(otbConcatenateVectorDataFilterNew);
  REGISTER_TEST(otbConcatenateVectorDataFilter);
  REGISTER_TEST(otbVectorDataExtractROINew);
  REGISTER_TEST(otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction);
}
