include(ParseArguments)

macro(OTB_CREATE_APPLICATION)
  PARSE_ARGUMENTS(
      APPLICATION
      "NAME;SOURCES;INCLUDE_DIRS;LINK_LIBRARIES"
      ""
      ${ARGN})

   set( APPLICATION_TARGET_NAME otbapp_${APPLICATION_NAME} )

   include_directories(${CMAKE_SOURCE_DIR}/Code/Core ${CMAKE_SOURCE_DIR}/Code/Common )
   include_directories(${APPLICATION_INCLUDE_DIRS})
   add_library(${APPLICATION_TARGET_NAME} MODULE ${APPLICATION_SOURCES})
   target_link_libraries(${APPLICATION_TARGET_NAME} OTBWrapperCore ${APPLICATION_LINK_LIBRARIES})
   set_target_properties(${APPLICATION_TARGET_NAME} PROPERTIES PREFIX "")

   if (APPLE)
     set_target_properties(${APPLICATION_TARGET_NAME} PROPERTIES SUFFIX ".dylib")
   endif()

   install(TARGETS ${APPLICATION_TARGET_NAME}
            RUNTIME DESTINATION lib/otbapp
            LIBRARY DESTINATION lib/otbapp)
endmacro(OTB_CREATE_APPLICATION)
