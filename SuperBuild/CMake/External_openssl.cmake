set(proj OPENSSL)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup OpenSSL ...")

if(USE_SYSTEM_OPENSSL)
  find_package ( OPENSSL )
  add_custom_target(${proj})
  message(STATUS "  Using OpenSSL system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using OpenSSL SuperBuild version")
  
  # declare dependencies
  set(${proj}_DEPENDENCIES ZLIB)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj OPENSSL)

  set(OPENSSL_BUILD_ARCH "linux-x32")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OPENSSL_BUILD_ARCH "linux-x86_64")
  endif()
  
  if(MSVC)
    message(FATAL_ERROR "No build rules added yet.")  
  else(UNIX)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      DEPENDS ${${proj}_DEPENDENCIES}
      URL "http://openssl.org/source/openssl-1.0.2c.tar.gz"
      URL_MD5 8c8d81a9ae7005276e486702edbcd4b6
      BINARY_DIR ${OPENSSL_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      PATCH_COMMAND  ${CMAKE_COMMAND} -E copy_directory ${OPENSSL_SB_SRC} ${OPENSSL_SB_BUILD_DIR}
      CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir ${OPENSSL_SB_BUILD_DIR} ./Configure ${OPENSSL_BUILD_ARCH}
      --prefix=${SB_INSTALL_PREFIX} shared zlib zlib-dynamic -I${SB_INSTALL_PREFIX}/include -L${SB_INSTALL_PREFIX}/lib
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
      )
  endif()
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    message(FATAL_ERROR "No build rules added yet.")  
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libssl${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()




