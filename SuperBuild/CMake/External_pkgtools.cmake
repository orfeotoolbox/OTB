include(ExternalProject)
set(proj PACKAGE-TOOLS)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

set(PKGTOOLS_SB_PREFIX_DIR "${CMAKE_BINARY_DIR}/PACKAGE-OTB/${proj}")

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
  ${CMAKE_BINARY_DIR}/PACKAGE-OTB/build/makeself-header.sh
  INSTALL_COMMAND ${CMAKE_COMMAND}
  -E copy
  ${PKGTOOLS_SB_PREFIX_DIR}/src-makeself/makeself.sh
  ${CMAKE_BINARY_DIR}/PACKAGE-OTB/build/makeself.sh

  )

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

ExternalProject_Add( ${proj}
  PREFIX             "${PKGTOOLS_SB_PREFIX_DIR}"
  DOWNLOAD_COMMAND   ""
  CONFIGURE_COMMAND  ""
  BUILD_COMMAND      ""
  INSTALL_COMMAND    ""
  DEPENDS            MAKESELF PATCHELF)

endif()
