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
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

# Where we will install the script in the build tree
get_target_property(CLI_OUPUT_DIR otbApplicationLauncherQt RUNTIME_OUTPUT_DIRECTORY)

# What is the path to the applications
# a MODULE target is always treated as LIBRARY
set(APPLICATION_BINARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Generate a script in the build dir, next to the gui launcher
# Need a two-step process since configure_file don't support permissions
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree)


if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.bat.buildtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbgui.bat
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbgui.bat
     DESTINATION ${CLI_OUPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.bat.installtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbgui.bat
                @ONLY )
install(PROGRAMS ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbgui.bat
        DESTINATION ${OTB_INSTALL_BIN_DIR}
        COMPONENT RuntimeLibraries)

else()
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.sh.buildtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbgui
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbgui
     DESTINATION ${CLI_OUPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)  
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbgui.sh.installtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbgui
                @ONLY )
install(PROGRAMS ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbgui
        DESTINATION ${OTB_INSTALL_BIN_DIR}
        COMPONENT RuntimeLibraries)

endif()

