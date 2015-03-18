#################################################################################
# This was taken from the http://www.cmake.org/Wiki/CMakeMacroParseArguments
#################################################################################
MACRO(OSSIM_PARSE_ARGUMENTS prefix arg_names option_names)
  SET(DEFAULT_ARGS)
  FOREACH(arg_name ${arg_names})    
    SET(${prefix}_${arg_name})
  ENDFOREACH(arg_name)
  FOREACH(option ${option_names})
    SET(${prefix}_${option} FALSE)
  ENDFOREACH(option)

  SET(current_arg_name DEFAULT_ARGS)
  SET(current_arg_list)
  FOREACH(arg ${ARGN})            
    SET(larg_names ${arg_names})    
    LIST(FIND larg_names "${arg}" is_arg_name)                   
    IF (is_arg_name GREATER -1)
      SET(${prefix}_${current_arg_name} ${current_arg_list})
      SET(current_arg_name ${arg})
      SET(current_arg_list)
    ELSE (is_arg_name GREATER -1)
      SET(loption_names ${option_names})    
      LIST(FIND loption_names "${arg}" is_option)            
      IF (is_option GREATER -1)
	     SET(${prefix}_${arg} TRUE)
      ELSE (is_option GREATER -1)
	     SET(current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option GREATER -1)
    ENDIF (is_arg_name GREATER -1)
  ENDFOREACH(arg)
  SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(OSSIM_PARSE_ARGUMENTS)

##############################################################################################
# This was taken from http://www.cmake.org/Wiki/CMakeMacroListOperations#CAR_and_CDR
##############################################################################################
MACRO(OSSIM_CAR var)
  SET(${var} ${ARGV1})
ENDMACRO(OSSIM_CAR)

#############################################################################################
# This was taken from http://www.cmake.org/Wiki/CMakeMacroListOperations#CAR_and_CDR
#############################################################################################
MACRO(OSSIM_CDR var junk)
  SET(${var} ${ARGN})
ENDMACRO(OSSIM_CDR)

#################################################################################
#  MACRO: TODAYS_DATE
#  
#  DESCRIPTION:
#      MACRO FOR GETTING THE DATE AND TIME INFORMATION
#################################################################################
MACRO (TODAYS_DATE RESULT)

   set(TEMP_DATE "")
  
   IF (CMAKE_HOST_WIN32)
      IF(NOT EXISTS "${CMAKE_BINARY_DIR}/get_date.cmd")

      ###### OUTPUT DATE ROUTINE #####
      write_file("${CMAKE_BINARY_DIR}/get_date.cmd" "@echo off
      @REM Seamonkey's quick date batch (MMDDYYYY format)
      @REM Setups %date variable
      @REM First parses month, day, and year into mm , dd, yyyy formats and then combines to be MMDDYYYY

      @FOR /F \"TOKENS=1* DELIMS= \" %%A IN ('DATE/T') DO SET CDATE=%%B
      @FOR /F \"TOKENS=1,2 eol=/ DELIMS=/ \" %%A IN ('DATE/T') DO SET mm=%%B
      @FOR /F \"TOKENS=1,2 DELIMS=/ eol=/\" %%A IN ('echo %CDATE%') DO SET dd=%%B
      @FOR /F \"TOKENS=2,3 DELIMS=/ \" %%A IN ('echo %CDATE%') DO SET yyyy=%%B
      @SET CURRENT_DATE=%yyyy%%mm%%dd%
      @echo on
      @echo %CURRENT_DATE%")

      ENDIF(NOT EXISTS "${CMAKE_BINARY_DIR}/get_date.cmd")
 
      EXECUTE_PROCESS(COMMAND "cmake" "-E" "comspec" "${CMAKE_BINARY_DIR}/get_date.cmd"  OUTPUT_VARIABLE ${RESULT})
      string(REGEX REPLACE "\n|\r" "" ${RESULT} ${${RESULT}})
   ELSEIF(CMAKE_HOST_UNIX)
      EXECUTE_PROCESS(COMMAND "date" "+%Y%m%d" OUTPUT_VARIABLE ${RESULT})
      string(REGEX REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
      string(REGEX REPLACE "\n|\r" "" ${RESULT} ${${RESULT}})
   ELSE (WIN32)
      MESSAGE(SEND_ERROR "date not implemented")
      SET(${RESULT} 000000)
   ENDIF (CMAKE_HOST_WIN32)
ENDMACRO (TODAYS_DATE)

#################################################################################
#  MACRO: GET_SVN_REVISION
#  
#  DESCRIPTION:
#      MACRO FOR GETTING THE SVN revision for this build
#################################################################################
MACRO (GET_SVN_REVISION)
   FIND_PACKAGE(Subversion)
   IF(SUBVERSION_FOUND)
      Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
      # MESSAGE("Current revision is ${Project_WC_REVISION}")
      # Subversion_WC_LOG(${PROJECT_SOURCE_DIR} Project)
      # MESSAGE("Last changed log is ${Project_LAST_CHANGED_LOG}")
   ENDIF()
ENDMACRO(GET_SVN_REVISION)

MACRO(LINK_EXTERNAL TRGTNAME)
    FOREACH(LINKLIB ${ARGN})
        TARGET_LINK_LIBRARIES(${TRGTNAME} "${LINKLIB}" )
    ENDFOREACH(LINKLIB)
ENDMACRO(LINK_EXTERNAL TRGTNAME)

MACRO(LINK_INTERNAL TRGTNAME)
    IF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 4)
        TARGET_LINK_LIBRARIES(${TRGTNAME} ${ARGN})
    ELSE(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 4)
        FOREACH(LINKLIB ${ARGN})
            IF(MSVC AND OSSIM_MSVC_VERSIONED_DLL)
                #when using versioned names, the .dll name differ from .lib name, there is a problem with that:
                #CMake 2.4.7, at least seem to use PREFIX instead of IMPORT_PREFIX  for computing linkage info to use into projects,
                # so we full path name to specify linkage, this prevent automatic inferencing of dependencies, so we add explicit depemdencies
                #to library targets used
                TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${OUTPUT_LIBDIR}/${LINKLIB}${CMAKE_RELEASE_POSTFIX}.lib" debug "${OUTPUT_LIBDIR}/${LINKLIB}${CMAKE_DEBUG_POSTFIX}.lib")
                ADD_DEPENDENCIES(${TRGTNAME} ${LINKLIB})
            ELSE(MSVC AND OSSIM_MSVC_VERSIONED_DLL)
                TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${LINKLIB}${CMAKE_RELEASE_POSTFIX}" debug "${LINKLIB}${CMAKE_DEBUG_POSTFIX}")
            ENDIF(MSVC AND OSSIM_MSVC_VERSIONED_DLL)
        ENDFOREACH(LINKLIB)
    ENDIF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 4)
