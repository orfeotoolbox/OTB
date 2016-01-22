#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbGenericRSTransformWithSRID);
  REGISTER_TEST(otbCreateInverseForwardSensorModel);
  REGISTER_TEST(otbForwardBackwardProjectionConsistency);
  REGISTER_TEST(otbGenericRSTransform);
  REGISTER_TEST(otbCreateProjectionWithOSSIM);
  REGISTER_TEST(otbLogPolarTransformResample);
  REGISTER_TEST(otbStreamingWarpImageFilterNew);
  REGISTER_TEST(otbLogPolarTransform);
  REGISTER_TEST(otbGenericRSTransformNew);
  REGISTER_TEST(otbLogPolarTransformNew);
  REGISTER_TEST(otbForwardSensorModelGrid);
  //##>REGISTER_TEST(otbSensorModel);
  REGISTER_TEST(otbGeocentricTransform);
  REGISTER_TEST(otbCreateProjectionWithOTB);
  //##>REGISTER_TEST(otbSensorModelGrid);
  REGISTER_TEST(otbGeocentricTransformNew);
  REGISTER_TEST(otbGenericMapProjection);
  REGISTER_TEST(otbStreamingWarpImageFilter);
  REGISTER_TEST(otbSensorModelsNew);
  REGISTER_TEST(otbGenericMapProjectionNew);
  REGISTER_TEST(otbInverseLogPolarTransform);
  REGISTER_TEST(otbInverseLogPolarTransformNew);
  REGISTER_TEST(otbInverseLogPolarTransformResample);
  REGISTER_TEST(otbStreamingResampleImageFilterWithAffineTransform);
}
