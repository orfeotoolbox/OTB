// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h" 

void RegisterTests()
{
REGISTER_TEST(otbExtractROI);
REGISTER_TEST(otbExtractROI_RGB);
REGISTER_TEST(otbMultiChannelImage );
REGISTER_TEST(otbMultiChannelExtractROItoto );


//REGISTER_TEST(otbConvertFormatImage );
//REGISTER_TEST(itkAffineTransformTest );
}
