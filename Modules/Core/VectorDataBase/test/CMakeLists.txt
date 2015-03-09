otb_module_test()

set(OTBVectorDataBaseTests
otbVectorDataBaseTestDriver.cxx
otbVectorData.cxx
otbVectorDataKeywordlistTest.cxx
otbPolygon.cxx
otbVectorDataSourceNew.cxx
otbDataNodeTest.cxx
otbRemoteSensingRegion.cxx
otbPolygonNew.cxx
otbPolyLineParametricPathWithValueNew.cxx
otbRemoteSensingRegionNew.cxx
otbVectorDataNew.cxx
)

add_executable(otbVectorDataBaseTestDriver ${OTBVectorDataBaseTests})
target_link_libraries(otbVectorDataBaseTestDriver ${OTBVectorDataBase-Test_LIBRARIES})
otb_module_target_label(otbVectorDataBaseTestDriver)

# Tests Declaration

otb_add_test(NAME coTvVectorData COMMAND otbVectorDataBaseTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/coTvVectorData.txt
  ${TEMP}/coTvVectorData.txt
  --ignore-lines-with 1 RTTI
  otbVectorData
  ${TEMP}/coTvVectorData.txt
  )

otb_add_test(NAME ioTuVectorDataKeywordlistNew COMMAND otbVectorDataBaseTestDriver
  otbVectorDataKeywordlistNew
  )

otb_add_test(NAME ioTvVectorDataKeywordlist COMMAND otbVectorDataBaseTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/ioTuVectorDataKeywordlist.txt
  ${TEMP}/ioTuVectorDataKeywordlist.txt
  otbVectorDataKeywordlist
  ${INPUTDATA}/waterways.shp
  ${TEMP}/ioTuVectorDataKeywordlist.txt
  )

otb_add_test(NAME coTvPolygon COMMAND otbVectorDataBaseTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/otbPolygonTest.txt
  ${TEMP}/otbPolygonTest.txt
  otbPolygon
  ${TEMP}/otbPolygonTest.txt
  2 10 7 10 7 9 5 9 5 8 7 8 7 6 9 6 9 5 7 5 7 3 2 3
  next 6 10 11 10 11 2 4 2 3 4 8 4 8 7 6 7
  )

otb_add_test(NAME coTuVectorDataSource COMMAND otbVectorDataBaseTestDriver
  otbVectorDataSourceNew
  )


otb_add_test(NAME coTvDataNode COMMAND otbVectorDataBaseTestDriver
  otbDataNodeTest
  )

otb_add_test(NAME coTvRemoteSensingRegion COMMAND otbVectorDataBaseTestDriver
  otbRemoteSensingRegion
  1.21 2.33 58.47 59.49  #size et index of the region
  0. 0.                  # Point to check
  )

otb_add_test(NAME coTuPolygonNew COMMAND otbVectorDataBaseTestDriver
  otbPolygonNew)

otb_add_test(NAME coTuPolyLineParametricPathWithValueNew COMMAND otbVectorDataBaseTestDriver
  otbPolyLineParametricPathWithValueNew)

otb_add_test(NAME coTuRemoteSensingRegionNew COMMAND otbVectorDataBaseTestDriver
  otbRemoteSensingRegionNew )


otb_add_test(NAME coTuVectorData COMMAND otbVectorDataBaseTestDriver
  otbVectorDataNew
  )

