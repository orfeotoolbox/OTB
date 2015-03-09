set(OTBRCC8_SRC
  otbRCC8Edge.cxx
  )

add_library(OTBRCC8 ${OTBRCC8_SRC})
target_link_libraries(OTBRCC8 
  ${OTBImageManipulation_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBBoost_LIBRARIES}

  )

otb_module_target(OTBRCC8)
