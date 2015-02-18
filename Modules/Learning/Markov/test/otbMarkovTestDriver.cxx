#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbMRFEnergyFisherClassificationNew);
  REGISTER_TEST(otbMRFEnergyFisherClassification);
  REGISTER_TEST(otbMRFSamplerRandom);
  REGISTER_TEST(otbMRFEnergyPottsNew);
  REGISTER_TEST(otbMRFSamplerMAPNew);
  REGISTER_TEST(otbMarkovRandomFieldFilter);
  REGISTER_TEST(otbMRFSamplerRandomMAPNew);
  REGISTER_TEST(otbMRFSamplerRandomNew);
  REGISTER_TEST(otbMRFEnergyGaussianNew);
  REGISTER_TEST(otbMRFOptimizerMetropolisNew);
  REGISTER_TEST(otbMRFSamplerMAP);
  REGISTER_TEST(otbMRFEnergyGaussian);
  REGISTER_TEST(otbMarkovRandomFieldFilterNew);
  REGISTER_TEST(otbMRFOptimizerMetropolis);
  REGISTER_TEST(otbMRFEnergyPotts);
  REGISTER_TEST(otbMRFEnergyEdgeFidelity);
  REGISTER_TEST(otbMRFSamplerRandomMAP);
  REGISTER_TEST(otbMRFEnergyEdgeFidelityNew);
  REGISTER_TEST(otbMRFOptimizerICM);
  REGISTER_TEST(otbMRFEnergyGaussianClassification);
  REGISTER_TEST(otbMRFOptimizerICMNew);
  REGISTER_TEST(otbMRFEnergyGaussianClassificationNew);
}
