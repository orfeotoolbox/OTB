otb_module_test()

set(OTBUrbanAreaTests
otbUrbanAreaTestDriver.cxx
otbUrbanAreaDetectionImageFilterNew.cxx
otbUrbanAreaDetectionImageFilter.cxx
)

add_executable(otbUrbanAreaTestDriver ${OTBUrbanAreaTests})
target_link_libraries(otbUrbanAreaTestDriver ${OTBUrbanArea-Test_LIBRARIES})
otb_module_target_label(otbUrbanAreaTestDriver)

# Tests Declaration

otb_add_test(NAME raTuUrbanAreaDetectionImageFilterNew COMMAND otbUrbanAreaTestDriver
  otbUrbanAreaDetectionImageFilterNew
  )

otb_add_test(NAME raTvUrbanAreaDetectionImageFilter COMMAND otbUrbanAreaTestDriver
  --compare-image ${EPSILON_8}   ${BASELINE}/raTvUrbanAreaDetection.png
  ${TEMP}/raTvUrbanAreaDetection.png
  otbUrbanAreaDetectionImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raTvUrbanAreaDetection.png
  3    # red
  2    # green
  4    # nir
  0.8  # threshold NDVI+NDWI
  0.005 # threshold EdgeDensity
  )

