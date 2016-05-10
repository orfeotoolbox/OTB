if(NOT __EXTERNAL_PACKAGE_TOOLS__)
set(__EXTERNAL_PACKAGE_TOOLS__ 1)

set(PKGTOOLS_SB_PREFIX_DIR "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS")

include(ExternalProject)

if(WIN32)
  ExternalProject_Add(PACKAGE-TOOLS
    PREFIX "${PKGTOOLS_SB_PREFIX_DIR}"
    URL "https://www.orfeo-toolbox.org/packages/qt4-native-tools-win64.zip"
    URL_MD5 0e4bfd5677eb63ae691f4615a4338490
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CONFIGURE_COMMAND  ""
    BUILD_COMMAND      ""
    INSTALL_COMMAND    ""
    )
endif()


if(UNIX AND NOT WIN32)
  if(APPLE)
    add_custom_target(PATCHELF)
  else()
    ExternalProject_Add(PATCHELF
      PREFIX "${PKGTOOLS_SB_PREFIX_DIR}"
      URL "http://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2"
      URL_MD5 d02687629c7e1698a486a93a0d607947
      DOWNLOAD_DIR   ${DOWNLOAD_LOCATION}
      SOURCE_DIR "${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf"
      BINARY_DIR "${PKGTOOLS_SB_PREFIX_DIR}/build-patchelf"
      TMP_DIR  "${PKGTOOLS_SB_PREFIX_DIR}/tmp-patchelf"
      STAMP_DIR "${PKGTOOLS_SB_PREFIX_DIR}/stamp-patchelf"
      PATCH_COMMAND  ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/tools
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf
      ./configure --prefix ${CMAKE_INSTALL_PREFIX}/tools
      BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf ${CMAKE_MAKE_PROGRAM}
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf/src/patchelf ${CMAKE_INSTALL_PREFIX}/tools
      )
  endif(APPLE)

  ExternalProject_Add(MAKESELF
    PREFIX "${PKGTOOLS_SB_PREFIX_DIR}"
    URL "https://www.orfeo-toolbox.org/packages/makeself-2.2.0.tar.gz"
    URL_MD5 3c61df934b0c61ddcd7bd63b391e951d
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    SOURCE_DIR "${PKGTOOLS_SB_PREFIX_DIR}/src-makeself"
    BINARY_DIR "${PKGTOOLS_SB_PREFIX_DIR}/build-makeself"
    TMP_DIR  "${PKGTOOLS_SB_PREFIX_DIR}/tmp-makeself"
    STAMP_DIR "${PKGTOOLS_SB_PREFIX_DIR}/stamp-makeself"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_COMMAND}
    -E copy
    ${PKGTOOLS_SB_PREFIX_DIR}/src-makeself/makeself-header.sh
    ${PKGTOOLS_SB_PREFIX_DIR}/makeself-header.sh
    INSTALL_COMMAND ${CMAKE_COMMAND}
    -E copy
    ${PKGTOOLS_SB_PREFIX_DIR}/src-makeself/makeself.sh
    ${PKGTOOLS_SB_PREFIX_DIR}/makeself.sh
    DEPENDS PATCHELF
    )

  set(MAKESELF_SCRIPT ${PKGTOOLS_SB_PREFIX_DIR}/makeself.sh)

  add_custom_target(PACKAGE-TOOLS DEPENDS MAKESELF)

endif(UNIX AND NOT WIN32)

endif() #idef guard
