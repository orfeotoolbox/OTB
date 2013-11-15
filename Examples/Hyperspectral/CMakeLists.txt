#
# Examples on the use of hyperspectral algorithms
#

project(HyperspectralExamples)
include_regular_expression("^.*$")

add_executable(HyperspectralUnmixingExample HyperspectralUnmixingExample.cxx )
target_link_libraries(HyperspectralUnmixingExample OTBCommon OTBIO OTBHyperspectral)


if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Hyperspectral)
set(BASELINE_IMAGES_OTB ${OTB_DATA_ROOT}/Baseline/OTB/Images)

set(INPUTDATA_EXAMPLES ${OTB_DATA_ROOT}/Examples)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

if(OTB_DATA_USE_LARGEINPUT)
  set(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif()

set(EXE_TESTS ${CXX_TEST_PATH}/otbHyperspectralExamplesTests)

set(TOL 0.0)

# tests#
# ------- HyperspectralUnmixingExampleTest ----------
if(OTB_DATA_USE_LARGEINPUT)

add_test(siTvHyperspectralExampleTest ${EXE_TESTS}
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
endif()

include_directories(${OTB_SOURCE_DIR}/Testing/Code)

add_executable(otbHyperspectralExamplesTests otbHyperspectralExamplesTests.cxx)
target_link_libraries(otbHyperspectralExamplesTests OTBCommon OTBIO OTBHyperspectral OTBCommon OTBTesting)

endif()
