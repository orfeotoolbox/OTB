OTB_CREATE_APPLICATION(NAME           ComputeImagesStatistics
                       SOURCES        otbComputeImagesStatistics.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ImageSVMClassifier
                       SOURCES        otbImageSVMClassifier.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning)

OTB_CREATE_APPLICATION(NAME           KMeansClassification
                       SOURCES        otbKMeansClassification.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning)

OTB_CREATE_APPLICATION(NAME           TrainSVMImagesClassifier
                       SOURCES        otbTrainSVMImagesClassifier.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning)

OTB_CREATE_APPLICATION(NAME           ValidateSVMImagesClassifier
                       SOURCES        otbValidateSVMImagesClassifier.cxx
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

IF(OTB_USE_OPENCV)
	OTB_CREATE_APPLICATION(NAME           TrainMachineLearningImagesClassifier
	                       SOURCES        otbTrainMachineLearningImagesClassifier.cxx otbTrainSVM.cxx otbTrainLibSVM.cxx otbTrainBoost.cxx 
	                       				  otbTrainDecisionTree.cxx otbTrainGradientBoostedTree.cxx otbTrainNeuralNetwork.cxx otbTrainNormalBayes.cxx
	                       				  otbTrainRandomForests.cxx
	                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning;OTBMachineLearning)

	OTB_CREATE_APPLICATION(NAME           ImageClassifier
                       	   SOURCES        otbImageClassifier.cxx
                           LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning;OTBMachineLearning)

	OTB_CREATE_APPLICATION(NAME           ValidateImagesClassifier
                           SOURCES        otbValidateImagesClassifier.cxx
                           LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction;OTBLearning;OTBMachineLearning)
ENDIF()