ENDMACRO(LINK_INTERNAL TRGTNAME)

######################################################################
#
# This set up the libraries to link to, it assumes there are two variable: one common for a group of examples or plugins
# kept in the variable TARGET_COMMON_LIBRARIES and an example or plugin specific kept in TARGET_ADDED_LIBRARIES 
# they are combined in a single list checked for unicity 
# the suffix ${CMAKE_DEBUG_POSTFIX} is used for differentiating optimized and debug
#
# a second variable TARGET_EXTERNAL_LIBRARIES hold the list of  libraries not differentiated between debug and optimized 
##################################################################################
MACRO(SETUP_LINK_LIBRARIES)
    SET(TARGET_LIBRARIES ${TARGET_COMMON_LIBRARIES})

    FOREACH(LINKLIB ${TARGET_ADDED_LIBRARIES})
      SET(TO_INSERT TRUE)
      FOREACH (value ${TARGET_COMMON_LIBRARIES})
            IF (${value} STREQUAL ${LINKLIB})
                  SET(TO_INSERT FALSE)
            ENDIF (${value} STREQUAL ${LINKLIB})
        ENDFOREACH (value ${TARGET_COMMON_LIBRARIES})
      IF(TO_INSERT)
          LIST(APPEND TARGET_LIBRARIES ${LINKLIB})
      ENDIF(TO_INSERT)
    ENDFOREACH(LINKLIB)

    LINK_INTERNAL(${TARGET_TARGETNAME} ${TARGET_LIBRARIES})
    TARGET_LINK_LIBRARIES(${TARGET_TARGETNAME} ${TARGET_EXTERNAL_LIBRARIES})
    IF(TARGET_LIBRARIES_VARS)
        LINK_WITH_VARIABLES(${TARGET_TARGETNAME} ${TARGET_LIBRARIES_VARS})
    ENDIF(TARGET_LIBRARIES_VARS)
ENDMACRO(SETUP_LINK_LIBRARIES)


