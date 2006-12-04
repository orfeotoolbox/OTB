 
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

# -------            otb::RCC8VertexBase   --------------------------

ADD_TEST(srTuRCC8VertexBaseNew ${SPATIALREASONING_TESTS} 
         otbRCC8VertexBaseNew)

ADD_TEST(srTvRCC8VertexBase ${SPATIALREASONING_TESTS} 
         otbRCC8VertexBase
	 5
	 5
	 )

# -------            otb::RCC8VertexWithCompacity----------------------

ADD_TEST(srTuRCC8VertexWithCompacityNew ${SPATIALREASONING_TESTS} 
         otbRCC8VertexWithCompacityNew)

ADD_TEST(srTvRCC8VertexWithCompacity ${SPATIALREASONING_TESTS} 
         otbRCC8VertexWithCompacity
	 5
	 5
	 0.125354
	 )

# -------            otb::RCC8VertexWithCompacity----------------------

ADD_TEST(srTuRCC8EdgeNew ${SPATIALREASONING_TESTS} 
         otbRCC8EdgeNew)

ADD_TEST(srTvRCC8Edge ${SPATIALREASONING_TESTS} 
         otbRCC8Edge
	 )

# -------            otb::RCC8Graph   --------------------------

ADD_TEST(srTuRCC8GraphNew ${SPATIALREASONING_TESTS} 
         otbRCC8GraphNew)

ADD_TEST(srTvRCC8Graph ${SPATIALREASONING_TESTS} 
         otbRCC8Graph	 
	 )

# -------            otb::RCC8GraphSource   --------------------------

ADD_TEST(srTuRCC8GraphSourceNew ${SPATIALREASONING_TESTS} 
         otbRCC8GraphSourceNew)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicSpatialReasoning_SRCS
otbImageToImageRCC8CalculatorNew.cxx
otbImageToImageRCC8Calculator.cxx
otbRCC8VertexBaseNew.cxx
otbRCC8VertexBase.cxx
otbRCC8VertexWithCompacityNew.cxx
otbRCC8VertexWithCompacity.cxx
otbRCC8EdgeNew.cxx
otbRCC8Edge.cxx
otbRCC8GraphNew.cxx
otbRCC8Graph.cxx
otbRCC8GraphSourceNew.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSpatialReasoningTests otbSpatialReasoningTests.cxx ${BasicSpatialReasoning_SRCS})
TARGET_LINK_LIBRARIES(otbSpatialReasoningTests OTBIO OTBSpatialReasoning gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
