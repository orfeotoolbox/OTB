#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbFineRegistrationImageFilterNew);
  REGISTER_TEST(otbBijectionCoherencyFilterNew);
  REGISTER_TEST(otbBijectionCoherencyFilter);
  REGISTER_TEST(otbMulti3DMapToDEMFilterNew);
  REGISTER_TEST(otbMulti3DMapToDEMFilterEPSG);
  REGISTER_TEST(otbMulti3DMapToDEMFilterManual);
  REGISTER_TEST(otbMulti3DMapToDEMFilter);
  REGISTER_TEST(otbNCCRegistrationFilter);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilterNew);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilter);
  REGISTER_TEST(otbPixelWiseBlockMatchingImageFilterNCC);
  REGISTER_TEST(otbFineRegistrationImageFilterTest);
  REGISTER_TEST(otbNCCRegistrationFilterNew);
  REGISTER_TEST(otbAdhesionCorrectionFilterNew);
  REGISTER_TEST(otbAdhesionCorrectionFilter);
  REGISTER_TEST(otbStereoSensorModelToElevationMapFilterNew);
  REGISTER_TEST(otbStereoSensorModelToElevationMapFilter);
}
