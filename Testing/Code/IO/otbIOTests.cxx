// $Id$
// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "otbTestMain.h" 


void RegisterTests()
{
REGISTER_TEST(otbCAIImageIOTestCanRead);
REGISTER_TEST(otbCAIImageIOTestCanWrite);
REGISTER_TEST(otbGDALImageIOTestCanRead);
REGISTER_TEST(otbONERAImageIOTestCanRead);
REGISTER_TEST(otbImageFileReaderTest);
REGISTER_TEST(otbImageFileWriterTest);
REGISTER_TEST(otbVectorImageFileReaderWriterTest);
REGISTER_TEST(otbImageFileReaderRGBTest);
REGISTER_TEST(otbImageFileWriterRGBTest);
REGISTER_TEST(otbImageTest);
REGISTER_TEST(otbMultiToMonoChannelExtractROISAR);
REGISTER_TEST(otbImageFileReaderERS);
REGISTER_TEST(otbImageFileReaderONERATest);
REGISTER_TEST(otbImageFileReaderONERAComplex);
REGISTER_TEST(otbStreamingImageFilterTest);
REGISTER_TEST(otbStreamingImageFileWriterTest);
REGISTER_TEST(otbVectorImageTest);
}
