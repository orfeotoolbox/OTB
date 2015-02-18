#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbVectorDataFileReaderWriter);
  REGISTER_TEST(otbVectorDataFileWriterPolygons);
  REGISTER_TEST(otbVectorDataFileReaderNew);
  REGISTER_TEST(otbVectorDataIOFactory);
  REGISTER_TEST(otbVectorDataFileWriterMultiPolygons);
  REGISTER_TEST(otbVectorDataFileReader);
  REGISTER_TEST(otbVectorDataFileGeoReaderWriter);
  REGISTER_TEST(otbVectorDataFileWriterNew);
  REGISTER_TEST(otbVectorDataFileWriter);
}
