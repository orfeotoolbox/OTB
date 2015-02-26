#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbVariableLengthVectorConverterNew);
  REGISTER_TEST(otbImageRegionTileMapSplitter);
  REGISTER_TEST(otbImageRegionAdaptativeSplitterNew);
  REGISTER_TEST(otbImageRegionAdaptativeSplitter);
  REGISTER_TEST(otbRGBAPixelConverterNew);
  REGISTER_TEST(otbRGBAPixelConverter);
  REGISTER_TEST(otbRectangle);
  REGISTER_TEST(otbImageRegionNonUniformMultidimensionalSplitterNew);
  REGISTER_TEST(otbSystemTest);
  REGISTER_TEST(otbParseHdfSubsetName);
  REGISTER_TEST(otbParseHdfFileName);
  REGISTER_TEST(otbImageRegionSquareTileSplitterNew);
  REGISTER_TEST(otbImageRegionSquareTileSplitter);
  REGISTER_TEST(otbImageRegionTileMapSplitterNew);
  REGISTER_TEST(otbImageRegionNonUniformMultidimensionalSplitter);
  REGISTER_TEST(otbRectangleNew);
  REGISTER_TEST(otbConfigurationManagerTest);
}
