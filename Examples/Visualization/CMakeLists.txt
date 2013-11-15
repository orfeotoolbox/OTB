project(VisuExamples)
include_regular_expression("^.*$")

add_executable(VisuExample1 VisuExample1.cxx )
target_link_libraries(VisuExample1 OTBVisualization OTBGui OTBIO OTBCommon ${OTB_VISU_GUI_LIBRARIES})

if(OTB_USE_MAPNIK)
  add_executable(VectorDataRendering VectorDataRendering.cxx )
  target_link_libraries(VectorDataRendering OTBVisualization OTBGui OTBIO OTBCommon ${OTB_VISU_GUI_LIBRARIES})
endif()

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(INPUTDATA ${OTB_DATA_ROOT}/Input)

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
if(OTB_DATA_USE_LARGEINPUT)
  set(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif()

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbVisualizationExamplesTests)

set(TOL 0.0)
set(EPSILON_4 0.0001)

# With Font file
if(OTB_DATA_USE_LARGEINPUT)
  if(OTB_USE_MAPNIK)
    add_test(vrTeVectorDataRendering ${EXE_TESTS}
      VectorDataRenderingTest
      ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
      ${INPUTLARGEDATA}/VECTOR/MidiPyrenees/roads.shp
      ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
      0
      ${INPUTDATA}/DejaVuSans.ttf
      )
  endif()
endif()

# Without Font file
if(OTB_DATA_USE_LARGEINPUT)
  if(OTB_USE_MAPNIK)
    add_test(vrTeVectorDataRenderingWithoutFont ${EXE_TESTS}
      VectorDataRenderingTest
      ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
      ${INPUTLARGEDATA}/VECTOR/MidiPyrenees/roads.shp
      ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
      0
      )
  endif()
endif()


include_directories(${OTB_SOURCE_DIR}/Testing/Code)

# Compile the example only if OTB uses mapnick
# This file (for tests registration) will be reserved for test using mapnick
if(OTB_USE_MAPNIK)
  add_executable(otbVisualizationExamplesTests otbVisualizationExamplesTests.cxx)
  target_link_libraries(otbVisualizationExamplesTests OTBCommon OTBIO OTBVisualization OTBTesting)
endif()

endif()
