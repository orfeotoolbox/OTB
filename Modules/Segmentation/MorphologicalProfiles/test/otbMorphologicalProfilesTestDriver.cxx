#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbMorphologicalProfilesSegmentationFilter);
  REGISTER_TEST(otbProfileToProfileDerivativeFilterNew);
  REGISTER_TEST(otbGeodesicMorphologyDecompositionImageFilter);
  REGISTER_TEST(otbMorphologicalClosingProfileFilterNew);
  REGISTER_TEST(otbConvexOrConcaveClassificationFilter);
  REGISTER_TEST(otbGeodesicMorphologyLevelingFilter);
  REGISTER_TEST(otbConvexOrConcaveClassificationFilterNew);
  REGISTER_TEST(otbMultiScaleConvexOrConcaveClassificationFilter);
  REGISTER_TEST(otbMorphologicalOpeningProfileFilter);
  REGISTER_TEST(otbGeodesicMorphologyIterativeDecompositionImageFilter);
  REGISTER_TEST(otbGeodesicMorphologyIterativeDecompositionImageFilterNew);
  REGISTER_TEST(otbGeodesicMorphologyLevelingFilterNew);
  REGISTER_TEST(otbClosingOpeningMorphologicalFilter);
  REGISTER_TEST(otbProfileDerivativeToMultiScaleCharacteristicsFilterNew);
  REGISTER_TEST(otbProfileToProfileDerivativeFilter);
  REGISTER_TEST(otbOpeningClosingMorphologicalFilterNew);
  REGISTER_TEST(otbGeodesicMorphologyDecompositionImageFilterNew);
  REGISTER_TEST(otbMorphologicalOpeningProfileFilterNew);
  REGISTER_TEST(otbProfileDerivativeToMultiScaleCharacteristicsFilter);
  REGISTER_TEST(otbMultiScaleConvexOrConcaveClassificationFilterNew);
  REGISTER_TEST(otbClosingOpeningMorphologicalFilterNew);
  REGISTER_TEST(otbOpeningClosingMorphologicalFilter);
  REGISTER_TEST(otbMorphologicalClosingProfileFilter);
}
