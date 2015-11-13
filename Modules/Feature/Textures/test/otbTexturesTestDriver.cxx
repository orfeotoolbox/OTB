#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbScalarImageToHigherOrderTexturesFilterNew);
  REGISTER_TEST(otbScalarImageToHigherOrderTexturesFilter);
  REGISTER_TEST(otbHaralickTexturesImageFunctionNew);
  REGISTER_TEST(otbHaralickTexturesImageFunction);
  REGISTER_TEST(otbGreyLevelCooccurrenceIndexedList);
  REGISTER_TEST(otbScalarImageToTexturesFilter);
  REGISTER_TEST(otbScalarImageToTexturesFilterNew);
  REGISTER_TEST(otbSFSTexturesImageFilterTest);
  REGISTER_TEST(otbSFSTexturesImageFilterNew);
  REGISTER_TEST(otbScalarImageToPanTexTextureFilterNew);
  REGISTER_TEST(otbScalarImageToAdvancedTexturesFilterNew);
  REGISTER_TEST(otbGreyLevelCooccurrenceIndexedListNew);
  REGISTER_TEST(otbScalarImageToAdvancedTexturesFilter);
  REGISTER_TEST(otbScalarImageToPanTexTextureFilter);
}
