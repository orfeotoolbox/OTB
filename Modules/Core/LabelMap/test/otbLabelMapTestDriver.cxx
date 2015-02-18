#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbAttributesMapLabelObjectWithClassLabelNew);
  REGISTER_TEST(otbLabelObjectMapVectorizer);
  REGISTER_TEST(otbAttributesMapOpeningLabelMapFilterNew);
  REGISTER_TEST(otbLabelImageToLabelMapWithAdjacencyFilterNew);
  REGISTER_TEST(otbLabelImageToLabelMapWithAdjacencyFilter);
  REGISTER_TEST(otbStatisticsAttributesLabelMapFilterNew);
  REGISTER_TEST(otbLabelObjectToPolygonFunctorNew);
  REGISTER_TEST(otbLabelMapWithClassLabelToLabeledSampleListFilterNew);
  REGISTER_TEST(otbAttributesMapLabelObjectNew);
  REGISTER_TEST(otbLabelMapToSampleListFilterNew);
  REGISTER_TEST(otbLabelMapSourceNew);
  REGISTER_TEST(otbLabelMapToAttributeImageFilterNew);
}
