OTB_CREATE_APPLICATION(NAME           ComputeImagesStatistics
                       SOURCES        otbComputeImagesStatistics.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           KMeansClassification
                       SOURCES        otbKMeansClassification.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning)

OTB_CREATE_APPLICATION(NAME           SOMClassification
                       SOURCES        otbSOMClassification.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning)

OTB_CREATE_APPLICATION(NAME           FusionOfClassifications
                       SOURCES        otbFusionOfClassifications.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning)

OTB_CREATE_APPLICATION(NAME           ComputeConfusionMatrix
                       SOURCES        otbComputeConfusionMatrix.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ClassificationMapRegularization
                       SOURCES        otbClassificationMapRegularization.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

set( TrainImagesClassifier_SRC otbTrainImagesClassifier.cxx otbTrainLibSVM.cxx )
if(OTB_USE_OPENCV)
  # This library is used available only when OpenCV is activated.
  # Use a cmake variable as proxy, which is empty when OpenCV is deactivated
  set( OTBMachineLearning_LIBRARY OTBMachineLearning )
  
  # Add source files with OpenCV dependency
  set( TrainImagesClassifier_SRC ${TrainImagesClassifier_SRC} 
         otbTrainSVM.cxx  otbTrainBoost.cxx 
         otbTrainDecisionTree.cxx otbTrainGradientBoostedTree.cxx
         otbTrainNeuralNetwork.cxx otbTrainNormalBayes.cxx
         otbTrainRandomForests.cxx otbTrainKNN.cxx )
endif()

OTB_CREATE_APPLICATION(NAME         TrainImagesClassifier
                       SOURCES      ${TrainImagesClassifier_SRC}
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning;${OTBMachineLearning_LIBRARY})

OTB_CREATE_APPLICATION(NAME         ImageClassifier
                       SOURCES      otbImageClassifier.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning;${OTBMachineLearning_LIBRARY})


 