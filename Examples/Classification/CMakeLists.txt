PROJECT(StatisticsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(KdTreeBasedKMeansClustering KdTreeBasedKMeansClustering.cxx )
TARGET_LINK_LIBRARIES(KdTreeBasedKMeansClustering OTBCommon ITKCommon ITKStatistics)

ADD_EXECUTABLE(ScalarImageKmeansClassifier ScalarImageKmeansClassifier.cxx )
TARGET_LINK_LIBRARIES(ScalarImageKmeansClassifier OTBIO OTBCommon ITKIO ITKCommon )

ADD_EXECUTABLE(ScalarImageKmeansModelEstimator ScalarImageKmeansModelEstimator.cxx )
TARGET_LINK_LIBRARIES(ScalarImageKmeansModelEstimator  OTBIO OTBCommon ITKIO ITKCommon )

ADD_EXECUTABLE(BayesianPluginClassifier BayesianPluginClassifier.cxx )
TARGET_LINK_LIBRARIES(BayesianPluginClassifier OTBIO OTBCommon ITKCommon ITKStatistics)

ADD_EXECUTABLE(ExpectationMaximizationMixtureModelEstimator ExpectationMaximizationMixtureModelEstimator.cxx )
TARGET_LINK_LIBRARIES(ExpectationMaximizationMixtureModelEstimator OTBIO OTBCommon ITKCommon ITKStatistics)

ADD_EXECUTABLE(ScalarImageMarkovRandomField1 ScalarImageMarkovRandomField1.cxx )
TARGET_LINK_LIBRARIES(ScalarImageMarkovRandomField1 OTBIO OTBCommon ITKIO ITKCommon )


#ADD_EXECUTABLE( BayesianClassifierInitializer BayesianClassifierInitializer.cxx )
#TARGET_LINK_LIBRARIES(BayesianClassifierInitializer ITKCommon ITKStatistics ITKIO)

#ADD_EXECUTABLE( BayesianClassifier BayesianClassifier.cxx )
#TARGET_LINK_LIBRARIES(BayesianClassifier ITKCommon ITKStatistics ITKIO)

#ADD_EXECUTABLE(EuclideanDistance EuclideanDistance.cxx )
#TARGET_LINK_LIBRARIES(EuclideanDistance ITKCommon)


#ADD_EXECUTABLE(GaussianDensityFunction GaussianDensityFunction.cxx )
#TARGET_LINK_LIBRARIES(GaussianDensityFunction ITKCommon)

#ADD_EXECUTABLE(Histogram Histogram.cxx )
#TARGET_LINK_LIBRARIES(Histogram ITKCommon ITKStatistics)

#ADD_EXECUTABLE(ImageToListAdaptor ImageToListAdaptor.cxx )
#TARGET_LINK_LIBRARIES(ImageToListAdaptor ITKCommon)

#ADD_EXECUTABLE(KdTree KdTree.cxx )
#TARGET_LINK_LIBRARIES(KdTree ITKCommon)


#ADD_EXECUTABLE(ListSample ListSample.cxx )
#TARGET_LINK_LIBRARIES(ListSample ITKCommon)

#ADD_EXECUTABLE(ListSampleToHistogramFilter ListSampleToHistogramFilter.cxx )
#TARGET_LINK_LIBRARIES(ListSampleToHistogramFilter ITKCommon ITKStatistics)

#ADD_EXECUTABLE(ListSampleToHistogramGenerator ListSampleToHistogramGenerator.cxx )
#TARGET_LINK_LIBRARIES(ListSampleToHistogramGenerator ITKCommon ITKStatistics)

#ADD_EXECUTABLE(MaximumDecisionRule MaximumDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MaximumDecisionRule ITKCommon)

#ADD_EXECUTABLE(MaximumRatioDecisionRule MaximumRatioDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MaximumRatioDecisionRule ITKCommon)

#ADD_EXECUTABLE(MembershipSample MembershipSample.cxx )
#TARGET_LINK_LIBRARIES(MembershipSample ITKCommon)

#ADD_EXECUTABLE(MinimumDecisionRule MinimumDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MinimumDecisionRule ITKCommon)

#ADD_EXECUTABLE(NeighborhoodSampler NeighborhoodSampler.cxx )
#TARGET_LINK_LIBRARIES(NeighborhoodSampler ITKCommon)

#ADD_EXECUTABLE(NormalVariateGenerator NormalVariateGenerator.cxx )
#TARGET_LINK_LIBRARIES(NormalVariateGenerator ITKCommon ITKStatistics)

#ADD_EXECUTABLE(PointSetToListAdaptor PointSetToListAdaptor.cxx )
#TARGET_LINK_LIBRARIES(PointSetToListAdaptor ITKCommon)

#ADD_EXECUTABLE(NormalVariateGenerator NormalVariateGenerator.cxx )
#TARGET_LINK_LIBRARIES(NormalVariateGenerator ITKCommon)

#ADD_EXECUTABLE(SampleStatistics SampleStatistics.cxx )
#TARGET_LINK_LIBRARIES(SampleStatistics ITKCommon)

#ADD_EXECUTABLE(SampleToHistogramProjectionFilter SampleToHistogramProjectionFilter.cxx )
#TARGET_LINK_LIBRARIES(SampleToHistogramProjectionFilter ITKCommon
#                                                        ITKStatistics)

#ADD_EXECUTABLE(SampleSorting SampleSorting.cxx )
#TARGET_LINK_LIBRARIES(SampleSorting ITKCommon)

#ADD_EXECUTABLE(WeightedSampleStatistics WeightedSampleStatistics.cxx )
#TARGET_LINK_LIBRARIES(WeightedSampleStatistics ITKCommon)

#ADD_EXECUTABLE(ImageHistogram1 ImageHistogram1.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram1 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram2 ImageHistogram2.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram2 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram3 ImageHistogram3.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram3 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram4 ImageHistogram4.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram4 ITKIO ITKCommon ITKStatistics )

#ADD_EXECUTABLE(ImageEntropy1 ImageEntropy1.cxx )
#TARGET_LINK_LIBRARIES(ImageEntropy1 ITKIO ITKCommon ITKStatistics)




IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Classification)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbClassificationExamplesTests)

SET(TOL 0.0)


# ------- ScalarImageMarkovRandomField1Test----------

ADD_TEST(ScalarImageMarkovRandomField1Test ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/ScalarImageMarkovRandomField1Output.png
	${TEMP}/ScalarImageMarkovRandomField1Output.png
	ScalarImageMarkovRandomField1Test
	${INPUTDATA}/QB_Suburb.png
	${INPUTDATA}/QB_Suburb_labelled.png
	${TEMP}/ScalarImageMarkovRandomField1Output.png
    50 3 4 79.5097 138.136 213.846 25.9395
)

# ------- ScalarImageKmeansClassifierTest----------

ADD_TEST(ScalarImageKmeansClassifierTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/QB_Suburb_labelled.png
	${TEMP}/QB_Suburb_labelled.png
	ScalarImageKmeansClassifierTest
	${INPUTDATA}/QB_Suburb.png
	${TEMP}/QB_Suburb_labelled.png
    0 4 79.5097 138.136 213.846 25.9395
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbClassificationExamplesTests otbClassificationExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbClassificationExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
