
message(STATUS "Setup OTB Packaging...")

set(PACKAGE_VERSION)

if(EXISTS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt")
  file(STRINGS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt" _otb_version_vars   REGEX "set\\\(OTB_VERSION_")
  file(WRITE  ${CMAKE_BINARY_DIR}/CMakeFiles/version_vars.cmake "#OTB version\n")
  foreach(_otb_version_var ${_otb_version_vars})
    file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/version_vars.cmake "${_otb_version_var}\n")
  endforeach()
  include(${CMAKE_BINARY_DIR}/CMakeFiles/version_vars.cmake)
  if(OTB_VERSION_STRING)
    set(PACKAGE_VERSION ${OTB_VERSION_STRING})
  else()
    message(FATAL_ERROR "Packaging: Cannot find OTB_VERSION_STRING!")
  endif()
else()
  message(FATAL_ERROR "Packaging: File '${CMAKE_SOURCE_DIR}/../CMakeLists.txt' does not exists")
endif()

set(PACKAGE_NAME OTB)
set(PACKAGE_LONG_NAME OrfeoToolBox)

set(PACKAGE_ARCH Linux32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PACKAGE_ARCH Linux64)
endif()

set(ARCHIVE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION}-${PACKAGE_ARCH})

set(MAKESELF_SCRIPT ${CMAKE_BINARY_DIR}/PACKAGE-OTB/build/makeself.sh)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/PACKAGE-OTB/build")

file(WRITE "${CMAKE_BINARY_DIR}/PACKAGE-OTB/src/PACKAGE-OTB/CMakeLists.txt"
    "cmake_minimum_required(VERSION 2.6)
       include(CMakeParseArguments)
       set(CMAKE_INSTALL_PREFIX \"${SB_INSTALL_PREFIX}\")
       set(DOWNLOAD_LOCATION \"${DOWNLOAD_LOCATION}\")
       set(SETUP_SCRIPT_SRC ${CMAKE_SOURCE_DIR}/CMake/pkgsetup.in)
       set(WITH_OTBGUI ${OTB_USE_QT4})
       set(WITH_MVD ${ENABLE_MONTEVERDI})
       include(${CMAKE_SOURCE_DIR}/CMake/External_pkgtools.cmake)
       include(${CMAKE_SOURCE_DIR}/CMake/Package_Macro.cmake)
       superbuild_package(
       OUTDIR \"${ARCHIVE_NAME}\"
       INSTALLDIR \"${SB_INSTALL_PREFIX}\"
       PEFILES \"${PEFILES}\"
       SEARCHDIRS \"\")")

  add_custom_target(PACKAGE-OTB-configure
    COMMAND ${CMAKE_COMMAND}
    "${CMAKE_BINARY_DIR}/PACKAGE-OTB/src/PACKAGE-OTB"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/PACKAGE-OTB/build" )

  add_custom_target(PACKAGE-OTB-build
    COMMAND ${CMAKE_COMMAND}
    "--build" "${CMAKE_BINARY_DIR}/PACKAGE-OTB/build" "--target" "install"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/PACKAGE-OTB/build"
    DEPENDS PACKAGE-OTB-configure)

  add_custom_target(PACKAGE-OTB
    COMMAND ${MAKESELF_SCRIPT} "--target" "${ARCHIVE_NAME}" "${SB_INSTALL_PREFIX}/${ARCHIVE_NAME}" "${ARCHIVE_NAME}.run" "${PACKAGE_LONG_NAME} ${PACKAGE_VERSION}" "./pkgsetup"
    DEPENDS PACKAGE-OTB-build)

  add_custom_target(PACKAGE-OTB-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-OTB"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.run"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" )