MACRO(OSSIM_SETUP_APPLICATION)
   OSSIM_PARSE_ARGUMENTS(APPLICATION
			"COMPONENT_NAME;SOURCE_FILES;HEADERS;TARGET_NAME;TARGET_LABEL" 
                        "COMMAND_LINE;INSTALL;REQUIRE_WINMAIN_FLAG" 
                        ${ARGN})
   OSSIM_CAR(APPLICATION_NAME "${APPLICATION_DEFAULT_ARGS}")
   OSSIM_CDR(APPLICATION_SOURCES "${APPLICATION_DEFAULT_ARGS}")
   SET(TARGET_NAME ${APPLICATION_NAME})
   SET(TARGET_TARGETNAME "${TARGET_DEFAULT_PREFIX}${APPLICATION_NAME}")
   IF(APPLICATION_TARGET_NAME)
      set(TARGET_TARGETNAME "${APPLICATION_TARGET_NAME}")
   ENDIF(APPLICATION_TARGET_NAME)

   SET(TARGET_LABEL "${TARGET_DEFAULT_LABEL_PREFIX} ${APPLICATION_NAME}")
   IF(APPLICATION_TARGET_LABEL)
      SET(TARGET_LABEL "${APPLICATION_TARGET_LABEL}")
   ENDIF(APPLICATION_TARGET_LABEL)

   IF(APPLICATION_COMMAND_LINE)
        ADD_EXECUTABLE(${TARGET_TARGETNAME} ${APPLICATION_SOURCE_FILES} ${APPLICATION_HEADERS})
        
    ELSE(APPLICATION_COMMAND_LINE)
        IF(APPLE)
            # SET(MACOSX_BUNDLE_LONG_VERSION_STRING "${OSSIM_MAJOR_VERSION}.${OSSIM_MINOR_VERSION}.${OSSIM_PATCH_VERSION}")
            # Short Version is the "marketing version". It is the version
            # the user sees in an information panel.
            SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OSSIM_MAJOR_VERSION}.${OSSIM_MINOR_VERSION}.${OSSIM_PATCH_VERSION}")
            # Bundle version is the version the OS looks at.
            SET(MACOSX_BUNDLE_BUNDLE_VERSION "${OSSIM_MAJOR_VERSION}.${OSSIM_MINOR_VERSION}.${OSSIM_PATCH_VERSION}")
            SET(MACOSX_BUNDLE_GUI_IDENTIFIER "org.ossim.${TARGET_TARGETNAME}" )
            SET(MACOSX_BUNDLE_BUNDLE_NAME "${TARGET_TARGETNAME}" )
            # SET(MACOSX_BUNDLE_ICON_FILE "myicon.icns")
            # SET(MACOSX_BUNDLE_COPYRIGHT "")
            # SET(MACOSX_BUNDLE_INFO_STRING "Info string, localized?")
        ENDIF(APPLE)

        IF(WIN32)
            IF (APPLICATION_REQUIRE_WINMAIN_FLAG)
                SET(PLATFORM_SPECIFIC_CONTROL WIN32)
            ENDIF (APPLICATION_REQUIRE_WINMAIN_FLAG)
        ENDIF(WIN32)

        IF(APPLE)
            IF(OSSIM_BUILD_APPLICATION_BUNDLES)
                SET(PLATFORM_SPECIFIC_CONTROL MACOSX_BUNDLE)
            ENDIF(OSSIM_BUILD_APPLICATION_BUNDLES)
        ENDIF(APPLE)

        ADD_EXECUTABLE(${TARGET_TARGETNAME} ${PLATFORM_SPECIFIC_CONTROL} ${APPLICATION_SOURCE_FILES} ${APPLICATION_HEADERS})
        
    ENDIF(APPLICATION_COMMAND_LINE)


    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES DEBUG_OUTPUT_NAME "${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES RELEASE_OUTPUT_NAME "${TARGET_NAME}${CMAKE_RELEASE_POSTFIX}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${TARGET_NAME}${CMAKE_RELWITHDEBINFO_POSTFIX}")
    SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${TARGET_NAME}${CMAKE_MINSIZEREL_POSTFIX}")

    IF(MSVC_IDE AND OSSIM_MSVC_VERSIONED_DLL)
            SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES PREFIX "../")    
    ENDIF(MSVC_IDE AND OSSIM_MSVC_VERSIONED_DLL)
    

    SETUP_LINK_LIBRARIES() 

    IF(APPLICATION_INSTALL)  
        IF(APPLE) 
            INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR} BUNDLE DESTINATION ${INSTALL_RUNTIME_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME})
        ELSE(APPLE)
            INSTALL(TARGETS ${TARGET_TARGETNAME} RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR} ${INSTALL_COMPONENT_INFO} COMPONENT ${APPLICATION_COMPONENT_NAME})
        ENDIF(APPLE)
    ENDIF(APPLICATION_INSTALL)

   SET_TARGET_PROPERTIES(${TARGET_TARGETNAME} PROPERTIES 
                              RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")    

      
ENDMACRO(OSSIM_SETUP_APPLICATION)

#####################################################################################################
# MACRO: OSSIM_LINK_LIBRARY
#
# Example: 
#     OSSIM_LINK_LIBRARY(ossim 
#                        COMPONENT_NAME ossim 
#                        SOURCE_FILES foo.cpp 
#                        HEADERS foo.h 
#                        TYPE SHARED
#                        LIBRARIES <list of libraries to link against>
#                        INSTALL_LIB
#                        INSTALL_HEADERS)
#
#    The INSTALL_LIB says to add a default install command for the library by default it will setup the following
#           install(TARGETS ossim
#               FRAMEWORK           DESTINATION         ${INSTALL_FRAMEWORK_DIR}
#               RUNTIME             DESTINATION         ${INSTALL_RUNTIME_DIR}
#               LIBRARY             DESTINATION         ${INSTALL_LIBRARY_DIR}
#               ARCHIVE             DESTINATION         ${INSTALL_ARCHIVE_DIR}
#               PUBLIC_HEADER       DESTINATION         ${INSTALL_INCLUDE_DIR} 
#               COMPONENT ossim)
#
#   The INSTALL_HEADERS will do a default header installation if the option is passed in
#        install(FILES <list of headers> DESTINATION "include/ossim" COMPONENT ossim)
#####################################################################################################
MACRO(OSSIM_LINK_LIBRARY)
   # The SO_VERSION and VERSION are here for override purpose only so other libraries with their own 
   # versioning scheme can use the sum linking
   #
   OSSIM_PARSE_ARGUMENTS(LINK
			"COMPONENT_NAME;SOURCE_FILES;HEADERS;TYPE;LIBRARIES;ADDITIONAL_COMPILE_FLAGS;SOVERSION;VERSION;PUBLIC_HEADERS"
                        "INSTALL_LIB;INSTALL_HEADERS;VERSION_SYMLINKS" 
                        ${ARGN})
   OSSIM_CAR(LINK_NAME "${LINK_DEFAULT_ARGS}")
   OSSIM_CDR(LINK_SOURCES "${LINK_DEFAULT_ARGS}")
   ADD_DEFINITIONS("${OSSIM_COMMON_COMPILER_FLAGS}")
   ADD_LIBRARY(${LINK_NAME}
               ${LINK_TYPE}
               ${LINK_HEADERS}
               ${LINK_SOURCE_FILES})
   IF(NOT LINK_PUBLIC_HEADERS)
      SET(LINK_PUBLIC_HEADERS ${LINK_HEADERS})
   ENDIF()
   IF(LINK_ADDITIONAL_COMPILE_FLAGS)
      SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES
                            COMPILE_FLAGS ${LINK_ADDITIONAL_COMPILE_FLAGS})
   ENDIF(LINK_ADDITIONAL_COMPILE_FLAGS)
   if(APPLE)
      IF(BUILD_SHARED_LIBS)
