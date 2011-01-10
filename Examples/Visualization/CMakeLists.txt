PROJECT(VisuExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(VisuExample1 VisuExample1.cxx )
TARGET_LINK_LIBRARIES(VisuExample1 OTBVisualization OTBGui OTBIO OTBCommon ${OTB_VISU_GUI_LIBRARIES})

IF(OTB_USE_MAPNIK)
  ADD_EXECUTABLE(VectorDataRendering VectorDataRendering.cxx )
  TARGET_LINK_LIBRARIES(VectorDataRendering OTBVisualization OTBGui OTBIO OTBCommon ${OTB_VISU_GUI_LIBRARIES})
ENDIF(OTB_USE_MAPNIK)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Visualization)

SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbVisualizationExamplesTests)

SET(TOL 0.0)
SET(EPSILON_4 0.0001)

IF(OTB_DATA_USE_LARGEINPUT)
  IF(OTB_USE_MAPNIK)
    ADD_TEST(vrTeVectorDataRendering ${EXE_TESTS}
      VectorDataRenderingTest
      ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
      ${INPUTLARGEDATA}/VECTOR/MidiPyrenees/roads.shp
      ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
      0
      )
  ENDIF(OTB_USE_MAPNIK) 
ENDIF(OTB_DATA_USE_LARGEINPUT)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)

# Compile the example only if OTB uses mapnick
# This file (for tests registration) will be reserved for test using mapnick
IF(OTB_USE_MAPNIK)
  ADD_EXECUTABLE(otbVisualizationExamplesTests otbVisualizationExamplesTests.cxx)
  TARGET_LINK_LIBRARIES(otbVisualizationExamplesTests OTBCommon OTBIO OTBVisualization OTBTesting)
ENDIF(OTB_USE_MAPNIK)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )