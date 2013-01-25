PROJECT(StatisticsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(KdTreeBasedKMeansClustering KdTreeBasedKMeansClustering.cxx )
TARGET_LINK_LIBRARIES(KdTreeBasedKMeansClustering OTBCommon ITKStatistics)

ADD_EXECUTABLE(ScalarImageKmeansClassifier ScalarImageKmeansClassifier.cxx )
TARGET_LINK_LIBRARIES(ScalarImageKmeansClassifier OTBIO OTBCommon )

ADD_EXECUTABLE(KMeansImageClassificationExample KMeansImageClassificationExample.cxx )
TARGET_LINK_LIBRARIES(KMeansImageClassificationExample OTBIO OTBLearning)

ADD_EXECUTABLE(ScalarImageKmeansModelEstimator ScalarImageKmeansModelEstimator.cxx )
TARGET_LINK_LIBRARIES(ScalarImageKmeansModelEstimator  OTBIO OTBCommon )

ADD_EXECUTABLE(BayesianPluginClassifier BayesianPluginClassifier.cxx )
TARGET_LINK_LIBRARIES(BayesianPluginClassifier OTBIO OTBCommon ITKStatistics)

ADD_EXECUTABLE(ExpectationMaximizationMixtureModelEstimator ExpectationMaximizationMixtureModelEstimator.cxx )
TARGET_LINK_LIBRARIES(ExpectationMaximizationMixtureModelEstimator OTBIO OTBCommon ITKStatistics)

ADD_EXECUTABLE(ScalarImageMarkovRandomField1 ScalarImageMarkovRandomField1.cxx )
TARGET_LINK_LIBRARIES(ScalarImageMarkovRandomField1 OTBIO OTBCommon )

ADD_EXECUTABLE(SOMImageClassificationExample SOMImageClassificationExample.cxx )
TARGET_LINK_LIBRARIES(SOMImageClassificationExample OTBIO OTBLearning)

ADD_EXECUTABLE(SVMImageClassificationExample2 SVMImageClassifierExample.cxx )
TARGET_LINK_LIBRARIES(SVMImageClassificationExample2 OTBIO OTBLearning)

ADD_EXECUTABLE(ClassificationMapRegularizationExample ClassificationMapRegularizationExample.cxx )
TARGET_LINK_LIBRARIES(ClassificationMapRegularizationExample OTBIO OTBCommon)

#ADD_EXECUTABLE( BayesianClassifierInitializer BayesianClassifierInitializer.cxx )
#TARGET_LINK_LIBRARIES(BayesianClassifierInitializer ITKStatistics)

#ADD_EXECUTABLE( BayesianClassifier BayesianClassifier.cxx )
#TARGET_LINK_LIBRARIES(BayesianClassifier ITKStatistics)

#ADD_EXECUTABLE(EuclideanDistance EuclideanDistance.cxx )
#TARGET_LINK_LIBRARIES(EuclideanDistance)


#ADD_EXECUTABLE(GaussianDensityFunction GaussianDensityFunction.cxx )
#TARGET_LINK_LIBRARIES(GaussianDensityFunction)

#ADD_EXECUTABLE(Histogram Histogram.cxx )
#TARGET_LINK_LIBRARIES(Histogram ITKStatistics)

#ADD_EXECUTABLE(ImageToListAdaptor ImageToListAdaptor.cxx )
#TARGET_LINK_LIBRARIES(ImageToListAdaptor)

#ADD_EXECUTABLE(KdTree KdTree.cxx )
#TARGET_LINK_LIBRARIES(KdTree)


#ADD_EXECUTABLE(ListSample ListSample.cxx )
#TARGET_LINK_LIBRARIES(ListSample)

#ADD_EXECUTABLE(ListSampleToHistogramFilter ListSampleToHistogramFilter.cxx )
#TARGET_LINK_LIBRARIES(ListSampleToHistogramFilter ITKStatistics)

#ADD_EXECUTABLE(ListSampleToHistogramGenerator ListSampleToHistogramGenerator.cxx )
#TARGET_LINK_LIBRARIES(ListSampleToHistogramGenerator ITKStatistics)

#ADD_EXECUTABLE(MaximumDecisionRule MaximumDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MaximumDecisionRule)

#ADD_EXECUTABLE(MaximumRatioDecisionRule MaximumRatioDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MaximumRatioDecisionRule)

#ADD_EXECUTABLE(MembershipSample MembershipSample.cxx )
#TARGET_LINK_LIBRARIES(MembershipSample)

#ADD_EXECUTABLE(MinimumDecisionRule MinimumDecisionRule.cxx )
#TARGET_LINK_LIBRARIES(MinimumDecisionRule)

#ADD_EXECUTABLE(NeighborhoodSampler NeighborhoodSampler.cxx )
#TARGET_LINK_LIBRARIES(NeighborhoodSampler)

#ADD_EXECUTABLE(NormalVariateGenerator NormalVariateGenerator.cxx )
#TARGET_LINK_LIBRARIES(NormalVariateGenerator ITKStatistics)

#ADD_EXECUTABLE(PointSetToListAdaptor PointSetToListAdaptor.cxx )
#TARGET_LINK_LIBRARIES(PointSetToListAdaptor)

#ADD_EXECUTABLE(NormalVariateGenerator NormalVariateGenerator.cxx )
#TARGET_LINK_LIBRARIES(NormalVariateGenerator)

#ADD_EXECUTABLE(SampleStatistics SampleStatistics.cxx )
#TARGET_LINK_LIBRARIES(SampleStatistics)

#ADD_EXECUTABLE(SampleToHistogramProjectionFilter SampleToHistogramProjectionFilter.cxx )
#TARGET_LINK_LIBRARIES(SampleToHistogramProjectionFilter
#                                                        ITKStatistics)

#ADD_EXECUTABLE(SampleSorting SampleSorting.cxx )
#TARGET_LINK_LIBRARIES(SampleSorting)

#ADD_EXECUTABLE(WeightedSampleStatistics WeightedSampleStatistics.cxx )
#TARGET_LINK_LIBRARIES(WeightedSampleStatistics)

#ADD_EXECUTABLE(ImageHistogram1 ImageHistogram1.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram1 ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram2 ImageHistogram2.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram2 ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram3 ImageHistogram3.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram3 ITKStatistics )

#ADD_EXECUTABLE(ImageHistogram4 ImageHistogram4.cxx )
#TARGET_LINK_LIBRARIES(ImageHistogram4 ITKStatistics )

#ADD_EXECUTABLE(ImageEntropy1 ImageEntropy1.cxx )
#TARGET_LINK_LIBRARIES(ImageEntropy1 ITKStatistics)




IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Classification)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbClassificationExamplesTests)

SET(TOL 0.0)


# ------- ScalarImageMarkovRandomField1Test----------

ADD_TEST(clTeScalarImageMarkovRandomField1Test ${EXE_TESTS}
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

ADD_TEST(clTeScalarImageKmeansClassifierTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/QB_Suburb_labelled.png
        ${TEMP}/QB_Suburb_labelled.png
        ScalarImageKmeansClassifierTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/QB_Suburb_labelled.png
    0 4 79.5097 138.136 213.846 25.9395
)

ADD_TEST(clTeNeighborhoodMajorityVotingImageFilterTest ${EXE_TESTS}
       --compare-n-images ${NOTOL}
       ${OTB_DATA_ROOT}/Baseline/OTB/Images/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
       ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7_EX.tif
      ClassificationMapRegularizationExampleTest
       ${OTB_DATA_ROOT}/Input/Classification/QB_1_ortho_4Cls_N_Classified_OTB.tif
       ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7_EX.tif
       false #KeepOriginalLabelBool
       2 #xRadius
       5 #yRadius
       10 #LabelForNoDataPixels
       7 #LabelForUndecidedPixels
 )

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbClassificationExamplesTests otbClassificationExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbClassificationExamplesTests  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
