#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew);
  REGISTER_TEST(otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter);
  REGISTER_TEST(otbConnectedComponentMuParserFunctorTest);
  REGISTER_TEST(otbMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter);
  REGISTER_TEST(otbLabelObjectOpeningMuParserFilterNew);
  REGISTER_TEST(otbLabelObjectOpeningMuParserFilterTest);
}
