set(OTBWavelet_SRC
  otbWaveletGenerator.cxx
  )

add_library(OTBWavelet ${OTBWavelet_SRC})
target_link_libraries(OTBWavelet
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )

otb_module_target(OTBWavelet)
