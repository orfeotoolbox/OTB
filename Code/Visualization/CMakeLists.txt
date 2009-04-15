# Sources of non-templated classes.

FILE(GLOB OTBVisualization_SRCS "*.cxx" )

# To suppress "ld: cycle in dylib re-exports with /usr/X11R6/lib/libGL.dylib" error on APPLE and SHARED configuration 
IF(APPLE AND BUILD_SHARED_LIBS)
      FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        SET(CMAKE_SHARED_LINKER_FLAGS${c} "${CMAKE_SHARED_LINKER_FLAGS${c}} -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
      ENDFOREACH(c)
ENDIF(APPLE AND BUILD_SHARED_LIBS)

FLTK_WRAP_UI(OTBVisualization otbImageWidgetPackedManager.fl otbImageWidgetSplittedManager.fl)

ADD_LIBRARY(OTBVisualization ${OTBVisualization_SRCS} ${OTBVisualization_FLTK_UI_SRCS})
TARGET_LINK_LIBRARIES (OTBVisualization OTBGui OTBCommon ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})
IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBVisualization PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS OTBVisualization
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  FILE(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  FILE(GLOB __files4 "${CMAKE_CURRENT_SOURCE_DIR}/*.fl")
  INSTALL(FILES ${__files1} ${__files2}  ${__files3}  ${__files4}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Visualization
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
