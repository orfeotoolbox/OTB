set(OTBCommandLine_SRC
  otbWrapperCommandLineLauncher.cxx
  otbWrapperCommandLineParser.cxx
  )

add_library(OTBCommandLine ${OTBCommandLine_SRC})
target_link_libraries(OTBCommandLine 
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  )
otb_module_target(OTBCommandLine)

add_executable(otbApplicationLauncherCommandLine otbApplicationLauncherCommandLine.cxx)
target_link_libraries(otbApplicationLauncherCommandLine OTBCommandLine)
otb_module_target(otbApplicationLauncherCommandLine)

# Where we will install the script in the build tree
get_target_property(CLI_OUTPUT_DIR otbApplicationLauncherCommandLine RUNTIME_OUTPUT_DIRECTORY)

# Generate a script in the build dir, next to the cli launcher
# Need a two-step process since configure_file don't support permissions
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts)

if (WIN32)
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.bat.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbcli.bat
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbcli.bat
     DESTINATION ${CLI_OUTPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# TODO : test if NO_INSTALL is set
install(PROGRAMS ${CLI_OUTPUT_DIR}/otbcli.bat
        DESTINATION ${OTBCommandLine_INSTALL_RUNTIME_DIR}
        COMPONENT Runtime)

else()
configure_file( ${CMAKE_SOURCE_DIR}/CMake/otbcli.sh.in
                ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbcli
                @ONLY )
file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Scripts/otbcli
     DESTINATION ${CLI_OUTPUT_DIR}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# TODO : test if NO_INSTALL is set
install(PROGRAMS ${CLI_OUTPUT_DIR}/otbcli
        DESTINATION ${OTBCommandLine_INSTALL_RUNTIME_DIR}
        COMPONENT Runtime)

endif()