#        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#        SET(CMAKE_INSTALL_RPATH "${OSSIM_COMPILE_FRAMEWORKS_INSTALL_NAME_DIR}")
        IF(BUILD_OSSIM_FRAMEWORKS)
          SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                             FRAMEWORK TRUE
                             BUILD_WITH_INSTALL_RPATH ON 
                             INSTALL_NAME_DIR @executable_path/../Frameworks)
        ELSE(BUILD_OSSIM_FRAMEWORKS)
          SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                             FRAMEWORK FALSE
                             BUILD_WITH_INSTALL_RPATH OFF 
                             INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBRARY_DIR})
        ENDIF(BUILD_OSSIM_FRAMEWORKS)
      ELSE(BUILD_SHARED_LIBRARY)
          SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                                FRAMEWORK FALSE)
      ENDIF(BUILD_SHARED_LIBS)
   ENDIF(APPLE)
   IF(UNIX AND BUILD_SHARED_LIBS AND NOT APPLE)   
      IF(LINK_VERSION_SYMLINKS)
         IF(NOT LINK_SOVERSION)
             set(LINK_SOVERSION "${OSSIM_SOVERSION}")
         ENDIF(NOT LINK_SOVERSION)
         IF(NOT LINK_VERSION)
             set(LINK_VERSION "${OSSIM_VERSION}")
         ENDIF(NOT LINK_VERSION)
      # ADD_CUSTOM_TARGET( lib DEPENDS ${LINK_NAME} )
      # change lib_target properties
         SET_TARGET_PROPERTIES( ${LINK_NAME} PROPERTIES
                                    # create *nix style library versions + symbolic links
                                   VERSION ${LINK_VERSION}
                                   SOVERSION ${LINK_SOVERSION}
                                  # allow creating static and shared libs without conflicts
                                  CLEAN_DIRECT_OUTPUT 1
                                  # avoid conflicts between library and binary target names
                                  OUTPUT_NAME ${LINK_NAME} )
      ENDIF(LINK_VERSION_SYMLINKS)
   ENDIF(UNIX AND BUILD_SHARED_LIBS AND NOT APPLE)
   SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                              RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")    
   IF(APPLE AND BUILD_OSSIM_FRAMEWORKS)
     SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                              LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_FRAMEWORK_DIR}")    
   ELSE(APPLE AND BUILD_OSSIM_FRAMEWORKS)
     SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                              LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")    
   ENDIF(APPLE AND BUILD_OSSIM_FRAMEWORKS)
   SET_TARGET_PROPERTIES(${LINK_NAME} PROPERTIES 
                              ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")    

   TARGET_LINK_LIBRARIES(${LINK_NAME} ${LINK_LIBRARIES} ${${LINK_NAME}_EXTRA_LIBS})

   IF(LINK_INSTALL_LIB)
        IF(LINK_INSTALL_HEADERS)
           SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${LINK_PUBLIC_HEADERS}")
           install(TARGETS ${LINK_NAME}
                   FRAMEWORK           DESTINATION         ${INSTALL_FRAMEWORK_DIR} COMPONENT ${LINK_COMPONENT_NAME}
                   RUNTIME             DESTINATION         ${INSTALL_RUNTIME_DIR} COMPONENT ${LINK_COMPONENT_NAME}
                   LIBRARY             DESTINATION         ${INSTALL_LIBRARY_DIR} COMPONENT ${LINK_COMPONENT_NAME}
                   ARCHIVE             DESTINATION         ${INSTALL_ARCHIVE_DIR} COMPONENT ${LINK_COMPONENT_NAME}-dev
                   PUBLIC_HEADER       DESTINATION         ${INSTALL_INCLUDE_DIR} COMPONENT ${LINK_COMPONENT_NAME}-dev)
        ELSE(LINK_INSTALL_HEADERS)
           install(TARGETS ${LINK_NAME}
                   FRAMEWORK           DESTINATION         ${INSTALL_FRAMEWORK_DIR} COMPONENT ${LINK_COMPONENT_NAME}
                   RUNTIME             DESTINATION         ${INSTALL_RUNTIME_DIR} COMPONENT ${LINK_COMPONENT_NAME}
                   LIBRARY             DESTINATION         ${INSTALL_LIBRARY_DIR} COMPONENT ${LINK_COMPONENT_NAME}
                   ARCHIVE             DESTINATION         ${INSTALL_ARCHIVE_DIR} COMPONENT ${LINK_COMPONENT_NAME}-dev)
        ENDIF(LINK_INSTALL_HEADERS)
    ENDIF(LINK_INSTALL_LIB)
ENDMACRO(OSSIM_LINK_LIBRARY)

MACRO(OSSIM_ADD_COMMON_MAKE_UNINSTALL)
#   get_target_property(TEST_UNINSTALL uninstall CREATED)
#   IF(NOT TEST_UNINSTALL)
      #-----------------------------------------------------------------------------
      ### uninstall target
      #-----------------------------------------------------------------------------
      SET(OSSIM_CMAKE_UNINSTALL_CONFIG "${PROJECT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in")
      IF(EXISTS ${OSSIM_CMAKE_UNINSTALL_CONFIG})
         CONFIGURE_FILE(
           "${OSSIM_CMAKE_UNINSTALL_CONFIG}"
           "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
           IMMEDIATE @ONLY)
         ADD_CUSTOM_TARGET(uninstall
           "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
           )
      set_target_properties(uninstall PROPERTIES CREATED 1)
      ENDIF(EXISTS ${OSSIM_CMAKE_UNINSTALL_CONFIG})
#   ENDIF(NOT TEST_UNINSTALL)
ENDMACRO(OSSIM_ADD_COMMON_MAKE_UNINSTALL)

