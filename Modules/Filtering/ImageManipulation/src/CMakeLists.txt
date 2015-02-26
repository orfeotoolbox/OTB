set(OTBImageManipulation_SRC
  otbStreamingShrinkImageFilter.cxx
  )

add_library(OTBImageManipulation ${OTBImageManipulation_SRC})
target_link_libraries(OTBImageManipulation
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBITKPendingPatches_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  )

otb_module_target(OTBImageManipulation)
