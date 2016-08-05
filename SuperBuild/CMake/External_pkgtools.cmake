if( __EXTERNAL_PACKAGE_TOOLS__)
  return()
else()
  set(__EXTERNAL_PACKAGE_TOOLS__ 1)
endif()

if(WIN32)
  add_custom_target(PACKAGE-TOOLS)
  return()
endif()

if(UNIX AND NOT WIN32)

  include(ExternalProject)

  set(PKGTOOLS_SB_PREFIX_DIR "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS")

  if(APPLE)
    add_custom_target(PATCHELF)
  else()
    set(PATCHELF_INSTALL_DIR ${PKGTOOLS_SB_PREFIX_DIR}/install-patchelf)
    set(PATCHELF_PROGRAM ${PATCHELF_INSTALL_DIR}/patchelf)
    ExternalProject_Add(PATCHELF
      PREFIX "${PKGTOOLS_SB_PREFIX_DIR}"
      URL "http://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2"
      URL_MD5 d02687629c7e1698a486a93a0d607947
      DOWNLOAD_DIR   ${DOWNLOAD_LOCATION}
      SOURCE_DIR "${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf"
      BINARY_DIR "${PKGTOOLS_SB_PREFIX_DIR}/build-patchelf"
      TMP_DIR  "${PKGTOOLS_SB_PREFIX_DIR}/tmp-patchelf"
      STAMP_DIR "${PKGTOOLS_SB_PREFIX_DIR}/stamp-patchelf"
      PATCH_COMMAND  ${CMAKE_COMMAND} -E make_directory ${PKGTOOLS_SB_PREFIX_DIR}/install
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf
      ./configure --prefix ${PATCHELF_INSTALL_DIR}
      BUILD_COMMAND ${CMAKE_COMMAND} -E chdir ${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf ${CMAKE_MAKE_PROGRAM}
      INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${PKGTOOLS_SB_PREFIX_DIR}/src-patchelf/src/patchelf ${PATCHELF_PROGRAM}
      )
  endif(APPLE)

  set(MAKESELF_INSTALL_DIR ${PKGTOOLS_SB_PREFIX_DIR}/install-makeself)
  set(MAKESELF_SCRIPT ${MAKESELF_INSTALL_DIR}/makeself.sh)
  set(MAKESELF_HEADER_SCRIPT ${MAKESELF_INSTALL_DIR}/makeself-header.sh)

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
    BUILD_COMMAND ${CMAKE_COMMAND} -E copy
    ${PKGTOOLS_SB_PREFIX_DIR}/src-makeself/makeself-header.sh
    ${MAKESELF_HEADER_SCRIPT}
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
    ${PKGTOOLS_SB_PREFIX_DIR}/src-makeself/makeself.sh
    ${MAKESELF_SCRIPT}
    DEPENDS PATCHELF
    )

  add_custom_target(PACKAGE-TOOLS DEPENDS MAKESELF)

endif(UNIX AND NOT WIN32)
