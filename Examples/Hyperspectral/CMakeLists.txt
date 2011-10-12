#
# Examples on the use of hyperspectral algorithms
#

PROJECT(HyperspectralExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(HyperspectralUnmixingExample HyperspectralUnmixingExample.cxx )
TARGET_LINK_LIBRARIES(HyperspectralUnmixingExample OTBCommon OTBIO OTBHyperspectral)


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Hyperspectral)
SET(BASELINE_IMAGES_OTB ${OTB_DATA_ROOT}/Baseline/OTB/Images)

SET(INPUTDATA_EXAMPLES ${OTB_DATA_ROOT}/Examples)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbHyperspectralExamplesTests)

SET(TOL 0.0)

# tests#
# ------- HyperspectralUnmixingExampleTest ----------
IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(siTvHyperspectralExampleTest ${EXE_TESTS}
     --compare-n-images ${TOL} 3
     ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand1.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
     ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand2.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
     ${BASELINE}/siTvHyperspectralUnmixingExampleTestBand3.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
     HyperspectralUnmixingExample
     ${IMAGEDATA}/AVIRIS/Indian_pines_corrected.tif
     ${TEMP}/siTvHyperspectralUnmixingExampleTest.tif
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand1.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand2.png
     ${TEMP}/siTvHyperspectralUnmixingExampleTestBand3.png
     16
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)

ADD_EXECUTABLE(otbHyperspectralExamplesTests otbHyperspectralExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbHyperspectralExamplesTests OTBCommon OTBIO OTBHyperspectral OTBCommon OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
