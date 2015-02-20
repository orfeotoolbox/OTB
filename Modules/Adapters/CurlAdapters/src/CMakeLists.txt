set(OTBCurlAdapters_SRC
  otbCurlHelperStub.cxx
  otbCurlHelper.cxx
  otbCurlHelperInterface.cxx
  )

add_library(OTBCurlAdapters ${OTBCurlAdapters_SRC})
target_link_libraries(OTBCurlAdapters
  ${OTBCommon_LIBRARIES}
  )

if (OTBCurl_ENABLED)
  target_link_libraries(OTBCurlAdapters ${OTBCurl_LIBRARIES})
endif()

otb_module_target(OTBCurlAdapters)
