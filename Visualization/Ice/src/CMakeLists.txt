file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx")

set(OTBICE_SRCS
  otbFragmentShader.cxx
  otbFragmentShaderRegistry.cxx
  otbGeoInterface.cxx
  otbGlActor.cxx
  otbGlImageActor.cxx
  otbGlROIActor.cxx
  otbGlVectorActor.cxx
  otbGlVersionChecker.cxx
  otbGlView.cxx
  otbStandardShader.cxx
  otbViewSettings.cxx
  )

add_library(OTBIce ${OTBICE_SRCS})
target_link_libraries(OTBIce ${OTBCommon_LIBRARIES} ${OTBOpenGL_LIBRARIES} ${OTBGlew_LIBRARIES})

otb_module_target(OTBIce)


if(OTB_USE_GLFW && OTB_USE_GLUT)
  set(OTBICEVIEWER_SRCS
    otbIceViewer.cxx)

  add_library(OTBIceViewer ${OTBICEVIEWER_SRCS})
  target_link_libraries(OTBIceViewer OTBIce $[OTBGLFW_LIBRARIES} ${OTBGlut_LIBRARIES})
  otb_module_target(OTBIceViewer)

  add_executable(otbIceViewer otbIce.cxx)
  target_link_libraries(otbIceViewer OTBIceViewer)
  otb_module_target(otbIceViewer)
  
endif()
