set(INPUTDATA ${OTB_DATA_ROOT}/Input)

if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)


SET(NOTOL 0.0)
SET(EPSILON_3 0.001)
SET(EPSILON_9 0.000000001)

add_subdirectory(Classification)
add_subdirectory(Hyperspectral)
add_subdirectory(Util)
