#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbRCC8GraphNew);
  REGISTER_TEST(otbRCC8GraphFileReader);
  REGISTER_TEST(otbRCC8GraphIOEndToEnd);
  REGISTER_TEST(otbPolygonListToRCC8GraphFilter);
  REGISTER_TEST(otbRCC8VertexBase);
  REGISTER_TEST(otbRCC8GraphFileReaderNew);
  REGISTER_TEST(otbRCC8VertexWithCompacityNew);
  REGISTER_TEST(otbImageMultiSegmentationToRCC8GraphFilterNew);
  REGISTER_TEST(otbRCC8GraphFileWriterNew);
  REGISTER_TEST(otbPolygonToPolygonRCC8Calculator);
  REGISTER_TEST(otbImageListToRCC8GraphFilterNew);
  REGISTER_TEST(otbImageToImageRCC8Calculator);
  REGISTER_TEST(otbRCC8Edge);
  REGISTER_TEST(otbPolygonListToRCC8GraphFilterNew);
  REGISTER_TEST(otbRCC8VertexWithCompacity);
  REGISTER_TEST(otbRCC8EdgeNew);
  REGISTER_TEST(otbPolygonToPolygonRCC8CalculatorNew);
  REGISTER_TEST(otbRCC8VertexBaseNew);
  REGISTER_TEST(otbRCC8Graph);
  REGISTER_TEST(otbRCC8GraphSourceNew);
  REGISTER_TEST(otbRCC8GraphFileWriter);
  REGISTER_TEST(otbImageMultiSegmentationToRCC8GraphFilter);
  REGISTER_TEST(otbImageToImageRCC8CalculatorNew);
}
