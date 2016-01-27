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
