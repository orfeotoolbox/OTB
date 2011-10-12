#
# Examples on the use of hyperspectral algorithms
#

PROJECT(HyperspectralExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(HyperspectralUnmixingExample HyperspectralUnmixing.cxx )
TARGET_LINK_LIBRARIES(HyperspectralUnmixingExample OTBCommon OTBIO OTBHyperspectral)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Hyperspctral)
SET(BASELINE_IMAGES_OTB ${OTB_DATA_ROOT}/Baseline/OTB/Images)

SET(INPUTDATA_EXAMPLES ${OTB_DATA_ROOT}/Examples)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbHyperspectralExamplesTests)

SET(TOL 0.0)

# tests#
# ------- HyperspectralUnmixingExampleTest ----------

ADD_TEST(siTvHyperspectralExampleTest ${EXE_TESTS}
     #--compare-image ${TOL} ${BASELINE}/siTvUnmixingExampleTest.txt
     #              ${TEMP}/siTvUnmixingExampleTest.txt
     HyperspectralUnmixingExample
     ${INPUTDATA_EXAMPLES}/Indian_pines_corrected.tif
     ${TEMP}/siTvHyperspectralUnmixingExampleTest.tif
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
     16
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)

ADD_EXECUTABLE(otbHyperspectralExamplesTests otbHyperspectralExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbHyperspectralExamplesTests OTBCommon OTBIO OTBHyperspectral OTBCommon OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
