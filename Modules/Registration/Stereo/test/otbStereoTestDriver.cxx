#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbBijectionCoherencyFilterNew);
  REGISTER_TEST(otbBijectionCoherencyFilter);
  REGISTER_TEST(otbMulti3DMapToDEMFilterNew);
  REGISTER_TEST(otbMulti3DMapToDEMFilterEPSG);
  REGISTER_TEST(otbMulti3DMapToDEMFilterManual);
  REGISTER_TEST(otbMulti3DMapToDEMFilter);
  REGISTER_TEST(otbAdhesionCorrectionFilterNew);
  REGISTER_TEST(otbAdhesionCorrectionFilter);
  REGISTER_TEST(otbStereoSensorModelToElevationMapFilterNew);
  REGISTER_TEST(otbStereoSensorModelToElevationMapFilter);
  REGISTER_TEST(otbStereorectificationDisplacementFieldSource);
  REGISTER_TEST(otbStereorectificationDisplacementFieldSourceNew);
}
