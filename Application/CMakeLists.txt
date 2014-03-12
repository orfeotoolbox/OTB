ADD_EXECUTABLE(otbiceviewer otbIce.cxx otbIceViewer.cxx)
TARGET_LINK_LIBRARIES(otbiceviewer OTBIce ${GLFW_LIBRARY} ${GLUT_LIBRARY})

install(TARGETS otbiceviewer
        RUNTIME DESTINATION ${Ice_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${Ice_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${Ice_INSTALL_LIB_DIR} COMPONENT Development)
