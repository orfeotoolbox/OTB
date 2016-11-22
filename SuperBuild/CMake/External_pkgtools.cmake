
set(PKGTOOLS_SB_PREFIX_DIR "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS")

set(PATCHELF_SOURCE_DIR  "${PKGTOOLS_SB_PREFIX_DIR}/src/patchelf")
# PATCHELF_BINARY_DIR is same as PATCHELF_SOURCE_DIR
set(PATCHELF_INSTALL_DIR "${PKGTOOLS_SB_PREFIX_DIR}/install/patchelf")
set(PATCHELF_STAMP_DIR   "${PKGTOOLS_SB_PREFIX_DIR}/stamp/patchelf")
set(PATCHELF_TMP_DIR     "${PKGTOOLS_SB_PREFIX_DIR}/tmp/patchelf")

#This variable is referenced in CreateCMakeProjects.cmake
set(PATCHELF_PROGRAM "${PATCHELF_SOURCE_DIR}/src/patchelf")


set(MAKESELF_SOURCE_DIR  "${PKGTOOLS_SB_PREFIX_DIR}/src/makeself")
# MAKESELF_SOURCE_DIR is same as MAKESELF_BINARY_DIR and MAKESELF_INSTALL_DIR
set(MAKESELF_STAMP_DIR   "${PKGTOOLS_SB_PREFIX_DIR}/stamp/makeself")
set(MAKESELF_TMP_DIR     "${PKGTOOLS_SB_PREFIX_DIR}/tmp/makeself")

#This variable is referenced in CreateCMakeProjects.cmake
set(MAKESELF_SCRIPT "${MAKESELF_SOURCE_DIR}/makeself.sh")

if( __EXTERNAL_PACKAGE_TOOLS__)
  return()
else()
  set(__EXTERNAL_PACKAGE_TOOLS__ 1)
endif()

if(WIN32)
  add_custom_target(PACKAGE-TOOLS)
  return()
endif()

include(ExternalProject)


if(APPLE)
  add_custom_target(PATCHELF)
else()
  ExternalProject_Add(PATCHELF
    PREFIX "${PKGTOOLS_SB_PREFIX_DIR}"
    URL                 "http://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2"
    URL_MD5             d02687629c7e1698a486a93a0d607947
    DOWNLOAD_DIR        "${DOWNLOAD_LOCATION}"
    SOURCE_DIR          "${PATCHELF_SOURCE_DIR}"
    BINARY_DIR          "${PATCHELF_SOURCE_DIR}"
    INSTALL_DIR         "${PATCHELF_INSTALL_DIR}"
    STAMP_DIR           "${PATCHELF_STAMP_DIR}"
    TMP_DIR             "${PATCHELF_TMP_DIR}"
    CONFIGURE_COMMAND "./configure" "--prefix" "${PATCHELF_INSTALL_DIR}"
    INSTALL_COMMAND ""
    )
endif()

ExternalProject_Add(MAKESELF
  PREFIX            "${PKGTOOLS_SB_PREFIX_DIR}"
  URL               "https://www.orfeo-toolbox.org/packages/makeself-2.2.0.tar.gz"
  URL_MD5           3c61df934b0c61ddcd7bd63b391e951d
  DOWNLOAD_DIR      "${DOWNLOAD_LOCATION}"
  SOURCE_DIR        "${MAKESELF_SOURCE_DIR}"
  BINARY_DIR        "${MAKESELF_SOURCE_DIR}"
  INSTALL_DIR       "${MAKESELF_SOURCE_DIR}"
  STAMP_DIR         "${MAKESELF_STAMP_DIR}"
  TMP_DIR           "${MAKESELF_TMP_DIR}"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  DEPENDS PATCHELF
  )

add_custom_target(PACKAGE-TOOLS DEPENDS MAKESELF)
