include(OTBParseArguments)

macro(OTB_CREATE_APPLICATION)
  OTB_PARSE_ARGUMENTS(
      APPLICATION
      "NAME;SOURCES;INCLUDE_DIRS;LINK_LIBRARIES;INSTALL_PATH"
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

   # Install only if a path has been provided
   if (APPLICATION_INSTALL_PATH)
     install(TARGETS ${APPLICATION_TARGET_NAME}
             LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH})
   endif()
   
endmacro(OTB_CREATE_APPLICATION)
