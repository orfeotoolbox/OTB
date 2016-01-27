project(OTBIce)

set(OTBIce_LIBRARIES OTBIce)

if(OTB_USE_GLFW && OTB_USE_GLUT)
  set(OTBIce_LIBRARIES $[OTBIce_LIBRARIES} OTBIceViewer)
endif()

otb_module_impl()
