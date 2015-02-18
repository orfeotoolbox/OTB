set(OTBCurlAdapters_SRC
  otbCurlHelperStub.cxx
  otbCurlHelper.cxx
  otbCurlHelperInterface.cxx
  )

add_library(OTBCurlAdapters ${OTBCurlAdapters_SRC})
target_link_libraries(OTBCurlAdapters 
  ${OTBCurl_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBCurlAdapters)
