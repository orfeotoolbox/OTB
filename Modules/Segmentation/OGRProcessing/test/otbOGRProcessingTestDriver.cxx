#include "otbTestMain.h"
void RegisterTests()
{
#ifdef OTB_USE_EDISON
  REGISTER_TEST(otbStreamingImageToOGRLayerSegmentationFilterNew);
  REGISTER_TEST(otbStreamingImageToOGRLayerSegmentationFilter);
#endif
  REGISTER_TEST(otbOGRLayerStreamStitchingFilter);
}
