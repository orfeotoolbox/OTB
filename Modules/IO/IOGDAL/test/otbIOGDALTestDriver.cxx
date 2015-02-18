#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbGDALImageIOTest_uint8);
  REGISTER_TEST(otbGDALImageIOTest_uint16);
  REGISTER_TEST(otbGDALImageIOTestWriteMetadata);
  REGISTER_TEST(otbGDALOverviewsBuilderNew);
  REGISTER_TEST(otbGDALOverviewsBuilder);
  REGISTER_TEST(otbOGRVectorDataIONew);
  REGISTER_TEST(otbGDALImageIOTestCanWrite);
  REGISTER_TEST(otbOGRVectorDataIOCanWrite);
  REGISTER_TEST(otbGDALReadPxlComplexFloat);
  REGISTER_TEST(otbGDALReadPxlComplexDouble);
  REGISTER_TEST(otbGDALImageIOTestCanRead);
  REGISTER_TEST(otbMultiDatasetReadingInfo);
  REGISTER_TEST(otbOGRVectorDataIOTestCanRead);
}
