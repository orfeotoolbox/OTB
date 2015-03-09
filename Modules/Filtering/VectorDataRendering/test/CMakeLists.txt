otb_module_test()

set(OTBVectorDataRenderingTests
otbVectorDataRenderingTestDriver.cxx
otbVectorDataToMapFilterWorld.cxx
otbVectorDataToMapFilter.cxx
otbVectorDataToMapFilterNew.cxx
otbVectorDataToMapFilterNoFonts.cxx
otbVectorDataToImageFilter.cxx
otbVectorDataToMapFilterSensorModel.cxx
)

add_executable(otbVectorDataRenderingTestDriver ${OTBVectorDataRenderingTests})
target_link_libraries(otbVectorDataRenderingTestDriver ${OTBVectorDataRendering-Test_LIBRARIES})
otb_module_target_label(otbVectorDataRenderingTestDriver)

# Tests Declaration

otb_add_test(NAME coTvVectorDataToMapFilterWorld COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilterWorld.png
  ${TEMP}/coTvVectorDataToMapFilterWorld.png
  otbVectorDataToMapFilterWorld
  ${INPUTDATA}/world_boundaries/world_boundaries_m.shp
  ${INPUTDATA}/world_boundaries/places.shp
  ${TEMP}/coTvVectorDataToMapFilterWorld.png
  500 500 #Size
  -20 60 #lon/lat
  0.1 -0.1 #spacing lon/lat
  ${INPUTDATA}/DejaVuSans.ttf # font
  )

otb_add_test(NAME coTvVectorDataToMapFilterBinary COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilterBinary.png
  ${TEMP}/coTvVectorDataToMapFilterBinary.png
  otbVectorDataToMapFilterBinary
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  ${TEMP}/coTvVectorDataToMapFilterBinary.png
  )

otb_add_test(NAME coTvVectorDataToMapFilter COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilter.png
  ${TEMP}/coTvVectorDataToMapFilter.png
  otbVectorDataToMapFilter
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  ${TEMP}/coTvVectorDataToMapFilter.png
  ${INPUTDATA}/DejaVuSans.ttf # font
  )

otb_add_test(NAME coTuVectorDataToMapFilterNew COMMAND otbVectorDataRenderingTestDriver
  otbVectorDataToMapFilterNew
  )

otb_add_test(NAME coTvVectorDataToMapFilterNoFonts COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilterNoFonts.png
  ${TEMP}/coTvVectorDataToMapFilterNoFonts.png
  otbVectorDataToMapFilterNoFonts
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  ${TEMP}/coTvVectorDataToMapFilterNoFonts.png
  )

otb_add_test(NAME coTvVectorDataToImageFilter COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilter.png
  ${TEMP}/coTvVectorDataToImageFilter.png
  otbVectorDataToImageFilter
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  ${TEMP}/coTvVectorDataToImageFilter.png
  ${INPUTDATA}/DejaVuSans.ttf # font
  )

otb_add_test(NAME coTvVectorDataToImageFilterBinary COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilterBinary.png
  ${TEMP}/coTvVectorDataToImageFilterBinary.png
  otbVectorDataToImageFilterBinary
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  ${TEMP}/coTvVectorDataToImageFilterBinary.png
  )

otb_add_test(NAME coTvVectorDataToMapFilterSensorModel COMMAND otbVectorDataRenderingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvVectorDataToMapFilterSensorModel.png
  ${TEMP}/coTvVectorDataToMapFilterSensorModel.png
  otbVectorDataToMapFilterSensorModel
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/coTvVectorDataToMapFilterSensorModel.png
  ${INPUTDATA}/DejaVuSans.ttf # font
  )

