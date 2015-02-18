#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbLabeledSampleLocalizationGeneratorNew);
  REGISTER_TEST(otbLabeledSampleLocalizationGenerator);
  REGISTER_TEST(otbStandardMetaImageFunctionBuilderNew);
  REGISTER_TEST(otbStandardMetaImageFunctionBuilder);
  REGISTER_TEST(otbDescriptorsListSampleGeneratorNew);
  REGISTER_TEST(otbDescriptorsListSampleGenerator);
  REGISTER_TEST(otbDescriptorsSVMModelCreation);
}
