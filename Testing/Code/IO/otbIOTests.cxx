// $Id$
// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//#define MAIN

#include <iostream>
#include "otbTestMain.h" 


void RegisterTests()
{
REGISTER_TEST(otbCAIImageIOTestCanRead);
REGISTER_TEST(otbCAIImageIOTestCanWrite); // PB compilation multi definition CAI_ERREUR
REGISTER_TEST(otbGDALImageIOTest);
REGISTER_TEST(otbImageFileReaderTest);
REGISTER_TEST(otbImageFileWriterTest);
REGISTER_TEST(otbImageFileReaderRGBTest);
REGISTER_TEST(otbImageFileWriterRGBTest);
REGISTER_TEST(otbExtractROI_VectorToRGB);
REGISTER_TEST(otbExtractROI_VectorToScalar);
}
