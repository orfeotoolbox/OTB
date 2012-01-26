
# When used externally, OTBParseArguments is included explicitely in UseOTB.cmake
# Otherwise, let's include it here
if (NOT COMMAND OTB_PARSE_ARGUMENTS)
  include(OTBParseArguments)
endif()

macro(OTB_CREATE_APPLICATION)
  OTB_PARSE_ARGUMENTS(
      APPLICATION
      "NAME;SOURCES;INCLUDE_DIRS;LINK_LIBRARIES;BUILD_PATH;INSTALL_PATH"
      ""
      ${ARGN})

   set( APPLICATION_TARGET_NAME otbapp_${APPLICATION_NAME} )

   # Build the library as a MODULE (shared lib even if OTB is built statically)
   include_directories(${APPLICATION_INCLUDE_DIRS})
   add_library(${APPLICATION_TARGET_NAME} MODULE ${APPLICATION_SOURCES})
   target_link_libraries(${APPLICATION_TARGET_NAME} OTBApplicationEngine ${APPLICATION_LINK_LIBRARIES})
   
   # Remove the usual 'lib' prefix to make it clear it is a plugin
   # and not a library to link against
   set_target_properties(${APPLICATION_TARGET_NAME} PROPERTIES PREFIX "")

   # on Apple platform, a "MODULE" library gets a ".so" extension
   # but the ITK factory mechanism looks for ".dylib"
   if (APPLE)
     set_target_properties(${APPLICATION_TARGET_NAME} PROPERTIES SUFFIX ".dylib")
   endif()

   # When called from the OTB build system, use OTB_INSTALL_APP_DIR_CM24
   if (NOT APPLICATION_INSTALL_PATH AND OTB_INSTALL_APP_DIR_CM24)
     set(APPLICATION_INSTALL_PATH ${OTB_INSTALL_APP_DIR_CM24})
   endif()
   
   if (APPLICATION_INSTALL_PATH)
     install(TARGETS ${APPLICATION_TARGET_NAME}
             LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH})
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
        set(SCRIPT_CLI_INSTALLABLE ${APPLICATION_BINARY_PATH}/otbcli_${APPLICATION_NAME}.bat)
      else()
        set(SCRIPT_CLI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbcli_app.sh.in)
        set(SCRIPT_CLI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbcli_${APPLICATION_NAME})
        set(SCRIPT_CLI_INSTALLABLE ${APPLICATION_BINARY_PATH}/otbcli_${APPLICATION_NAME})
      endif()
      
      if (EXISTS ${SCRIPT_CLI_SOURCE})
          # Generate a script in the build dir, next to the cli launcher
          configure_file( ${SCRIPT_CLI_SOURCE}
                          ${SCRIPT_CLI_INTERMEDIATE}
                          @ONLY )
          
          # Copy it next to the application shared lib, and give executable rights
          file(COPY ${SCRIPT_CLI_INTERMEDIATE}
               DESTINATION ${APPLICATION_BINARY_PATH}
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)  
    
          if (OTB_INSTALL_BIN_DIR_CM24)
              # Install a version of this script if we are inside the OTB build
              install(PROGRAMS ${SCRIPT_CLI_INSTALLABLE}
                      DESTINATION ${OTB_INSTALL_BIN_DIR_CM24}
                      COMPONENT RuntimeLibraries)
          endif()
      endif()

      if (WIN32)
        set(SCRIPT_GUI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbgui_app.bat.in)
        set(SCRIPT_GUI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbgui_${APPLICATION_NAME}.bat)
        set(SCRIPT_GUI_INSTALLABLE ${APPLICATION_BINARY_PATH}/otbgui_${APPLICATION_NAME}.bat)
      else()
        set(SCRIPT_GUI_SOURCE ${OTB_SOURCE_DIR}/CMake/otbgui_app.sh.in)
        set(SCRIPT_GUI_INTERMEDIATE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/otbgui_${APPLICATION_NAME})
        set(SCRIPT_GUI_INSTALLABLE ${APPLICATION_BINARY_PATH}/otbgui_${APPLICATION_NAME})
      endif()

      if (EXISTS ${SCRIPT_GUI_SOURCE})
          # Generate a script in the build dir, next to the cli launcher
          configure_file( ${SCRIPT_GUI_SOURCE}
                          ${SCRIPT_GUI_INTERMEDIATE}
                          @ONLY )
          
          # Copy it next to the application shared lib, and give executable rights
          file(COPY ${SCRIPT_GUI_INTERMEDIATE}
               DESTINATION ${APPLICATION_BINARY_PATH}
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)  

          if (OTB_INSTALL_BIN_DIR_CM24)
              # Install a version of this script if we are inside the OTB build
              install(PROGRAMS ${SCRIPT_GUI_INSTALLABLE}
                      DESTINATION ${OTB_INSTALL_BIN_DIR_CM24})
          endif()
      endif()
   #endif(NOT WIN32)
   
   list(APPEND OTB_APPLICATIONS_NAME_LIST ${APPLICATION_NAME})
   set(OTB_APPLICATIONS_NAME_LIST ${OTB_APPLICATIONS_NAME_LIST}
       CACHE STRING "List of all applications" FORCE)

endmacro(OTB_CREATE_APPLICATION)





macro(OTB_TEST_APPLICATION)
  OTB_PARSE_ARGUMENTS(
      TESTAPPLICATION
      "NAME;APP;OPTIONS;TESTENVOPTIONS;VALID"
      ""
      ${ARGN})

   if (NOT TESTAPPLICATION_TESTENVOPTIONS)
     add_test(NAME ${TESTAPPLICATION_NAME}
              COMMAND otbTestDriver
              ${TESTAPPLICATION_VALID}
              Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
              ${TESTAPPLICATION_APP}
              $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
              ${TESTAPPLICATION_OPTIONS}
              -testenv)
   else (NOT TESTAPPLICATION_TESTENVOPTIONS)
     add_test(NAME ${TESTAPPLICATION_NAME}
              COMMAND otbTestDriver
              ${TESTAPPLICATION_VALID}
              Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
              ${TESTAPPLICATION_APP}
              $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
              ${TESTAPPLICATION_OPTIONS}
              -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
   endif (NOT TESTAPPLICATION_TESTENVOPTIONS)
   
endmacro(OTB_TEST_APPLICATION)
