#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbParserXTestNew);
  REGISTER_TEST(otbParserXTest);
  REGISTER_TEST(otbMaskMuParserFilterNew);
  REGISTER_TEST(otbMaskMuParserFilterTest);
  REGISTER_TEST(otbParserConditionDataNodeFeatureFunctionNew);
  REGISTER_TEST(otbParserConditionDataNodeFeatureFunction);
  REGISTER_TEST(otbParserTestNew);
  REGISTER_TEST(otbParserTest);
  REGISTER_TEST(otbImageListToSingleImageFilterNew);
  REGISTER_TEST(otbImageListToSingleImageFilter);
  REGISTER_TEST(otbBandMathXImageFilterNew);
  REGISTER_TEST(otbBandMathXImageFilter);
  REGISTER_TEST(otbBandMathXImageFilterConv);
  REGISTER_TEST(otbBandMathXImageFilterTxt);
  REGISTER_TEST(otbBandMathXImageFilterWithIdx);
  REGISTER_TEST(otbBandMathImageFilterNew);
  REGISTER_TEST(otbBandMathImageFilter);
  REGISTER_TEST(otbBandMathImageFilterWithIdx);
}
