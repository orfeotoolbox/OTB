otb_module_test()

set(OTBSpatialObjectsTests
otbSpatialObjectsTestDriver.cxx
otbLineSpatialObjectListToPointSetFilterNew.cxx
otbSpatialObjectToImageDrawingFilter.cxx
otbDrawLineSpatialObject.cxx
otbSpatialObjectToImageDrawingFilterNew.cxx
otbLineSpatialObjectList.cxx
otbLineSpatialObjectNew.cxx
otbDrawLineSpatialObjectListNew.cxx
otbDrawLineSpatialObjectList.cxx
otbSpatialObjectSourceNew.cxx
otbImageToLineSpatialObjectListNew.cxx
otbDrawLineSpatialObjectNew.cxx
)

add_executable(otbSpatialObjectsTestDriver ${OTBSpatialObjectsTests})
target_link_libraries(otbSpatialObjectsTestDriver ${OTBSpatialObjects-Test_LIBRARIES})
otb_module_target_label(otbSpatialObjectsTestDriver)

# Tests Declaration


otb_add_test(NAME coTuLineSpatialObjectListToPointSetFilter COMMAND otbSpatialObjectsTestDriver
  otbLineSpatialObjectListToPointSetFilterNew)

otb_add_test(NAME bfTvSpatialObjectToImageDrawingFilterWithoutInputImage COMMAND otbSpatialObjectsTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvSpatialObjectDrawingNoInput.png
  ${TEMP}/bfTvSpatialObjectDrawingNoInput.png
  otbSpatialObjectToImageDrawingFilter
  ${TEMP}/bfTvSpatialObjectDrawingNoInput.png
  100
  50
  )


otb_add_test(NAME coTuDrawLineSpatialObject COMMAND otbSpatialObjectsTestDriver
  otbDrawLineSpatialObject
  ${INPUTDATA}/ImageLineDir.bsq
  ${TEMP}/coFiltreDrawLine_ImageLineDir.png 10. 22. 38. 42.)

otb_add_test(NAME bfTuSpatialObjectToImageDrawingFilterNew COMMAND otbSpatialObjectsTestDriver
  otbSpatialObjectToImageDrawingFilterNew)

otb_add_test(NAME coTuLineSpatialObjectList COMMAND otbSpatialObjectsTestDriver
  otbLineSpatialObjectList
  )

otb_add_test(NAME coTuLineSpatialObjectNew COMMAND otbSpatialObjectsTestDriver
  otbLineSpatialObjectNew)

otb_add_test(NAME coTuDrawLineSpatialObjectListNew COMMAND otbSpatialObjectsTestDriver
  otbDrawLineSpatialObjectListNew)

otb_add_test(NAME coTuDrawLineSpatialObjectList COMMAND otbSpatialObjectsTestDriver
  otbDrawLineSpatialObjectList
  ${INPUTDATA}/ImageLineDir.bsq
  ${TEMP}/coFiltreDrawLineList_ImageLineDir.png )

otb_add_test(NAME coTuSpatialObjectSourceNew COMMAND otbSpatialObjectsTestDriver
  otbSpatialObjectSourceNew)

otb_add_test(NAME coTuImageToLineSpatialObjectListNew COMMAND otbSpatialObjectsTestDriver
  otbImageToLineSpatialObjectListNew)

otb_add_test(NAME coTuDrawLineSpatialObjectNew COMMAND otbSpatialObjectsTestDriver
  otbDrawLineSpatialObjectNew)

