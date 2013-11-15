project(ChangeDetectionExamples)
include_regular_expression("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(
        KullbackLeiblerDistanceChDet.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
endif(CMAKE_COMPILER_IS_GNUCXX)



add_executable(ChangeDetectionFrameworkExample ChangeDetectionFrameworkExample.cxx )
target_link_libraries(ChangeDetectionFrameworkExample OTBCommon  OTBIO)

add_executable(DiffChDet DiffChDet.cxx)
target_link_libraries(DiffChDet OTBIO OTBCommon  )

add_executable(RatioChDet RatioChDet.cxx)
target_link_libraries(RatioChDet OTBIO OTBCommon  )

add_executable(CorrelChDet CorrelChDet.cxx)
target_link_libraries(CorrelChDet OTBIO OTBCommon  )

add_executable(LHMIChDet LHMIChDet.cxx)
target_link_libraries(LHMIChDet OTBIO OTBCommon  ${ITK_LIBRARIES})

add_executable(JHMIChDet JHMIChDet.cxx)
target_link_libraries(JHMIChDet OTBIO OTBCommon  ${ITK_LIBRARIES})

add_executable(KullbackLeiblerDistanceChDet KullbackLeiblerDistanceChDet.cxx)
target_link_libraries(KullbackLeiblerDistanceChDet OTBIO OTBCommon  ${ITK_LIBRARIES})

add_executable(KullbackLeiblerProfileChDet KullbackLeiblerProfileChDet.cxx)
target_link_libraries(KullbackLeiblerProfileChDet OTBIO OTBCommon  ${ITK_LIBRARIES})

add_executable(KullbackLeiblerSupervizedDistanceChDet KullbackLeiblerSupervizedDistanceChDet.cxx)
target_link_libraries(KullbackLeiblerSupervizedDistanceChDet OTBIO OTBCommon  ${ITK_LIBRARIES})

add_executable(MultivariateAlterationDetector MultivariateAlterationDetector.cxx)
target_link_libraries(MultivariateAlterationDetector OTBIO OTBCommon  ITKStatistics)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(
        otbChangeDetectionExamplesTests.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
endif(CMAKE_COMPILER_IS_GNUCXX)

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/ChangeDetection)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbChangeDetectionExamplesTests)

set(TOL 0.0)
set(PNGEPSILON 2)

# ------- CorrelChDetTest----------

add_test(cdTeCorrelChDetTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/CorrChDet.tif
        ${TEMP}/CorrChDet.tif
        CorrelChDetTest
        ${INPUTDATA}/ERSBefore.png
        ${INPUTDATA}/ERSAfter.png
        ${TEMP}/CorrChDet.tif
    15
)

# ------- DiffChDetTest----------

add_test(cdTeDiffChDetTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/DiffChDet.tif
        ${TEMP}/DiffChDet.tif
        DiffChDetTest
        ${INPUTDATA}/SpotBefore.png
        ${INPUTDATA}/SpotAfter.png
        ${TEMP}/DiffChDet.tif
    3
)

# ------- RatioChDetTest----------

add_test(cdTeRatioChDetTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/RatioChDet.tif
        ${TEMP}/RatioChDet.tif
        RatioChDetTest
        ${INPUTDATA}/GomaAvant.png
        ${INPUTDATA}/GomaApres.png
        ${TEMP}/RatioChDet.tif
    3
)

# ------- KullbackLeiblerDistanceChDetTest----------

add_test(cdTeKullbackLeiblerDistanceChDetTest ${EXE_TESTS}
        --compare-n-images ${PNGEPSILON} 1
        ${BASELINE}/KullbackLeiblerDistanceChDetTest.png
        ${TEMP}/KullbackLeiblerDistanceChDetTest.png
        KullbackLeiblerDistanceChDetTest
        ${INPUTDATA}/GomaAvant.png
        ${INPUTDATA}/GomaApres.png
        ${TEMP}/KullbackLeiblerDistanceChDetTest.png
        35
)

# ------- KullbackLeiblerProfileChDetTest----------

add_test(cdTeKullbackLeiblerProfileChDetTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/KullbackLeiblerProfileChDetTest.png
        ${TEMP}/KullbackLeiblerProfileChDetTest.png
        KullbackLeiblerProfileChDetTest
        ${INPUTDATA}/GomaAvant.png
        ${INPUTDATA}/GomaApres.png
        ${TEMP}/KullbackLeiblerProfileChDetTest.png
        5 51 1 12 24
)

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbChangeDetectionExamplesTests otbChangeDetectionExamplesTests.cxx)
target_link_libraries(otbChangeDetectionExamplesTests   ${ITK_LIBRARIES} OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

