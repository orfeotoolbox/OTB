project(StatisticsExamples)
include_regular_expression("^.*$")

add_executable(KdTreeBasedKMeansClustering KdTreeBasedKMeansClustering.cxx )
target_link_libraries(KdTreeBasedKMeansClustering OTBCommon ${ITK_LIBRARIES})

add_executable(ScalarImageKmeansClassifier ScalarImageKmeansClassifier.cxx )
target_link_libraries(ScalarImageKmeansClassifier OTBIO OTBCommon )

add_executable(KMeansImageClassificationExample KMeansImageClassificationExample.cxx )
target_link_libraries(KMeansImageClassificationExample OTBIO OTBLearning)

add_executable(ScalarImageKmeansModelEstimator ScalarImageKmeansModelEstimator.cxx )
target_link_libraries(ScalarImageKmeansModelEstimator  OTBIO OTBCommon )

add_executable(BayesianPluginClassifier BayesianPluginClassifier.cxx )
target_link_libraries(BayesianPluginClassifier OTBIO OTBCommon ${ITK_LIBRARIES})

add_executable(ExpectationMaximizationMixtureModelEstimator ExpectationMaximizationMixtureModelEstimator.cxx )
target_link_libraries(ExpectationMaximizationMixtureModelEstimator OTBIO OTBCommon ${ITK_LIBRARIES})

add_executable(ScalarImageMarkovRandomField1 ScalarImageMarkovRandomField1.cxx )
target_link_libraries(ScalarImageMarkovRandomField1 OTBIO OTBCommon )

add_executable(SOMImageClassificationExample SOMImageClassificationExample.cxx )
target_link_libraries(SOMImageClassificationExample OTBIO OTBLearning)

add_executable(SupervisedImageClassificationExample SupervisedImageClassificationExample.cxx )
target_link_libraries(SupervisedImageClassificationExample OTBIO OTBLearning)

add_executable(MajorityVotingFusionOfClassificationMapsExample MajorityVotingFusionOfClassificationMapsExample.cxx )
target_link_libraries(MajorityVotingFusionOfClassificationMapsExample OTBIO)

add_executable(DempsterShaferFusionOfClassificationMapsExample DempsterShaferFusionOfClassificationMapsExample.cxx )
target_link_libraries(DempsterShaferFusionOfClassificationMapsExample OTBIO)

add_executable(ClassificationMapRegularizationExample ClassificationMapRegularizationExample.cxx )
target_link_libraries(ClassificationMapRegularizationExample OTBIO OTBCommon)

#add_executable( BayesianClassifierInitializer BayesianClassifierInitializer.cxx )
#target_link_libraries(BayesianClassifierInitializer ${ITK_LIBRARIES})

#add_executable( BayesianClassifier BayesianClassifier.cxx )
#target_link_libraries(BayesianClassifier ${ITK_LIBRARIES})

#add_executable(EuclideanDistance EuclideanDistance.cxx )
#target_link_libraries(EuclideanDistance)


#add_executable(GaussianDensityFunction GaussianDensityFunction.cxx )
#target_link_libraries(GaussianDensityFunction)

#add_executable(Histogram Histogram.cxx )
#target_link_libraries(Histogram ${ITK_LIBRARIES})

#add_executable(ImageToListAdaptor ImageToListAdaptor.cxx )
#target_link_libraries(ImageToListAdaptor)

#add_executable(KdTree KdTree.cxx )
#target_link_libraries(KdTree)


#add_executable(ListSample ListSample.cxx )
#target_link_libraries(ListSample)

#add_executable(ListSampleToHistogramFilter ListSampleToHistogramFilter.cxx )
#target_link_libraries(ListSampleToHistogramFilter ${ITK_LIBRARIES})

#add_executable(ListSampleToHistogramGenerator ListSampleToHistogramGenerator.cxx )
#target_link_libraries(ListSampleToHistogramGenerator ${ITK_LIBRARIES})

#add_executable(MaximumDecisionRule MaximumDecisionRule.cxx )
#target_link_libraries(MaximumDecisionRule)

#add_executable(MaximumRatioDecisionRule MaximumRatioDecisionRule.cxx )
#target_link_libraries(MaximumRatioDecisionRule)

#add_executable(MembershipSample MembershipSample.cxx )
#target_link_libraries(MembershipSample)

#add_executable(MinimumDecisionRule MinimumDecisionRule.cxx )
#target_link_libraries(MinimumDecisionRule)

#add_executable(NeighborhoodSampler NeighborhoodSampler.cxx )
#target_link_libraries(NeighborhoodSampler)

#add_executable(NormalVariateGenerator NormalVariateGenerator.cxx )
#target_link_libraries(NormalVariateGenerator ${ITK_LIBRARIES})

#add_executable(PointSetToListAdaptor PointSetToListAdaptor.cxx )
#target_link_libraries(PointSetToListAdaptor)

#add_executable(NormalVariateGenerator NormalVariateGenerator.cxx )
#target_link_libraries(NormalVariateGenerator)

#add_executable(SampleStatistics SampleStatistics.cxx )
#target_link_libraries(SampleStatistics)

#add_executable(SampleToHistogramProjectionFilter SampleToHistogramProjectionFilter.cxx )
#target_link_libraries(SampleToHistogramProjectionFilter
#                                                        ${ITK_LIBRARIES})

#add_executable(SampleSorting SampleSorting.cxx )
#target_link_libraries(SampleSorting)

#add_executable(WeightedSampleStatistics WeightedSampleStatistics.cxx )
#target_link_libraries(WeightedSampleStatistics)

#add_executable(ImageHistogram1 ImageHistogram1.cxx )
#target_link_libraries(ImageHistogram1 ${ITK_LIBRARIES} )

#add_executable(ImageHistogram2 ImageHistogram2.cxx )
#target_link_libraries(ImageHistogram2 ${ITK_LIBRARIES} )

#add_executable(ImageHistogram3 ImageHistogram3.cxx )
#target_link_libraries(ImageHistogram3 ${ITK_LIBRARIES} )

#add_executable(ImageHistogram4 ImageHistogram4.cxx )
#target_link_libraries(ImageHistogram4 ${ITK_LIBRARIES} )

#add_executable(ImageEntropy1 ImageEntropy1.cxx )
#target_link_libraries(ImageEntropy1 ${ITK_LIBRARIES})




if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Classification)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbClassificationExamplesTests)

set(TOL 0.0)


# ------- ScalarImageMarkovRandomField1Test----------

add_test(clTeScalarImageMarkovRandomField1Test ${EXE_TESTS}
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

add_test(clTeScalarImageKmeansClassifierTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/QB_Suburb_labelled.png
        ${TEMP}/QB_Suburb_labelled.png
        ScalarImageKmeansClassifierTest
        ${INPUTDATA}/QB_Suburb.png
        ${TEMP}/QB_Suburb_labelled.png
    0 4 79.5097 138.136 213.846 25.9395
)

add_test(clTeNeighborhoodMajorityVotingImageFilterTest ${EXE_TESTS}
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

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbClassificationExamplesTests otbClassificationExamplesTests.cxx)
target_link_libraries(otbClassificationExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

endif()
