 
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



# -------            otb::ImageToImageRCC8Calculator   ----------

ADD_TEST(srTuRCC8CalculatorNew ${SPATIALREASONING_TESTS} 
         otbImageToImageRCC8CalculatorNew)

ADD_TEST(srTvRCC8Calculator ${SPATIALREASONING_TESTS} 
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/srRCC8CalculatorOutput.txt
 		   ${TEMP}/srRCC8CalculatorOutput.txt
	 otbImageToImageRCC8Calculator
	 4
	 ${TEMP}/srRCC8CalculatorOutput.txt
	 ${INPUTDATA}/rcc8_mire1.png
	 ${INPUTDATA}/rcc8_mire2.png
	 ${INPUTDATA}/rcc8_mire3.png
	 ${INPUTDATA}/rcc8_mire4.png
)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicSpatialReasoning_SRCS
otbImageToImageRCC8CalculatorNew.cxx
otbImageToImageRCC8Calculator.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSpatialReasoningTests otbSpatialReasoningTests.cxx ${BasicSpatialReasoning_SRCS})
TARGET_LINK_LIBRARIES(otbSpatialReasoningTests OTBIO OTBSpatialReasoning gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
