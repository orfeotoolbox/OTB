#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbFillGapsFilter);
  REGISTER_TEST(otbLineSpatialObjectListToPointSetFilterNew);
  REGISTER_TEST(otbSpatialObjectToImageDrawingFilter);
  REGISTER_TEST(otbFillGapsFilterNew);
  REGISTER_TEST(otbDrawLineSpatialObject);
  REGISTER_TEST(otbSpatialObjectToImageDrawingFilterNew);
  REGISTER_TEST(otbLineSpatialObjectList);
  REGISTER_TEST(otbLineSpatialObjectNew);
  REGISTER_TEST(otbDrawLineSpatialObjectListNew);
  REGISTER_TEST(otbDrawLineSpatialObjectList);
  REGISTER_TEST(otbSpatialObjectSourceNew);
  REGISTER_TEST(otbImageToLineSpatialObjectListNew);
  REGISTER_TEST(otbDrawLineSpatialObjectNew);
}
