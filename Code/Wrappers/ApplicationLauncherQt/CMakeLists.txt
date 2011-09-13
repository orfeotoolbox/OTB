include_directories(${QT_INCLUDE_DIRS})
include_directories(../QtWidget)

set( SRCS
     otbApplicationLauncherQt.cxx )

add_executable(otbApplicationLauncherQt ${SRCS})

target_link_libraries(otbApplicationLauncherQt
                       OTBWrapperQtWidget
                       OTBApplicationEngine
                       ${QT_LIBRARIES})
                       
install(TARGETS otbApplicationLauncherQt
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24}
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24}
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24})
