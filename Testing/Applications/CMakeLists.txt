set(INPUTDATA ${OTB_DATA_ROOT}/Input)

if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(BASELINE              ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES        ${OTB_DATA_ROOT}/Baseline/OTB/Files)

SET(OTBAPP_BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Images)
SET(OTBAPP_BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Files)


SET(NOTOL 0.0)
SET(EPSILON_3 0.001)
SET(EPSILON_7 0.0000001)
SET(EPSILON_9 0.000000001)

add_subdirectory(Classification)
add_subdirectory(FeatureExtraction)
add_subdirectory(Hyperspectral)
add_subdirectory(Segmentation)
add_subdirectory(Util)
