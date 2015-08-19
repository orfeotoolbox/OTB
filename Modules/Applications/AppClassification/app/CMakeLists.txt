set(OTBAppClassification_LINK_LIBS
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBConversion_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBColorMap_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBMajorityVoting_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBSOM_LIBRARIES}
  ${OTBSVMLearning_LIBRARIES}
  ${OTBLearningBase_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBIndices_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBDempsterShafer_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBIOXML_LIBRARIES}
  ${OTBVectorDataManipulation_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBCommon_LIBRARIES}
)

otb_create_application(
  NAME           ComputeOGRLayersFeaturesStatistics
  SOURCES        otbComputeOGRLayersFeaturesStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SOMClassification
  SOURCES        otbSOMClassification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ImageClassifier
  SOURCES        otbImageClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TrainOGRLayersClassifier
  SOURCES        otbTrainOGRLayersClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ComputeConfusionMatrix
  SOURCES        otbComputeConfusionMatrix.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           FusionOfClassifications
  SOURCES        otbFusionOfClassifications.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ComputePolylineFeatureFromImage
  SOURCES        otbComputePolylineFeatureFromImage.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           KMeansClassification
  SOURCES        otbKMeansClassification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TrainImagesClassifier
  SOURCES        otbTrainImagesClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TrainRegression
  SOURCES        otbTrainRegression.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           PredictRegression
  SOURCES        otbPredictRegression.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           DSFuzzyModelEstimation
  SOURCES        otbDSFuzzyModelEstimation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ClassificationMapRegularization
  SOURCES        otbClassificationMapRegularization.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           OGRLayerClassifier
  SOURCES        otbOGRLayerClassifier.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ComputeImagesStatistics
  SOURCES        otbComputeImagesStatistics.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorDataDSValidation
  SOURCES        otbVectorDataDSValidation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
