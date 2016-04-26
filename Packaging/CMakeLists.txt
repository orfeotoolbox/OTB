# cpack has a built-in target called 'package'. In our case we create two standalone installers with
# and uses a custom target named packages.

# stop generation of msvc packages for windows. 04-apr-2016. point OTB.

##### check if standalone project ######
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.6)
  include(CMakeParseArguments)
  project(OTBPackaging)
endif()

set(OUT_OF_SOURCE_BUILD FALSE)
if(PROJECT_NAME STREQUAL "OTBPackaging")
  set(OUT_OF_SOURCE_BUILD TRUE)
endif()

option(GENERATE_XDK "Generate XDK for OTB development" OFF)
set(MXE_TARGET_DIR "" CACHE PATH "path to mxe directory")
set(OTB_SOURCE_DIR "" CACHE PATH "path to otb source directory")

if(OUT_OF_SOURCE_BUILD)
  if(BUILD_TESTING)
    include(CTest)
  endif()
endif()

#used in PackageHelper.cmake (don't change)
set(PKG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

set(PACKAGE_NAME OTB)
set(PACKAGE_VERSION_STRING ${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH})

#if(CMAKE_CROSSCOMPILING)

if(NOT MXE_TARGET_DIR)
  message(FATAL_ERROR "MXE_TARGET_DIR is missing")
endif()

if(NOT OTB_SOURCE_DIR)
  message(FATAL_ERROR "OTB_SOURCE_DIR is missing")
endif()

find_program(ZIP_EXECUTABLE zip)
if(NOT ZIP_EXECUTABLE)
  message(FATAL_ERROR "Cannot find zip executable. Please add it to your path")
endif()
##### End of checking required tools and platform. (cross-compile with zip)

#define the name of final archive based on build architecture. win32 or win64?
set(PACKAGE_PLATFORM_NAME win)
if(MXE_TARGET_DIR MATCHES "i686")
  set(MXE_ARCH "i686")
  set(PACKAGE_ARCH "32")
elseif(MXE_TARGET_DIR MATCHES "x86_64")
  set(MXE_ARCH "x86_64")
  set(PACKAGE_ARCH "64") #helper for ARCHIVE_NAME
endif()

get_filename_component(MXE_MXEROOT ${MXE_TARGET_DIR} PATH)
get_filename_component(MXE_MXEROOT ${MXE_MXEROOT} PATH)

set(PACKAGE_LIST OTB)
if(GENERATE_XDK)
  list(APPEND PACKAGE_LIST XDK)
endif()

foreach(PACKAGE ${PACKAGE_LIST})
  #message( "-- To get ${PACKAGE} package. Run 'make PACKAGE-${PACKAGE}'")
  message( "-- Configuring ${PACKAGE} package")
  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/${PACKAGE}-PACKAGE)
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/_build")
  #Easy way to have the write "PACKAGE_PROJECT_DIR/src/PACKAGE-${PACKAGE}/CMakeLists.txt"
  #inside foreach()
  if("${PACKAGE}" STREQUAL "XDK")
    set(PACKAGE_PLATFORM_NAME "xdk-${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PKG_GENERATE_XDK OFF)
  endif()

  set(ARCHIVE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})

  #empty build directory
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PKG_INSTALL_PREFIX}/${ARCHIVE_NAME}")

  file(WRITE "${PACKAGE_PROJECT_DIR}/CMakeLists.txt"
  "cmake_minimum_required(VERSION 2.6)
       include(CMakeParseArguments)
       include(CMakeDetermineSystem)
       set(Monteverdi_SOURCE_DIR \"${Monteverdi_SOURCE_DIR}\")
       set(PACKAGE_SUPPORT_FILES_DIR \"${Monteverdi_SOURCE_DIR}/Packaging\")
       set(OTB_MODULES_DIR \"${OTB_MODULES_DIR}\")
       set(QT_PLUGINS_DIR \"${QT_PLUGINS_DIR}\")
       set(CMAKE_CROSSCOMPILING ON)
       set(QT_TRANSLATIONS_DIR \"${QT_TRANSLATIONS_DIR}\")
       set(Monteverdi_BINARY_DIR \"${CMAKE_BINARY_DIR}\")
       set(CMAKE_INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}\")
       set(Monteverdi_INSTALL_DATA_DIR \"${Monteverdi_INSTALL_DATA_DIR}\")
       set(MXE_ARCH \"${MXE_ARCH}\")
       set(MXE_MXEROOT \"${MXE_MXEROOT}\")
       set(PKG_GENERATE_XDK ${GENERATE_XDK})
       set(PKG_INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}\")
       include(${OTB_SOURCE_DIR}/SuperBuild/CMake/PackageHelper.cmake)
       super_package(STAGE_DIR \"${ARCHIVE_NAME}\")" )

   add_custom_target(InstallProject-${PACKAGE}
     COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}" --target install
     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

   #custom targets to configure , build and create package.
   add_custom_target(configure-PACKAGE-${PACKAGE}
     COMMAND ${CMAKE_COMMAND}
     "${PACKAGE_PROJECT_DIR}"
     WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/_build"
     DEPENDS InstallProject-${PACKAGE})

   add_custom_target(build-PACKAGE-${PACKAGE}
     COMMAND ${CMAKE_COMMAND}
     "--build" "${PACKAGE_PROJECT_DIR}/_build" "--target" "install"
     WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/_build"
     DEPENDS configure-PACKAGE-${PACKAGE})

   add_custom_target(PACKAGE-${PACKAGE}
     COMMAND ${ZIP_EXECUTABLE} "-r" "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.zip" "${ARCHIVE_NAME}"
     WORKING_DIRECTORY "${PKG_INSTALL_PREFIX}"
     DEPENDS build-PACKAGE-${PACKAGE})
endforeach()