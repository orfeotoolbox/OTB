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
if(OUT_OF_SOURCE_BUILD)
  if(BUILD_TESTING)
    include(CTest)
  endif()
  set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
  option(GENERATE_XDK "Generate XDK for OTB development" OFF)
  option(ENABLE_MONTEVERDI "enable monteverdi" ON)
  set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild binary directory")
  set(SB_INSTALL_PREFIX "" CACHE PATH "path to superbuild install directory")
else()
  set(SUPERBUILD_BINARY_DIR ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_INSTALL_PREFIX "${SB_INSTALL_PREFIX}")

set(PACKAGE_NAME OTB)
set(PACKAGE_LONG_NAME OrfeoToolBox)
if(UNIX)
  if(APPLE)
    set(PACKAGE_PLATFORM_NAME "Darwin")
  else()
    set(PACKAGE_PLATFORM_NAME "Linux")
  endif()
endif()
set(PACKAGE_ARCH 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PACKAGE_ARCH 64)
endif()

get_filename_component(OTB_SUPERBUILD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH)
get_filename_component(OTB_SOURCE_DIR ${OTB_SUPERBUILD_SOURCE_DIR} PATH)

include(version_macro.cmake)
get_version("${OTB_SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake" "ITK_SB_VERSION" ITK_SB_VERSION)
get_version("${OTB_SOURCE_DIR}/CMakeLists.txt" "OTB_VERSION_" OTB_VERSION_STRING)


set(PACKAGE_LIST OTB)
if(GENERATE_XDK)
  list(APPEND PACKAGE_LIST XDK)
endif()

include(${OTB_SUPERBUILD_SOURCE_DIR}/CMake/External_pkgtools.cmake)

foreach(PACKAGE ${PACKAGE_LIST})
  #message( "-- To get ${PACKAGE} package. Run 'make PACKAGE-${PACKAGE}'")
  message( "-- Configuring ${PACKAGE} package")

  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/${PACKAGE}-PACKAGE)

  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build")

  #Easy way to have the write "PACKAGE_PROJECT_DIR/src/PACKAGE-${PACKAGE}/CMakeLists.txt"
  #inside foreach()
  if("${PACKAGE}" STREQUAL "XDK")
    set(PACKAGE_PLATFORM_NAME "xdk-${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PKG_GENERATE_XDK OFF)
  endif()

  set(ARCHIVE_NAME ${PACKAGE_NAME}-${get_version_OTB_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})

  file(WRITE "${PACKAGE_PROJECT_DIR}/src/PACKAGE-${PACKAGE}/CMakeLists.txt"
    "cmake_minimum_required(VERSION 2.6)
       include(CMakeParseArguments)
       set(PKG_INSTALL_PREFIX \"${SB_INSTALL_PREFIX}\")
       set(CMAKE_INSTALL_PREFIX \"${SB_INSTALL_PREFIX}\")
       set(DOWNLOAD_LOCATION \"${DOWNLOAD_LOCATION}\")
       set(PACKAGE_SUPPORT_FILES_DIR \"${CMAKE_CURRENT_SOURCE_DIR}\")
       set(Monteverdi_SOURCE_DIR \"${SUPERBUILD_BINARY_DIR}/MVD/src/MVD\")
       set(Monteverdi_BINARY_DIR \"${SUPERBUILD_BINARY_DIR}/MVD/build\")
       set(OTB_INSTALL_DIR \"${SUPERBUILD_BINARY_DIR}/OTB/build\")
       set(QT_PLUGINS_DIR \"${SB_INSTALL_PREFIX}/plugins\")
       set(QT_TRANSLATIONS_DIR \"${SB_INSTALL_PREFIX}/translations\")
       set(ITK_VERSION_STRING \"${get_version_ITK_SB_VERSION}\")
       set(PKG_GENERATE_XDK ${PKG_GENERATE_XDK})
       include(\"${OTB_SUPERBUILD_SOURCE_DIR}/CMake/Package_Macro.cmake\")
       include(\"${OTB_SUPERBUILD_SOURCE_DIR}/CMake/PackageHelper.cmake\")
       superbuild_package(STAGE_DIR \"${ARCHIVE_NAME}\")")

  #we have a dependency to PACKAGE_TOOLS always. check External_pkgtools.cmake
  set(PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS PACKAGE-TOOLS)

  #if we are building packages seperately we need to check OTB and MVD buil
  if(OUT_OF_SOURCE_BUILD)
    add_custom_target(PACKAGE-${PACKAGE}-check-otb
      COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}/OTB/build"
      WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}/OTB/build"
      )
    if(ENABLE_MONTEVERDI)
      add_custom_target(PACKAGE-${PACKAGE}-check-monteverdi
        COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}/MVD/build"
        WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}/MVD/build"
        DEPENDS PACKAGE-${PACKAGE}-check-otb
        )
      list(APPEND
        PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS
        PACKAGE-${PACKAGE}-check-monteverdi)
    else(ENABLE_MONTEVERDI)
      #just add only check-otb target to extra depends
      list(APPEND
        PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS
        PACKAGE-${PACKAGE}-check-otb)
    endif(ENABLE_MONTEVERDI)
  else(OUT_OF_SOURCE_BUILD)
    list(APPEND
      PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS
      OTB)
    if(ENABLE_MONTEVERDI)
      list(APPEND
        PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS
        MVD)
    endif(ENABLE_MONTEVERDI)
  endif(OUT_OF_SOURCE_BUILD)

  set(PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS)
  add_custom_target(PACKAGE-${PACKAGE}-configure
    COMMAND ${CMAKE_COMMAND}
    "${PACKAGE_PROJECT_DIR}/src/PACKAGE-${PACKAGE}"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS
    ${PACKAGE-${PACKAGE}-configure_EXTRA_DEPENDS}
    )

  add_custom_target(PACKAGE-${PACKAGE}-build
    COMMAND ${CMAKE_COMMAND}
    "--build" "${PACKAGE_PROJECT_DIR}/build" "--target" "install"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS PACKAGE-${PACKAGE}-configure
    )

  add_custom_target(PACKAGE-${PACKAGE}
    ALL DEPENDS
    COMMAND
    ${MAKESELF_SCRIPT}
    "--target"
    "${ARCHIVE_NAME}"
    "${SB_INSTALL_PREFIX}/${ARCHIVE_NAME}"
    "${ARCHIVE_NAME}.run"
    "${PACKAGE_LONG_NAME} ${get_version_OTB_VERSION_STRING}"
    "./pkgsetup"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS PACKAGE-${PACKAGE}-build
    )

  add_custom_target(PACKAGE-${PACKAGE}-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${PACKAGE}-PACKAGE"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.run"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

endforeach()