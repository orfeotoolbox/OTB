#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbMassOfBeliefDSApplied);
  REGISTER_TEST(otbConfusionMatrixToMassOfBeliefNew);
  REGISTER_TEST(otbConfusionMatrixToMassOfBeliefTest);
  REGISTER_TEST(otbVectorDataToDSValidatedVectorDataFilterNew);
  REGISTER_TEST(otbVectorDataToDSValidatedVectorDataFilter);
  REGISTER_TEST(otbDempsterShaferFusionOptRecConfMatTest);
  REGISTER_TEST(otbDempsterShaferFusionOptRecTest);
  REGISTER_TEST(otbDempsterShaferFusionOptTest);
  REGISTER_TEST(otbDempsterShaferFusionTest);
  REGISTER_TEST(otbDempsterShaferFusionOptRecConfMatFileTest);
  REGISTER_TEST(otbDempsterShaferFusionConfMatFileTest);
  REGISTER_TEST(otbDSFusionOfClassifiersImageFilterNew);
  REGISTER_TEST(otbDSFusionOfClassifiersImageFilterTest);
  REGISTER_TEST(otbJointMassOfBeliefFilter);
  REGISTER_TEST(otbJointMassOfBeliefFilterLimit);
  REGISTER_TEST(otbMassOfBelief);
  REGISTER_TEST(otbStandardDSCostFunctionNew);
}
