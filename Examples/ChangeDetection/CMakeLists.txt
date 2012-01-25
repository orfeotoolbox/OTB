PROJECT(ChangeDetectionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
        KullbackLeiblerDistanceChDet.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)



ADD_EXECUTABLE(ChangeDetectionFrameworkExample ChangeDetectionFrameworkExample.cxx )
TARGET_LINK_LIBRARIES(ChangeDetectionFrameworkExample OTBCommon  OTBIO)

ADD_EXECUTABLE(DiffChDet DiffChDet.cxx)
TARGET_LINK_LIBRARIES(DiffChDet OTBIO OTBCommon  )

ADD_EXECUTABLE(RatioChDet RatioChDet.cxx)
TARGET_LINK_LIBRARIES(RatioChDet OTBIO OTBCommon  )

ADD_EXECUTABLE(CorrelChDet CorrelChDet.cxx)
TARGET_LINK_LIBRARIES(CorrelChDet OTBIO OTBCommon  )

ADD_EXECUTABLE(LHMIChDet LHMIChDet.cxx)
TARGET_LINK_LIBRARIES(LHMIChDet OTBIO OTBCommon  ITKStatistics)

ADD_EXECUTABLE(JHMIChDet JHMIChDet.cxx)
TARGET_LINK_LIBRARIES(JHMIChDet OTBIO OTBCommon  ITKStatistics)

ADD_EXECUTABLE(KullbackLeiblerDistanceChDet KullbackLeiblerDistanceChDet.cxx)
TARGET_LINK_LIBRARIES(KullbackLeiblerDistanceChDet OTBIO OTBCommon  ITKStatistics)

ADD_EXECUTABLE(KullbackLeiblerProfileChDet KullbackLeiblerProfileChDet.cxx)
TARGET_LINK_LIBRARIES(KullbackLeiblerProfileChDet OTBIO OTBCommon  ITKStatistics)

ADD_EXECUTABLE(KullbackLeiblerSupervizedDistanceChDet KullbackLeiblerSupervizedDistanceChDet.cxx)
TARGET_LINK_LIBRARIES(KullbackLeiblerSupervizedDistanceChDet OTBIO OTBCommon  ITKStatistics)

ADD_EXECUTABLE(MultivariateAlterationDetector MultivariateAlterationDetector.cxx)
TARGET_LINK_LIBRARIES(MultivariateAlterationDetector OTBIO OTBCommon  ITKStatistics)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
        otbChangeDetectionExamplesTests.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/ChangeDetection)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbChangeDetectionExamplesTests)

SET(TOL 0.0)
SET(PNGEPSILON 2)

# ------- CorrelChDetTest----------

ADD_TEST(cdTeCorrelChDetTest ${EXE_TESTS}
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

ADD_TEST(cdTeDiffChDetTest ${EXE_TESTS}
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

ADD_TEST(cdTeRatioChDetTest ${EXE_TESTS}
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

ADD_TEST(cdTeKullbackLeiblerDistanceChDetTest ${EXE_TESTS}
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

ADD_TEST(cdTeKullbackLeiblerProfileChDetTest ${EXE_TESTS}
        --compare-n-images ${TOL} 1
        ${BASELINE}/KullbackLeiblerProfileChDetTest.png
        ${TEMP}/KullbackLeiblerProfileChDetTest.png
        KullbackLeiblerProfileChDetTest
        ${INPUTDATA}/GomaAvant.png
        ${INPUTDATA}/GomaApres.png
        ${TEMP}/KullbackLeiblerProfileChDetTest.png
        5 51 1 12 24
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbChangeDetectionExamplesTests otbChangeDetectionExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbChangeDetectionExamplesTests  ITKAlgorithms ITKStatistics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

