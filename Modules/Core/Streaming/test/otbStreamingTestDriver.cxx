#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbStreamingManagerNew);
  REGISTER_TEST(otbNumberOfLinesStrippedStreamingManager);
  REGISTER_TEST(otbRAMDrivenStrippedStreamingManager);
  REGISTER_TEST(otbTileDimensionTiledStreamingManager);
  REGISTER_TEST(otbRAMDrivenTiledStreamingManager);
  REGISTER_TEST(otbRAMDrivenAdaptativeStreamingManager);
}
