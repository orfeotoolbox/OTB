project(PatentedExamples)
include_regular_expression("^.*$")


#add_executable(FuzzyConnectednessImageFilter FuzzyConnectednessImageFilter.cxx )
#target_link_libraries(FuzzyConnectednessImageFilter OTBIO OTBCommon )

#add_executable(HybridSegmentationFuzzyVoronoi HybridSegmentationFuzzyVoronoi.cxx )
#target_link_libraries(HybridSegmentationFuzzyVoronoi OTBIO OTBCommon )

#Examples using SIFT
if(OTB_USE_SIFTFAST)
add_executable(SIFTDisparityMapEstimation SIFTDisparityMapEstimation.cxx )
target_link_libraries(SIFTDisparityMapEstimation OTBFeatureExtraction OTBCommon OTBIO
${ITK_LIBRARIES})
endif()

add_executable(EstimateAffineTransformationExample EstimateAffineTransformationExample.cxx )
target_link_libraries(EstimateAffineTransformationExample OTBFeatureExtraction OTBCommon OTBIO OTBProjections
${ITK_LIBRARIES})

#Feature extraction examples based on SIFT
add_executable(SIFTExample SIFTExample.cxx)
target_link_libraries(SIFTExample OTBIO OTBCommon OTBFeatureExtraction
${ITK_LIBRARIES}  )

if(OTB_USE_SIFTFAST)

  add_executable(SIFTFastExample SIFTFastExample.cxx)
  target_link_libraries(SIFTFastExample OTBIO OTBCommon OTBFeatureExtraction ${ITK_LIBRARIES})

  add_executable(SIFTDensityExample SIFTDensityExample.cxx)
  target_link_libraries(SIFTDensityExample OTBIO OTBCommon OTBBasicFilters ${ITK_LIBRARIES})

endif()


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Patented)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbPatentedExamplesTests)
set(EXE_TESTS2 ${CXX_TEST_PATH}/otbPatentedExamplesTests2)

set(TOL 0.0)


# ------- HybridSegmentationFuzzyVoronoiTest----------

#add_test(paTeHybridSegmentationFuzzyVoronoiTest ${EXE_TESTS}
#	--compare-n-images ${TOL} 1
#	${BASELINE}/HybridSegmentationFuzzyVoronoiOutput.png
#	${TEMP}/HybridSegmentationFuzzyVoronoiOutput.png
#	HybridSegmentationFuzzyVoronoiTest
#	${INPUTDATA}/QB_Suburb.png
#	${TEMP}/HybridSegmentationFuzzyVoronoiOutput.png
#    111 38 75 20 0.5 3.0
#)

# -----EstimateAffineTransformationExampleTest---------

add_test(dmTeEstimateAffineTransformationExampleTest ${EXE_TESTS}
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
if(OTB_USE_SIFTFAST)
add_test(dmTeSIFTDisparityMapEstimationTest ${EXE_TESTS2}
  --compare-image ${TOL}
   ${BASELINE}/SIFTdeformationFieldOutput.png
   ${TEMP}/SIFTdeformationFieldOutput.png
SIFTDisparityMapEstimationTest
   ${INPUTDATA}/ROISpot5.png
   ${INPUTDATA}/ROISpot5Warped.png
   ${TEMP}/SIFTdeformationFieldOutput.png
)
endif()

if(OTB_USE_SIFTFAST)
add_test(feTeSIFTFastExampleTest ${EXE_TESTS2}
  --compare-image ${TOL}
   ${BASELINE}/ROISpot5SiftFast.png
   ${TEMP}/ROISpot5SiftFast.png
  SIFTFastExampleTest
   ${INPUTDATA}/ROISpot5.png
   ${TEMP}/ROISpot5SiftFast.png
   3
)
endif()

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbPatentedExamplesTests otbPatentedExamplesTests.cxx)
target_link_libraries(otbPatentedExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

if(OTB_USE_SIFTFAST)
add_executable(otbPatentedExamplesTests2 otbPatentedExamplesTests2.cxx)
target_link_libraries(otbPatentedExamplesTests2  OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBFeatureExtraction OTBTesting)
endif()

endif()
