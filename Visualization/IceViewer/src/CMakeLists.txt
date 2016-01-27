set(OTBICEVIEWER_SRCS
  otbIceViewer.cxx)

add_library(OTBIceViewer ${OTBICEVIEWER_SRCS})
target_link_libraries(OTBIceViewer OTBIce $[OTBGLFW_LIBRARIES} ${OTBGlut_LIBRARIES})
otb_module_target(OTBIceViewer)

add_executable(otbIceViewer otbIce.cxx)
target_link_libraries(otbIceViewer OTBIceViewer)
otb_module_target(otbIceViewer)
