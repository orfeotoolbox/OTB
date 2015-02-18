add_library(OTBTestKernel otbTestHelper.cxx)
target_link_libraries(OTBTestKernel 
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  )
otb_module_target(OTBTestKernel)

add_executable(otbTestDriver otbTestDriver.cxx)
target_link_libraries(otbTestDriver OTBTestKernel)
otb_module_target(otbTestDriver)
