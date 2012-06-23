file(GLOB srcs "*.cxx")

add_library(OTBWrapperCommandLine ${srcs})
target_link_libraries(OTBWrapperCommandLine OTBApplicationEngine)

IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(OTBWrapperCommandLine PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

install(TARGETS OTBWrapperCommandLine
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

        

set( SRCS
     otbApplicationLauncherCommandLine.cxx )

add_executable(otbApplicationLauncherCommandLine ${SRCS})
target_link_libraries(otbApplicationLauncherCommandLine
                       OTBWrapperCommandLine
                       OTBApplicationEngine)

install(TARGETS otbApplicationLauncherCommandLine
        RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

# Where we will install the script in the build tree
get_target_property(CLI_OUPUT_DIR otbApplicationLauncherCommandLine RUNTIME_OUTPUT_DIRECTORY)

# What is the path to the applications
set(APPLICATION_BINARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Generate a script in the build dir, next to the cli launcher
# Need a two-step process since configure_file don't support permissions
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree)

if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.bat.buildtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbcli.bat
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbcli.bat
     DESTINATION ${CLI_OUPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.bat.installtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbcli.bat
                @ONLY )
install(PROGRAMS ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbcli.bat
        DESTINATION ${OTB_INSTALL_BIN_DIR}
        COMPONENT RuntimeLibraries)

else()
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.sh.buildtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbcli
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/buildtree/otbcli
     DESTINATION ${CLI_OUPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)  
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.sh.installtree.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbcli
                @ONLY )
install(PROGRAMS ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/installtree/otbcli
        DESTINATION ${OTB_INSTALL_BIN_DIR}
        COMPONENT RuntimeLibraries)

endif()
