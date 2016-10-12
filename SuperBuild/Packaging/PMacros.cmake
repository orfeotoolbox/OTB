macro(macro_setup_cmake_project pkg)

  message( "-- Configuring ${pkg} package")

  include(${SUPERBUILD_SOURCE_DIR}/CMake/External_pkgtools.cmake)

  #reset it again in macro(macro_create_targets_for_package pkg)
  #because that's the cmake macros way.
  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/PACKAGE-${pkg})
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build")

  if("${pkg}" STREQUAL "XDK")
    set(archive_name ${PACKAGE_NAME}-${PKG_OTB_VERSION_STRING}-xdk-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  else()
    set(archive_name ${PACKAGE_NAME}-${PKG_OTB_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  endif()

  if("${pkg}" STREQUAL "XDK")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PKG_GENERATE_XDK OFF)
  endif()

  set(ConfigureMonteverdi_H ${OTB_BINARY_DIR}/Modules/Visualization/MonteverdiCore/ConfigureMonteverdi.h)
  if(NOT EXISTS ${ConfigureMonteverdi_H})
    message(FATAL_ERROR "${ConfigureMonteverdi_H} does not exists. Cannot continue")
  endif()

  file(STRINGS "${ConfigureMonteverdi_H}" ConfigureMonteverdi_H_CONTENTS REGEX "^#define.Monteverdi_INSTALL_DATA_DIR")
  string(REGEX REPLACE
    "^#define.Monteverdi_INSTALL_DATA_DIR" ""
    ConfigureMonteverdi_H_CONTENTS
    ${ConfigureMonteverdi_H_CONTENTS} )

  if(NOT ConfigureMonteverdi_H_CONTENTS)
    message(FATAL_ERROR "parse error in ${ConfigureMonteverdi_H}. Cannot continue")
  endif()

  string(REGEX REPLACE "\"" "" PKG_OTB_INSTALL_DATA_DIR "${ConfigureMonteverdi_H_CONTENTS}")

  if(NOT PKG_OTB_INSTALL_DATA_DIR)
    message(FATAL_ERROR "parse error in ${ConfigureMonteverdi_H_CONTENTS}. Cannot continue")
  endif()

  string(STRIP "${PKG_OTB_INSTALL_DATA_DIR}" PKG_OTB_INSTALL_DATA_DIR)
    
  #set archive name inside loop
  file(WRITE "${PACKAGE_PROJECT_DIR}/src/CMakeLists.txt"
  "cmake_minimum_required(VERSION 3.2)
   include(CMakeParseArguments)
   include(CMakeDetermineSystem)
   set(CMAKE_BUILD_TYPE             Release)
   set(PACKAGE_OTB_SRC_DIR               \"${PACKAGE_OTB_SRC_DIR}\")
   set(OTB_BINARY_DIR               \"${OTB_BINARY_DIR}\")
   set(OTB_INSTALL_DIR              \"${OTB_INSTALL_DIR}\")
   set(OTB_INSTALL_DATA_DIR         \"${PKG_OTB_INSTALL_DATA_DIR}\")
   set(QT_PLUGINS_DIR               \"${QT_PLUGINS_DIR}\")
   set(QT_TRANSLATIONS_DIR          \"${QT_TRANSLATIONS_DIR}\")
   set(DEPENDENCIES_INSTALL_DIR     \"${DEPENDENCIES_INSTALL_DIR}\")
   set(PACKAGE_SUPPORT_FILES_DIR    \"${PACKAGE_OTB_SRC_DIR}/SuperBuild/Packaging/Files\")

   set(CMAKE_INSTALL_PREFIX         \"${CMAKE_INSTALL_PREFIX}\")
   set(PKG_ITK_SB_VERSION           \"${PKG_ITK_SB_VERSION}\")
   set(PKG_OTB_VERSION_MAJOR        \"${PKG_OTB_VERSION_MAJOR}\")
   set(PKG_OTB_VERSION_MINOR        \"${PKG_OTB_VERSION_MINOR}\")
   set(PKG_OTB_VERSION_PATCH        \"${PKG_OTB_VERSION_PATCH}\")
   set(PKG_OTB_VERSION_STRING       \"${PKG_OTB_VERSION_STRING}\")
   set(PKG_GENERATE_XDK              ${PKG_GENERATE_XDK})
   set(PATCHELF_PROGRAM              ${PATCHELF_PROGRAM})
   set(PYTHON_EXECUTABLE             \"${PYTHON_EXECUTABLE}\")
   ${EXTRA_CACHE_CONFIG}
   include(${SUPERBUILD_SOURCE_DIR}/Packaging/PackageHelper.cmake)
   macro_super_package(STAGE_DIR \"${archive_name}\")"
  )

  if(UNIX)
    if(APPLE)
      set(README_FILE ${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst/Installation_Macx.txt)
    else() #not osx
      set(README_FILE ${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst/Installation_Linux.txt)
    endif() #if(APPLE)
  else() #windows
    set(README_FILE ${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst/Installation_Windows.txt)
  endif() #if(UNIX)

  configure_file(
    "${README_FILE}"
    ${PACKAGE_PROJECT_DIR}/src/README
    )

  macro_create_targets_for_package(${pkg})

endmacro()

macro(macro_create_targets_for_package pkg)

  if(WIN32 AND NOT MSVC)
    add_custom_target(PACKAGE-${pkg}-check
      COMMAND ${CMAKE_COMMAND} --build "." --target install
      WORKING_DIRECTORY "${OTB_BINARY_DIR}"
      )
  else() #Using SuperBuild
    #For out of source build,
    #we assume the otb is built correctly with superbuild
    if(OUT_OF_SOURCE_BUILD)
      add_custom_target(PACKAGE-${pkg}-check
        COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-${pkg}-check")
    else(OUT_OF_SOURCE_BUILD)
      add_custom_target(PACKAGE-${pkg}-check
        COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-${pkg}-check"
	      DEPENDS OTB
        WORKING_DIRECTORY ${SUPERBUILD_BINARY_DIR}
        )      
    endif(OUT_OF_SOURCE_BUILD)
  endif()

  add_dependencies(PACKAGE-${pkg}-check PACKAGE-TOOLS)

  if("${pkg}" STREQUAL "XDK")
    set(PACKAGE_PLATFORM_NAME_ "xdk-${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PACKAGE_PLATFORM_NAME_ "${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK OFF)
  endif()

  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/PACKAGE-${pkg})
  if("${pkg}" STREQUAL "XDK")
    set(archive_name ${PACKAGE_NAME}-${PKG_OTB_VERSION_STRING}-xdk-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  else()
    set(archive_name ${PACKAGE_NAME}-${PKG_OTB_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  endif()

  #configure
  add_custom_target(PACKAGE-${pkg}-configure
  COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build"
   WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}"
    COMMAND ${CMAKE_COMMAND} "-G${CMAKE_GENERATOR}"
    "${PACKAGE_PROJECT_DIR}/src"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS PACKAGE-${pkg}-check
    )

  #build
  add_custom_target(PACKAGE-${pkg}-build
    COMMAND ${CMAKE_COMMAND}
    "--build" "${PACKAGE_PROJECT_DIR}/build" "--target" "install"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS PACKAGE-${pkg}-configure
    )

  #create package
  # creation of package is different from windows and unix like
  if(WIN32)
    add_custom_target(PACKAGE-${pkg}
      COMMAND ${ZIP_EXECUTABLE} "a" "-r" "-y"
      "${CMAKE_BINARY_DIR}/${archive_name}.zip" "${archive_name}/*"
      WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
      DEPENDS PACKAGE-${pkg}-build
      COMMENT "Creating ${CMAKE_BINARY_DIR}/${archive_name}.zip"
      )
  else()
    add_custom_target(PACKAGE-${pkg}
      COMMAND ${MAKESELF_SCRIPT}
      "--quiet"
      "--target"
      "${archive_name}"
      "${CMAKE_INSTALL_PREFIX}/${archive_name}"
      "${archive_name}.run"
      "${PACKAGE_LONG_NAME} ${PKG_OTB_VERSION_STRING}"
      "./pkgsetup"
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
      DEPENDS PACKAGE-${pkg}-build
      COMMENT "Creating ${CMAKE_CURRENT_BINARY_DIR}/${archive_name}.run"
      )
  endif()

  set(PACKAGE_EXTENSION .run)
  if(WIN32)
    set(PACKAGE_EXTENSION .zip)
  endif()

  #clean
  add_custom_target(PACKAGE-${pkg}-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-${pkg}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${archive_name}${PACKAGE_EXTENSION}"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

endmacro(macro_create_targets_for_package)


# macro:  get_version
# args :
# INPUT_FILE     - Input cmake file where the version variable is set via cmake set() command.
#
# MATCH_STRINGS  - list of cmake variables that are set in INPUT_FILE.
#                - eg: set(OTB_VERSION_MAJOR "5") or set(OTB_VERSION_PATCH 85)
#                - It works on both qouted and non-quoted values.
#
# PREFIX         - optional prefix string where you need to have the output stored
#
# Sample usage:
#
# get_version(
# "INPUT_FILE ${SUPERBUILD_SOURCE_DIR}/../CMakeLists.txt"
# "MATCH_STRING OTB_VERSION_MAJOR, OTB_VERSION_MINOR"
#  PREFIX PKG)
# The above call will search set(OTB_VERSION_MAJOR [0-9]) and set(OTB_VERSION_MINOR [0-9]) 
# in the file CMakeLists.txt and if a match is found, it store the value of set command
# into a variable PKG_OTB_VERSION_MAJOR. Note that PKG is the prefix we given. If no
# prefix is set, it uses get_version. so the result variables will be
# get_version_OTB_VERSION_MAJOR and get_version_OTB_VERSION_MINOR

macro(get_version)
  cmake_parse_arguments(gv  "" "INPUT_FILE;PREFIX" "MATCH_STRINGS" ${ARGN} )
  
  if(NOT EXISTS "${gv_INPUT_FILE}")
    message(FATAL_ERROR "macro(get_version): File '${gv_INPUT_FILE}' does not exists")
  endif()

  if(NOT gv_PREFIX)
    set(gv_PREFIX "get_version")
  endif()
  
  file(STRINGS "${gv_INPUT_FILE}" gv_INPUT_FILE_CONTENTS)  
  foreach(gv_match_string ${gv_MATCH_STRINGS})
    foreach(gv_line ${gv_INPUT_FILE_CONTENTS})
      string(REGEX MATCH  "^set.${gv_match_string}\\ .*[0-9].*\\\)$" matched "${gv_line}")
      if(matched)
        string(REGEX REPLACE  "^set.${gv_match_string}."  "" gv_output_string "${matched}")
        string(REPLACE  "${gv_match_string}" "${gv_match_string}" gv_output_string ${gv_output_string})
        if(gv_output_string)
          string(REGEX REPLACE  "\\\)$" "" gv_output_string "${gv_output_string}")
          string(REGEX REPLACE "\"" "" gv_output_string "${gv_output_string}")
          string(STRIP "${gv_output_string}" gv_output_string)
        endif()
        set(${gv_PREFIX}_${gv_match_string}  "${gv_output_string}")
      endif()
    endforeach()
  endforeach()  
endmacro()
