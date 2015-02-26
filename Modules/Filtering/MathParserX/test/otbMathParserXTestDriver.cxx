#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbParserXTest);
  REGISTER_TEST(otbBandMathXImageFilter);
  REGISTER_TEST(otbParserXTestNew);
  REGISTER_TEST(otbBandMathXImageFilterNew);
  REGISTER_TEST(otbBandMathXImageFilterConv);
  REGISTER_TEST(otbBandMathXImageFilterTxt);
  REGISTER_TEST(otbBandMathXImageFilterWithIdx);
}
