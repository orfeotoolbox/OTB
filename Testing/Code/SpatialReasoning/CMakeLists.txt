
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(SPATIALREASONING_TESTS1 ${CXX_TEST_PATH}/otbSpatialReasoningTests1)
set(SPATIALREASONING_TESTS2 ${CXX_TEST_PATH}/otbSpatialReasoningTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSPATIALREASONINGTests1 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::ImageToImageRCC8Calculator   ----------

add_test(srTuRCC8CalculatorNew ${SPATIALREASONING_TESTS1}
         otbImageToImageRCC8CalculatorNew)

add_test(srTvRCC8Calculator ${SPATIALREASONING_TESTS1}
   --compare-ascii ${NOTOL}
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


# -------            otb::PolygonToPolygonRCC8Calculator   ----------

add_test(srTuPolygonRCC8CalculatorNew ${SPATIALREASONING_TESTS1}
         otbPolygonToPolygonRCC8CalculatorNew)

add_test(srTvPolygonRCC8Calculator ${SPATIALREASONING_TESTS1}
   --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/srRCC8PolygonToPolygonCalculatorOutput.txt
                    ${TEMP}/srRCC8PolygonToPolygonCalculatorOutput.txt
         otbPolygonToPolygonRCC8Calculator
         4
         ${TEMP}/srRCC8PolygonToPolygonCalculatorOutput.txt
         ${INPUTDATA}/rcc8_mire1.png
         ${INPUTDATA}/rcc8_mire2.png
         ${INPUTDATA}/rcc8_mire3.png
         ${INPUTDATA}/rcc8_mire4.png
)

# -------            otb::RCC8VertexBase   --------------------------

add_test(srTuRCC8VertexBaseNew ${SPATIALREASONING_TESTS1}
         otbRCC8VertexBaseNew)

add_test(srTvRCC8VertexBase ${SPATIALREASONING_TESTS1}
         otbRCC8VertexBase
         5
         5
         )

# -------            otb::RCC8VertexWithCompacity----------------------

add_test(srTuRCC8VertexWithCompacityNew ${SPATIALREASONING_TESTS1}
         otbRCC8VertexWithCompacityNew)

add_test(srTvRCC8VertexWithCompacity ${SPATIALREASONING_TESTS1}
         otbRCC8VertexWithCompacity
         5
         5
         0.125354
         )

# -------            otb::RCC8VertexWithCompacity----------------------

add_test(srTuRCC8EdgeNew ${SPATIALREASONING_TESTS1}
         otbRCC8EdgeNew)

add_test(srTvRCC8Edge ${SPATIALREASONING_TESTS1}
         otbRCC8Edge
         )


# -------            otb::RCC8Graph   --------------------------
# (also testing otb::RCC8VertexIterator and otb::RCC8EdgeIterator)

add_test(srTuRCC8GraphNew ${SPATIALREASONING_TESTS1}
         otbRCC8GraphNew)

add_test(srTvRCC8Graph ${SPATIALREASONING_TESTS1}
         otbRCC8Graph
         )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSPATIALREASONINGTests2 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::RCC8GraphSource   --------------------------

add_test(srTuRCC8GraphSourceNew ${SPATIALREASONING_TESTS2}
         otbRCC8GraphSourceNew)

# -------        otb::RCC8GraphFileWriter   --------------------------

add_test(srTuRCC8GraphFileWriterNew ${SPATIALREASONING_TESTS2}
         otbRCC8GraphFileWriterNew)

add_test(srTvRCC8GraphFileWriter ${SPATIALREASONING_TESTS2}
        --compare-ascii        ${NOTOL}
                        ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
                        ${TEMP}/srRCC8GraphWriterOutput1.dot
         otbRCC8GraphFileWriter
                        ${TEMP}/srRCC8GraphWriterOutput1.dot
)

# -------        otb::RCC8GraphFileReader   --------------------------

add_test(srTuRCC8GraphFileReaderNew ${SPATIALREASONING_TESTS2}
         otbRCC8GraphFileReaderNew)

add_test(srTvRCC8GraphFileReader ${SPATIALREASONING_TESTS2}
         otbRCC8GraphFileReader
                        ${TEMP}/srRCC8GraphWriterOutput1.dot
)
set_tests_properties(srTvRCC8GraphFileReader PROPERTIES DEPENDS srTvRCC8GraphFileWriter)

add_test(srTvRCC8GraphIOEndToEnd ${SPATIALREASONING_TESTS2}
        --compare-ascii        ${NOTOL}
                        ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
                        ${TEMP}/srRCC8GraphIOEndToEndOut.dot
         otbRCC8GraphIOEndToEnd
                        ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
                        ${TEMP}/srRCC8GraphIOEndToEndOut.dot
)

# -------        otb::ImageListToRCC8GraphFilter   --------------------------



add_test(srTuImageListToRCC8GraphFilterNew ${SPATIALREASONING_TESTS2}
         otbImageListToRCC8GraphFilterNew)


# -------        otb::ImageMultiSegmentationToRCC8GraphFilter   --------------------------

add_test(srTuMultiSegToRCC8GraphFilterNew ${SPATIALREASONING_TESTS2}
         otbImageMultiSegmentationToRCC8GraphFilterNew)


add_test(srTvMultiSegToRCC8GraphFilter1 ${SPATIALREASONING_TESTS2}
        --compare-ascii        ${NOTOL}
                        ${BASELINE_FILES}/srRCC8GraphFilterOutput.dot
                        ${TEMP}/srRCC8GraphFilterOutput.dot
         otbImageMultiSegmentationToRCC8GraphFilter
                        ${TEMP}/srRCC8GraphFilterOutput.dot
                        0
                        2
                        ${INPUTDATA}/Seg1InputForRCC8Graph.tif
                        ${INPUTDATA}/Seg2InputForRCC8Graph.tif
)

add_test(srTvMultiSegToRCC8GraphFilter1WithOpti ${SPATIALREASONING_TESTS2}
        --compare-ascii        ${NOTOL}
                        ${BASELINE_FILES}/srRCC8GraphFilterOutput.dot
                        ${TEMP}/srRCC8GraphFilterOutputWithOpti.dot
         otbImageMultiSegmentationToRCC8GraphFilter
                        ${TEMP}/srRCC8GraphFilterOutputWithOpti.dot
                        1
                        2
                        ${INPUTDATA}/Seg1InputForRCC8Graph.tif
                        ${INPUTDATA}/Seg2InputForRCC8Graph.tif
)

add_test(srTvMultiSegToRCC8GraphFilter2 ${SPATIALREASONING_TESTS2}
        --compare-ascii ${NOTOL}
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

add_test(srTvMultiSegToRCC8GraphFilter2WithOpti ${SPATIALREASONING_TESTS2}
        --compare-ascii ${NOTOL}
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

# -------        otb::PolygonListToRCC8GraphFilter   --------------------------

add_test(srTuPolygonListToRCC8GraphFilterNew ${SPATIALREASONING_TESTS2}
         otbPolygonListToRCC8GraphFilterNew)



add_test(srTvPolygonListToRCC8GraphFilter ${SPATIALREASONING_TESTS2}
        --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/srTvPolygonListToRCC8GraphFilterOuptut.dot
                ${TEMP}/srTvPolygonListToRCC8GraphFilterOuptut.dot
         otbPolygonListToRCC8GraphFilter
         ${TEMP}/srTvPolygonListToRCC8GraphFilterOuptut.dot)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbSPATIALREASONINGTests3 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------       Fichiers sources CXX -----------------------------------
set(BasicSpatialReasoning_SRCS1
otbSpatialReasoningTests1.cxx
otbImageToImageRCC8CalculatorNew.cxx
otbImageToImageRCC8Calculator.cxx
otbPolygonToPolygonRCC8CalculatorNew.cxx
otbPolygonToPolygonRCC8Calculator.cxx
otbRCC8VertexBaseNew.cxx
otbRCC8VertexBase.cxx
otbRCC8VertexWithCompacityNew.cxx
otbRCC8VertexWithCompacity.cxx
otbRCC8EdgeNew.cxx
otbRCC8Edge.cxx
otbRCC8GraphNew.cxx
otbRCC8Graph.cxx
)
set(BasicSpatialReasoning_SRCS2
otbSpatialReasoningTests2.cxx
otbRCC8GraphSourceNew.cxx
otbRCC8GraphFileWriterNew.cxx
otbRCC8GraphFileWriter.cxx
otbRCC8GraphFileReaderNew.cxx
otbRCC8GraphFileReader.cxx
otbRCC8GraphIOEndToEnd.cxx
otbImageListToRCC8GraphFilterNew.cxx
otbImageMultiSegmentationToRCC8GraphFilterNew.cxx
otbImageMultiSegmentationToRCC8GraphFilter.cxx
otbPolygonListToRCC8GraphFilterNew.cxx
otbPolygonListToRCC8GraphFilter.cxx
)



OTB_ADD_EXECUTABLE(otbSpatialReasoningTests1 "${BasicSpatialReasoning_SRCS1}" "OTBSpatialReasoning;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbSpatialReasoningTests2 "${BasicSpatialReasoning_SRCS2}" "OTBSpatialReasoning;OTBIO;OTBTesting")

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
