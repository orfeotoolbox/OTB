#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbLabelizeConfidenceConnectedImageFilter);
  REGISTER_TEST(otbLabelizeConnectedThresholdImageFilter);
  REGISTER_TEST(otbLabelizeConnectedThresholdImageFilterNew);
  REGISTER_TEST(otbLabelizeNeighborhoodConnectedImageFilterNew);
  REGISTER_TEST(otbLabelizeNeighborhoodConnectedImageFilter);
  REGISTER_TEST(otbLabelizeConfidenceConnectedImageFilterNew);
  REGISTER_TEST(otbLabelToBoundaryImageFilter);
  REGISTER_TEST(otbLabelToBoundaryImageFilterNew);
}
