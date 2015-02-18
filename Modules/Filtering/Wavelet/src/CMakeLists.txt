set(OTBWavelet_SRC
  otbWaveletGenerator.cxx
  )

add_library(OTBWavelet ${OTBWavelet_SRC})
target_link_libraries(OTBWavelet 
  ${OTBCommon_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}

  )

otb_module_target(OTBWavelet)
