#include "otbTestMain.h"
void RegisterTests()
{
  REGISTER_TEST(otbConfusionMatrixCalculatorNew);
  REGISTER_TEST(otbConfusionMatrixCalculatorSetListSamples);
  REGISTER_TEST(otbConfusionMatrixCalculatorWrongSize);
  REGISTER_TEST(otbConfusionMatrixCalculatorCompute);
  REGISTER_TEST(otbConfusionMatrixCalculatorComputeWithBaseline);
  REGISTER_TEST(otbConfusionMatrixMeasurementsNew);
  REGISTER_TEST(otbConfusionMatrixMeasurementsTest);
  REGISTER_TEST(otbConfusionMatrixConcatenateTest);
  
  #ifdef OTB_USE_LIBSVM
  REGISTER_TEST(otbLibSVMMachineLearningModelCanRead);
  #endif
  
  #ifdef OTB_USE_OPENCV
  REGISTER_TEST(otbSVMMachineLearningModelCanRead);
  REGISTER_TEST(otbRandomForestsMachineLearningModelCanRead);
  REGISTER_TEST(otbBoostMachineLearningModelCanRead);
  REGISTER_TEST(otbNeuralNetworkMachineLearningModelCanRead);
  REGISTER_TEST(otbNormalBayesMachineLearningModelCanRead);
  REGISTER_TEST(otbDecisionTreeMachineLearningModelCanRead);
  REGISTER_TEST(otbGradientBoostedTreeMachineLearningModelCanRead);
  REGISTER_TEST(otbKNNMachineLearningModelCanRead);
  #endif
  
  #ifdef OTB_USE_LIBSVM
  REGISTER_TEST(otbLibSVMMachineLearningModelNew);
  REGISTER_TEST(otbLibSVMMachineLearningModel);
  REGISTER_TEST(otbLibSVMRegressionTests);
  #endif
  
  #ifdef OTB_USE_OPENCV
  REGISTER_TEST(otbSVMMachineLearningModelNew);
  REGISTER_TEST(otbSVMMachineLearningModel);
  REGISTER_TEST(otbSVMMachineLearningRegressionModel);
  REGISTER_TEST(otbKNearestNeighborsMachineLearningModelNew);
  REGISTER_TEST(otbKNearestNeighborsMachineLearningModel);
  REGISTER_TEST(otbRandomForestsMachineLearningModelNew);
  REGISTER_TEST(otbRandomForestsMachineLearningModel);
  REGISTER_TEST(otbBoostMachineLearningModelNew);
  REGISTER_TEST(otbBoostMachineLearningModel);
  REGISTER_TEST(otbANNMachineLearningModelNew);
  REGISTER_TEST(otbANNMachineLearningModel);
  REGISTER_TEST(otbNormalBayesMachineLearningModelNew);
  REGISTER_TEST(otbNormalBayesMachineLearningModel);
  REGISTER_TEST(otbDecisionTreeMachineLearningModelNew);
  REGISTER_TEST(otbDecisionTreeMachineLearningModel);
  REGISTER_TEST(otbGradientBoostedTreeMachineLearningModelNew);
  REGISTER_TEST(otbGradientBoostedTreeMachineLearningModel);
  REGISTER_TEST(otbNeuralNetworkRegressionTests);
  REGISTER_TEST(otbSVMRegressionTests);
  REGISTER_TEST(otbDecisionTreeRegressionTests);
  REGISTER_TEST(otbGradientBoostedTreeRegressionTests);
  REGISTER_TEST(otbKNearestNeighborsRegressionTests);
  REGISTER_TEST(otbRandomForestsRegressionTests);
#endif
  
    REGISTER_TEST(otbImageClassificationFilterNew);
  REGISTER_TEST(otbImageClassificationFilter);
}
