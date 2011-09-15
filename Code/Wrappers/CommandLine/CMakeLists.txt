file(GLOB srcs "*.cxx")

add_library(OTBWrapperCommandLine ${srcs})
target_link_libraries(OTBWrapperCommandLine OTBApplicationEngine)

IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBWrapperCommandLine PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

install(TARGETS OTBWrapperCommandLine
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24}
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24}
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24})

        

set( SRCS
     otbApplicationLauncherCommandLine.cxx )

add_executable(otbApplicationLauncherCommandLine ${SRCS})
target_link_libraries(otbApplicationLauncherCommandLine
                       OTBWrapperCommandLine
                       OTBApplicationEngine
                       ${QT_LIBRARIES})

install(TARGETS otbApplicationLauncherCommandLine
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24}
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24}
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24})
