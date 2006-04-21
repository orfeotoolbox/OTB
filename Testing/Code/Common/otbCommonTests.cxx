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
REGISTER_TEST(otbExtractROINew);
REGISTER_TEST(otbExtractROI_RGB);
REGISTER_TEST(otbMultiChannelExtractROI );
REGISTER_TEST(otbMultiChannelExtractROINew );
REGISTER_TEST(otbMultiToMonoChannelExtractROI );
REGISTER_TEST(otbMultiToMonoChannelExtractROINew );
REGISTER_TEST(otbLineSpatialObjectList);
REGISTER_TEST(otbPointSetSourceTest);
REGISTER_TEST(otbDrawLineSpatialObjectNew);
REGISTER_TEST(otbDrawLineSpatialObject);
REGISTER_TEST(otbDrawLineSpatialObjectListNew);
REGISTER_TEST(otbDrawLineSpatialObjectList);
REGISTER_TEST(otbImageToLineSpatialObjectListNew);
}
