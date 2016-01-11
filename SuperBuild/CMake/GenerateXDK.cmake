
message(STATUS "Setup OTB xdk...")

set(PACKAGE_VERSION xdk)
set(PACKAGE_NAME OTB)
set(PACKAGE_LONG_NAME OrfeoToolBox)

set(PACKAGE_ARCH Linux32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PACKAGE_ARCH Linux64)
endif()

set(ARCHIVE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION}-${PACKAGE_ARCH})

set(MAKESELF_SCRIPT ${CMAKE_BINARY_DIR}/GENERATE-XDK/build/makeself.sh)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/GENERATE-XDK/build")

file(WRITE "${CMAKE_BINARY_DIR}/GENERATE-XDK/src/GENERATE-XDK/CMakeLists.txt"
    "cmake_minimum_required(VERSION 2.6)
       include(CMakeParseArguments)
       set(CMAKE_INSTALL_PREFIX \"${SB_INSTALL_PREFIX}\")
       set(DOWNLOAD_LOCATION \"${DOWNLOAD_LOCATION}\")
       set(SETUP_SCRIPT_SRC ${CMAKE_SOURCE_DIR}/CMake/pkgsetup.in)
       set(WITH_OTBGUI OFF)
       set(MAKE_XDK ON)
       include(${CMAKE_SOURCE_DIR}/CMake/External_pkgtools.cmake)
       include(${CMAKE_SOURCE_DIR}/CMake/Package_Macro.cmake)
       superbuild_package(
       OUTDIR \"${ARCHIVE_NAME}\"
       INSTALLDIR \"${SB_INSTALL_PREFIX}\"
       PEFILES \"${PEFILES}\"
       SEARCHDIRS \"\")")

  add_custom_target(GENERATE-XDK-configure
    COMMAND ${CMAKE_COMMAND}
    "${CMAKE_BINARY_DIR}/GENERATE-XDK/src/GENERATE-XDK"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/GENERATE-XDK/build" )

  add_custom_target(GENERATE-XDK-build
    COMMAND ${CMAKE_COMMAND}
    "--build" "${CMAKE_BINARY_DIR}/GENERATE-XDK/build" "--target" "install"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/GENERATE-XDK/build"
    DEPENDS GENERATE-XDK-configure)

  add_custom_target(GENERATE-XDK
    COMMAND ${MAKESELF_SCRIPT} "--target" "${ARCHIVE_NAME}" "${SB_INSTALL_PREFIX}/${ARCHIVE_NAME}" "${ARCHIVE_NAME}.run" "${PACKAGE_LONG_NAME} ${PACKAGE_VERSION}" "./pkgsetup"
    DEPENDS GENERATE-XDK-build)

  add_custom_target(GENERATE-XDK-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/GENERATE-XDK"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.run"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" )
