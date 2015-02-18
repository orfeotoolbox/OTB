set(OTBImageManipulation_SRC
  otbStreamingShrinkImageFilter.cxx
  )

add_library(OTBImageManipulation ${OTBImageManipulation_SRC})
target_link_libraries(OTBImageManipulation 
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBImageManipulation)
