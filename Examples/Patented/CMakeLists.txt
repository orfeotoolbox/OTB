PROJECT(PatentedExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")


#ADD_EXECUTABLE(FuzzyConnectednessImageFilter FuzzyConnectednessImageFilter.cxx )
#TARGET_LINK_LIBRARIES(FuzzyConnectednessImageFilter OTBIO OTBCommon )

#ADD_EXECUTABLE(HybridSegmentationFuzzyVoronoi HybridSegmentationFuzzyVoronoi.cxx )
#TARGET_LINK_LIBRARIES(HybridSegmentationFuzzyVoronoi OTBIO OTBCommon )

#Examples using SIFT
IF(OTB_USE_SIFTFAST)
ADD_EXECUTABLE(SIFTDisparityMapEstimation SIFTDisparityMapEstimation.cxx )
TARGET_LINK_LIBRARIES(SIFTDisparityMapEstimation OTBFeatureExtraction OTBCommon OTBIO
${ITK_LIBRARIES})
ENDIF(OTB_USE_SIFTFAST)

ADD_EXECUTABLE(EstimateAffineTransformationExample EstimateAffineTransformationExample.cxx )
TARGET_LINK_LIBRARIES(EstimateAffineTransformationExample OTBFeatureExtraction OTBCommon OTBIO OTBProjections
${ITK_LIBRARIES})

#Feature extraction examples based on SIFT
ADD_EXECUTABLE(SIFTExample SIFTExample.cxx)
TARGET_LINK_LIBRARIES(SIFTExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

IF(OTB_USE_SIFTFAST)

  ADD_EXECUTABLE(SIFTFastExample SIFTFastExample.cxx)
  TARGET_LINK_LIBRARIES(SIFTFastExample OTBIO OTBCommon OTBFeatureExtraction ${ITK_LIBRARIES})

  ADD_EXECUTABLE(SIFTDensityExample SIFTDensityExample.cxx)
  TARGET_LINK_LIBRARIES(SIFTDensityExample OTBIO OTBCommon OTBBasicFilters ${ITK_LIBRARIES})

ENDIF(OTB_USE_SIFTFAST)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Patented)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbPatentedExamplesTests)
SET(EXE_TESTS2 ${CXX_TEST_PATH}/otbPatentedExamplesTests2)

SET(TOL 0.0)


# ------- HybridSegmentationFuzzyVoronoiTest----------

#ADD_TEST(paTeHybridSegmentationFuzzyVoronoiTest ${EXE_TESTS}
#	--compare-n-images ${TOL} 1
#	${BASELINE}/HybridSegmentationFuzzyVoronoiOutput.png
#	${TEMP}/HybridSegmentationFuzzyVoronoiOutput.png
#	HybridSegmentationFuzzyVoronoiTest
#	${INPUTDATA}/QB_Suburb.png
#	${TEMP}/HybridSegmentationFuzzyVoronoiOutput.png
#    111 38 75 20 0.5 3.0
#)

# -----EstimateAffineTransformationExampleTest---------

ADD_TEST(dmTeEstimateAffineTransformationExampleTest ${EXE_TESTS}
   --compare-image ${TOL}
         ${BASELINE}/EstimateAffineTransformationOutput.png
         ${TEMP}/AffineTransformationOutput.png
         EstimateAffineTransformationExampleTest
   ${INPUTDATA}/QB_Suburb.png
         ${INPUTDATA}/QB_SuburbR10X13Y17.png
         ${TEMP}/AffineTransformationOutput.png
         ${TEMP}/AffineTransformationTxtOutput.txt
   2 3 0 0 0.5 1
)

# -----SIFTDisparityMapEstimationTest---------
IF(OTB_USE_SIFTFAST)
ADD_TEST(dmTeSIFTDisparityMapEstimationTest ${EXE_TESTS2}
  --compare-image ${TOL}
   ${BASELINE}/SIFTdeformationFieldOutput.png
   ${TEMP}/SIFTdeformationFieldOutput.png
SIFTDisparityMapEstimationTest
   ${INPUTDATA}/ROISpot5.png
   ${INPUTDATA}/ROISpot5Warped.png
   ${TEMP}/SIFTdeformationFieldOutput.png
)
ENDIF(OTB_USE_SIFTFAST)

ADD_TEST(feTeScaleInvariantFeatureImageFilterTest ${EXE_TESTS}
        --compare-n-images ${TOL} 4
        ${BASELINE}/ScaleInvariantFeatureImageFilterOutput1.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutput1.png
  ${BASELINE}/ScaleInvariantFeatureImageFilterOutput2.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutput2.png
  ${BASELINE}/ScaleInvariantFeatureImageFilterOutputKeys1.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutputKeys1.png
  ${BASELINE}/ScaleInvariantFeatureImageFilterOutputKeys2.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutputKeys2.png

        ScaleInvariantFeatureImageFilterTest

        ${INPUTDATA}/Scene.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutput1.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutput2.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutputKeys1.png
        ${TEMP}/ScaleInvariantFeatureImageFilterOutputKeys2.png
  1 10 0.5 0
)

IF(OTB_USE_SIFTFAST)
ADD_TEST(feTeSIFTFastExampleTest ${EXE_TESTS2}
  --compare-image ${TOL}
   ${BASELINE}/ROISpot5SiftFast.png
   ${TEMP}/ROISpot5SiftFast.png
  SIFTFastExampleTest
   ${INPUTDATA}/ROISpot5.png
   ${TEMP}/ROISpot5SiftFast.png
   3
)
ENDIF(OTB_USE_SIFTFAST)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbPatentedExamplesTests otbPatentedExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbPatentedExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

IF(OTB_USE_SIFTFAST)
ADD_EXECUTABLE(otbPatentedExamplesTests2 otbPatentedExamplesTests2.cxx)
TARGET_LINK_LIBRARIES(otbPatentedExamplesTests2  OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBFeatureExtraction OTBTesting)
ENDIF(OTB_USE_SIFTFAST)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
