# Sources of non-templated classes.

FILE(GLOB OTBVisuRefac_SRCS "*.cxx" )

# To suppress "ld: cycle in dylib re-exports with /usr/X11R6/lib/libGL.dylib" error on APPLE and SHARED configuration 
IF(APPLE AND BUILD_SHARED_LIBS)
      FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        SET(CMAKE_SHARED_LINKER_FLAGS${c} "${CMAKE_SHARED_LINKER_FLAGS${c}} -Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
      ENDFOREACH(c)
ENDIF(APPLE AND BUILD_SHARED_LIBS)

ADD_LIBRARY(OTBVisuRefac ${OTBVisuRefac_SRCS})
TARGET_LINK_LIBRARIES (OTBVisuRefac OTBGui OTBCommon ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})

INSTALL(TARGETS OTBVisuRefac
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")

INSTALL(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/VisuRefac
    COMPONENT Development)
