macro(otb_create_application)
   cmake_parse_arguments(APPLICATION  "" "NAME;BUILD_PATH;INSTALL_PATH" "SOURCES;INCLUDE_DIRS;LINK_LIBRARIES" ${ARGN} )

   set( APPLICATION_TARGET_NAME otbapp_${APPLICATION_NAME} )

   # Build the library as a MODULE (shared lib even if OTB is built statically)
   include_directories(${APPLICATION_INCLUDE_DIRS})
   add_library(${APPLICATION_TARGET_NAME} MODULE ${APPLICATION_SOURCES})
   target_link_libraries(${APPLICATION_TARGET_NAME} OTBApplicationEngine ${APPLICATION_LINK_LIBRARIES})
   if(otb-module)
     otb_module_target_label(${APPLICATION_TARGET_NAME})
   endif()

   # Setup build output location
   # Do not output in the standard lib folder where all shared libs goes.
   # This is to avoid the application factory to look into each and every shared lib
   # for itkLoad symbol
   if(otb-module)
     set_property(TARGET ${APPLICATION_TARGET_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/otb/applications)
   endif()

   # Remove the usual 'lib' prefix to make it clear it is a plugin
   # and not a shared library to link against
   set_property(TARGET ${APPLICATION_TARGET_NAME} PROPERTY PREFIX "")

   # on Apple platform, a "MODULE" library gets a ".so" extension
   # but the ITK factory mechanism looks for ".dylib"
   if (APPLE)
     set_property(TARGET ${APPLICATION_TARGET_NAME} PROPERTY SUFFIX ".dylib")
   endif()

   # When called from the OTB build system, use OTB_INSTALL_APP_DIR
   if (NOT APPLICATION_INSTALL_PATH AND OTB_INSTALL_APP_DIR)
     set(APPLICATION_INSTALL_PATH ${OTB_INSTALL_APP_DIR})
   endif()

   if (APPLICATION_INSTALL_PATH)
     if(otb-module)
       install(TARGETS ${APPLICATION_TARGET_NAME}
               EXPORT ${${otb-module}-targets}
               LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH}
               COMPONENT RuntimeLibraries)
     else()
       install(TARGETS ${APPLICATION_TARGET_NAME}
               LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH}
               COMPONENT RuntimeLibraries)
     endif()
   else()
     install(TARGETS ${APPLICATION_TARGET_NAME}
             LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
             COMPONENT RuntimeLibraries)
   endif()

   # Generate a quickstart script in the build dir
   #if (NOT WIN32)

      # What is the path to the applications
      # a MODULE target is always treated as LIBRARY
      get_target_property(APPLICATION_BINARY_PATH ${APPLICATION_TARGET_NAME} LIBRARY_OUTPUT_DIRECTORY)

      if (NOT APPLICATION_BINARY_PATH)
        set(APPLICATION_BINARY_PATH ${CMAKE_CURRENT_BINARY_DIR})
      endif()

      if (WIN32)
        set(SCRIPT_CLI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbcli_app.bat.in)
        set(SCRIPT_CLI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbcli_${APPLICATION_NAME}.bat)
        set(SCRIPT_CLI_INSTALLABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/otbcli_${APPLICATION_NAME}.bat)
      else()
        set(SCRIPT_CLI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbcli_app.sh.in)
        set(SCRIPT_CLI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbcli_${APPLICATION_NAME})
        set(SCRIPT_CLI_INSTALLABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/otbcli_${APPLICATION_NAME})
      endif()

      if (EXISTS ${SCRIPT_CLI_SOURCE})
          # Generate a script in the build dir, next to the cli launcher
          configure_file( ${SCRIPT_CLI_SOURCE}
                          ${SCRIPT_CLI_INTERMEDIATE}
                          @ONLY )

          # Copy it next to the application shared lib, and give executable rights
          file(COPY ${SCRIPT_CLI_INTERMEDIATE}
               DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

          # Install a version of this script if we are inside the OTB build
          install(PROGRAMS ${SCRIPT_CLI_INSTALLABLE}
                  DESTINATION ${OTB_INSTALL_RUNTIME_DIR}
                  COMPONENT Runtime)
      endif()

      if (WIN32)
        set(SCRIPT_GUI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbgui_app.bat.in)
        set(SCRIPT_GUI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbgui_${APPLICATION_NAME}.bat)
        set(SCRIPT_GUI_INSTALLABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/otbgui_${APPLICATION_NAME}.bat)
      else()
        set(SCRIPT_GUI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbgui_app.sh.in)
        set(SCRIPT_GUI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbgui_${APPLICATION_NAME})
        set(SCRIPT_GUI_INSTALLABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/otbgui_${APPLICATION_NAME})
      endif()

      if (EXISTS ${SCRIPT_GUI_SOURCE})
          # Generate a script in the build dir, next to the cli launcher
          configure_file( ${SCRIPT_GUI_SOURCE}
                          ${SCRIPT_GUI_INTERMEDIATE}
                          @ONLY )

          # Copy it next to the application shared lib, and give executable rights
          file(COPY ${SCRIPT_GUI_INTERMEDIATE}
               DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

          # Install a version of this script if we are inside the OTB build
          install(PROGRAMS ${SCRIPT_GUI_INSTALLABLE}
                  DESTINATION ${OTB_INSTALL_RUNTIME_DIR}
                  COMPONENT Runtime)
      endif()
   #endif()

   list(APPEND OTB_APPLICATIONS_NAME_LIST ${APPLICATION_NAME})
   list(REMOVE_DUPLICATES OTB_APPLICATIONS_NAME_LIST)
   set(OTB_APPLICATIONS_NAME_LIST ${OTB_APPLICATIONS_NAME_LIST}
       CACHE STRING "List of all applications" FORCE)

endmacro()

macro(otb_test_application)
  cmake_parse_arguments(TESTAPPLICATION  "" "NAME;APP" "OPTIONS;TESTENVOPTIONS;VALID" ${ARGN} )
  if(otb-module)
    otb_add_test(NAME ${TESTAPPLICATION_NAME}
                  COMMAND otbTestDriver
                  ${TESTAPPLICATION_VALID}
                  Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                  ${TESTAPPLICATION_APP}
                  $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
                  ${TESTAPPLICATION_OPTIONS}
                  -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
    # Be sure that the ${otb-module}-all target triggers the build of commandline launcher and testdriver
    add_dependencies(${otb-module}-all otbApplicationLauncherCommandLine)
    add_dependencies(${otb-module}-all otbTestDriver)
  else()
    add_test(NAME ${TESTAPPLICATION_NAME}
            COMMAND otbTestDriver
            ${TESTAPPLICATION_VALID}
            Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
            ${TESTAPPLICATION_APP}
            $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
            ${TESTAPPLICATION_OPTIONS}
            -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
  endif()
endmacro()
