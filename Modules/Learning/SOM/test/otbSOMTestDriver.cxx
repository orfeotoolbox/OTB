#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbSOMbasedImageFilterNew);
  REGISTER_TEST(otbSOM);
  REGISTER_TEST(otbSOMImageClassificationFilter);
  REGISTER_TEST(otbSOMActivationBuilder);
  REGISTER_TEST(otbSOMActivationBuilderNew);
  REGISTER_TEST(otbSOMClassifierNew);
  REGISTER_TEST(otbSOMImageClassificationFilterNew);
  REGISTER_TEST(otbSOMWithMissingValueTest);
  REGISTER_TEST(otbSOMNew);
  REGISTER_TEST(otbSOMMap);
  REGISTER_TEST(otbSOMWithMissingValueNew);
  REGISTER_TEST(otbSOMMapNew);
  REGISTER_TEST(otbPeriodicSOMTest);
  REGISTER_TEST(otbPeriodicSOMNew);
  REGISTER_TEST(otbSOMClassifier);
  REGISTER_TEST(otbSOMbasedImageFilterTest);
}
