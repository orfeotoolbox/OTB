otb_module_test()

set(OTBIOTileMapTests
otbIOTileMapTestDriver.cxx
otbTileMapImageIOHelperTest.cxx
otbTileMapImageIOTest.cxx
otbTileMapWriter.cxx
)

add_executable(otbIOTileMapTestDriver ${OTBIOTileMapTests})
target_link_libraries(otbIOTileMapTestDriver ${OTBIOTileMap-Test_LIBRARIES})
otb_module_target_label(otbIOTileMapTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuTileMapImageIOHelperNew COMMAND otbIOTileMapTestDriver
  otbTileMapImageIOHelperNew
  )

otb_add_test(NAME ioTvTileMapImageIOHelperTest COMMAND otbIOTileMapTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvTileMapImageIOHelperTest.txt
  ${TEMP}/ioTvTileMapImageIOHelperTest.txt
  otbTileMapImageIOHelperTest
  ${TEMP}/ioTvTileMapImageIOHelperTest.txt
  )

otb_add_test(NAME ioTvTileMapImageIOWeb COMMAND otbIOTileMapTestDriver
  otbTileMapImageIOTest
  http://tile.openstreetmap.org/
  ${TEMP}/ioTvTileMapImageIOWebOutput.png
  ${TEMP}
  1.4835345 43.55968261 13
  )

otb_add_test(NAME ioTvTileMapImageIOFile COMMAND otbIOTileMapTestDriver
  otbTileMapImageIOTest
  ${OTB_DATA_ROOT}/Examples/osmfile.otb
  ${TEMP}/ioTvTileMapImageIOFileOutput.png
  ${TEMP}
  1.4835345 43.55968261 13
  )

otb_add_test(NAME ioTvTileMapWriter COMMAND otbIOTileMapTestDriver
  otbTileMapWriter
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
  ${TEMP}
  500 # extract index[0]
  500 # extract index[1]
  200 # extract size[0]
  200 # extract size[1]
  )

