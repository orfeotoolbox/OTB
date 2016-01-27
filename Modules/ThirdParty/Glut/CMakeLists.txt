project(OTBGlut)
set(OTBGlut_THIRD_PARTY 1)

set(OTBGlut_SYSTEM_INCLUDE_DIRS ${GLUT_INCLUDE_DIR})
set(OTBGlut_LIBRARIES ${GLUT_glut_LIBRARY})

otb_module_impl()
