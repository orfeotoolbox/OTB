 
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
# (also testing otb::RCC8VertexIterator and otb::RCC8EdgeIterator)

ADD_TEST(srTuRCC8GraphNew ${SPATIALREASONING_TESTS} 
         otbRCC8GraphNew)

ADD_TEST(srTvRCC8Graph ${SPATIALREASONING_TESTS} 
         otbRCC8Graph	 
	 )

# -------            otb::RCC8GraphSource   --------------------------

ADD_TEST(srTuRCC8GraphSourceNew ${SPATIALREASONING_TESTS} 
         otbRCC8GraphSourceNew)

# -------        otb::RCC8GraphFileWriter   --------------------------

ADD_TEST(srTuRCC8GraphFileWriterNew ${SPATIALREASONING_TESTS} 
         otbRCC8GraphFileWriterNew)

ADD_TEST(srTvRCC8GraphFileWriter ${SPATIALREASONING_TESTS} 
	--compare-ascii	${TOL}
			${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
			${TEMP}/srRCC8GraphWriterOutput1.dot
	 otbRCC8GraphFileWriter
			${TEMP}/srRCC8GraphWriterOutput1.dot
)

# -------        otb::RCC8GraphFileReader   --------------------------

ADD_TEST(srTuRCC8GraphFileReaderNew ${SPATIALREASONING_TESTS} 
         otbRCC8GraphFileReaderNew)

ADD_TEST(srTvRCC8GraphFileReader ${SPATIALREASONING_TESTS} 
	 otbRCC8GraphFileReader
			${TEMP}/srRCC8GraphWriterOutput1.dot
)

ADD_TEST(srTvRCC8GraphIOEndToEnd ${SPATIALREASONING_TESTS}
	--compare-ascii	${TOL}
			${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
			${TEMP}/srRCC8GraphIOEndToEndOut.dot
	 otbRCC8GraphIOEndToEnd
	                ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
			${TEMP}/srRCC8GraphIOEndToEndOut.dot
)

# -------        otb::ImageListToRCC8GraphFilter   --------------------------



ADD_TEST(srTuImageListToRCC8GraphFilterNew ${SPATIALREASONING_TESTS} 
         otbImageListToRCC8GraphFilterNew)


# -------        otb::ImageMultiSegmentationToRCC8GraphFilter   --------------------------

ADD_TEST(srTuMultiSegToRCC8GraphFilterNew ${SPATIALREASONING_TESTS} 
         otbImageMultiSegmentationToRCC8GraphFilterNew)


ADD_TEST(srTvMultiSegToRCC8GraphFilter1 ${SPATIALREASONING_TESTS}
	--compare-ascii	${TOL}
			${BASELINE_FILES}/srRCC8GraphFilterOutput.dot
			${TEMP}/srRCC8GraphFilterOutput.dot
	 otbImageMultiSegmentationToRCC8GraphFilter
			${TEMP}/srRCC8GraphFilterOutput.dot
			0
			2
			${INPUTDATA}/Seg1InputForRCC8Graph.tif
			${INPUTDATA}/Seg2InputForRCC8Graph.tif			
)

ADD_TEST(srTvMultiSegToRCC8GraphFilter1WithOpti ${SPATIALREASONING_TESTS}
	--compare-ascii	${TOL}
			${BASELINE_FILES}/srRCC8GraphFilterOutput.dot
			${TEMP}/srRCC8GraphFilterOutputWithOpti.dot
	 otbImageMultiSegmentationToRCC8GraphFilter
			${TEMP}/srRCC8GraphFilterOutputWithOpti.dot
			1
			2
			${INPUTDATA}/Seg1InputForRCC8Graph.tif
			${INPUTDATA}/Seg2InputForRCC8Graph.tif		
)

ADD_TEST(srTvMultiSegToRCC8GraphFilter2 ${SPATIALREASONING_TESTS}
	--compare-ascii ${TOL}
			${BASELINE_FILES}/srRCC8GraphFilterOutput2.dot
			${TEMP}/srRCC8GraphFilterOutput2.dot
	 otbImageMultiSegmentationToRCC8GraphFilter
			${TEMP}/srRCC8GraphFilterOutput2.dot
			0
			3
			${INPUTDATA}/Seg3InputForRCC8Graph.tif
			${INPUTDATA}/Seg4InputForRCC8Graph.tif
			${INPUTDATA}/Seg5InputForRCC8Graph.tif
)

ADD_TEST(srTvMultiSegToRCC8GraphFilter2WithOpti ${SPATIALREASONING_TESTS}
	--compare-ascii ${TOL}
			${BASELINE_FILES}/srRCC8GraphFilterOutput2.dot
			${TEMP}/srRCC8GraphFilterOutput2WithOpti.dot
	 otbImageMultiSegmentationToRCC8GraphFilter
			${TEMP}/srRCC8GraphFilterOutput2WithOpti.dot
			1
			3
			${INPUTDATA}/Seg3InputForRCC8Graph.tif
			${INPUTDATA}/Seg4InputForRCC8Graph.tif
			${INPUTDATA}/Seg5InputForRCC8Graph.tif
)

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
otbRCC8GraphFileWriterNew.cxx
otbRCC8GraphFileWriter.cxx
otbRCC8GraphFileReaderNew.cxx
otbRCC8GraphFileReader.cxx
otbRCC8GraphIOEndToEnd.cxx
otbImageListToRCC8GraphFilterNew.cxx
otbImageMultiSegmentationToRCC8GraphFilterNew.cxx
otbImageMultiSegmentationToRCC8GraphFilter.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbSpatialReasoningTests otbSpatialReasoningTests.cxx ${BasicSpatialReasoning_SRCS})
TARGET_LINK_LIBRARIES(otbSpatialReasoningTests OTBIO OTBSpatialReasoning gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
