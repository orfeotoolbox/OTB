 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)


SET(SPATIALREASONING_TESTS ${CXX_TEST_PATH}/otbSpatialReasoningTests)


# -------       Fichiers sources CXX -----------------------------------
SET(BasicSpatialReasoning_SRCS

)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSpatialReasoningTests otbSpatialReasoningTests.cxx ${BasicSpatialReasoning_SRCS})
TARGET_LINK_LIBRARIES(otbSpatialReasoningTests OTBIO OTBSpatialReasoning gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
