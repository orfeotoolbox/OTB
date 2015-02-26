#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbMaskMuParserFilterNew);
  REGISTER_TEST(otbMaskMuParserFilterTest);
  REGISTER_TEST(otbParserConditionDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbParserConditionDataNodeFeatureFunction);
  REGISTER_TEST(otbParserTestNew);
  REGISTER_TEST(otbParserTest);
  REGISTER_TEST(otbImageListToSingleImageFilterNew);
  REGISTER_TEST(otbImageListToSingleImageFilter);
  REGISTER_TEST(otbBandMathImageFilterNew);
  REGISTER_TEST(otbBandMathImageFilter);
  REGISTER_TEST(otbBandMathImageFilterWithIdx);
}
