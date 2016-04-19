if(NOT __EXTERNAL_GDAL__)
set(__EXTERNAL_GDAL__ 1)

if(USE_SYSTEM_GDAL)
  find_package ( GDAL )
  message(STATUS "  Using GDAL system version")
else()
  SETUP_SUPERBUILD(PROJECT GDAL)
  message(STATUS "  Using GDAL SuperBuild version")

  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(GDAL TIFF CURL GEOTIFF PNG JPEG OPENJPEG SQLITE GEOS ZLIB EXPAT)

  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL TIFF_ROOT     --with-libtiff)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL GEOTIFF_ROOT  --with-geotiff)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL PNG_ROOT      --with-png)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL JPEG_ROOT     --with-jpeg)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL OPENJPEG_ROOT --with-openjpeg)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL SQLITE_ROOT   --with-sqlite3)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL ZLIB_ROOT     --with-libz)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL EXPAT_ROOT    --with-expat)
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL CURL_ROOT     --with-curl "/bin/curl-config")
  ADD_SUPERBUILD_CONFIGURE_VAR(GDAL GEOS_ROOT     --with-geos "/bin/geos-config")

  if(UNIX)
    set(GDAL_SB_EXTRA_OPTIONS "" CACHE STRING "Extra options to be passed to GDAL configure script")
    mark_as_advanced(GDAL_SB_EXTRA_OPTIONS)

    #Convert GDAL_SB_EXTRA_OPTIONS to a list to allow to add multiple instructions to the CONFIGURE_COMMAND
    separate_arguments(GDAL_SB_EXTRA_OPTIONS)

    ExternalProject_Add(GDAL
      PREFIX GDAL
      URL "http://download.osgeo.org/gdal/1.11.2/gdal-1.11.2.tar.gz"
      URL_MD5 866a46f72b1feadd60310206439c1a76
      SOURCE_DIR ${GDAL_SB_SRC}
      BINARY_DIR ${GDAL_SB_SRC}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      DEPENDS ${GDAL_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E touch ${GDAL_SB_SRC}/config.rpath
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/GDAL/GNUmakefile ${GDAL_SB_SRC}/swig/python/GNUmakefile
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/patches/GDAL/S2_patch ${GDAL_SB_SRC}
      CONFIGURE_COMMAND
        # use 'env' because CTest launcher doesn't perform shell interpretation
        ${SB_ENV_CONFIGURE_CMD}
        ${GDAL_SB_SRC}/configure
        --prefix=${SB_INSTALL_PREFIX}
        --enable-static=no
        --without-ogdi
        --without-jasper
        --with-sentinel2
        --without-netcdf #netcdf is not added by superbuild. better to deactivate it.
        --without-hdf4
        --without-hdf5
        --without-xml2
        ${GDAL_SB_CONFIG}
        ${GDAL_SB_EXTRA_OPTIONS}
      BUILD_COMMAND $(MAKE)
      INSTALL_COMMAND $(MAKE) install
    )

  else(MSVC)
  ##add libkml
  ##https://trac.osgeo.org/gdal/ticket/5725
  ##is needed for SQLITE driver
    list(REMOVE_ITEM GDAL_DEPENDENCIES LIBKML)

    STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${SB_INSTALL_PREFIX})
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})
    configure_file(${CMAKE_SOURCE_DIR}/patches/GDAL/nmake_gdal_extra.opt.in ${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt)

    ExternalProject_Add(GDAL
       PREFIX GDAL
       URL "http://download.osgeo.org/gdal/1.11.2/gdal-1.11.2.tar.gz"
       URL_MD5 866a46f72b1feadd60310206439c1a76
       SOURCE_DIR ${GDAL_SB_SRC}
       BINARY_DIR ${GDAL_SB_SRC}
       INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
       DEPENDS ${GDAL_DEPENDENCIES}
       PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/GDAL/GNUmakefile ${GDAL_SB_SRC}/swig/python/GNUmakefile
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/patches/GDAL/S2_patch ${GDAL_SB_SRC}
       CONFIGURE_COMMAND  ${CMAKE_COMMAND} -E copy  ${CMAKE_SOURCE_DIR}/patches/GDAL/ogrsqlitevirtualogr.cpp
      ${GDAL_SB_SRC}/ogr/ogrsf_frmts/sqlite/ogrsqlitevirtualogr.cpp
       BUILD_COMMAND nmake /f ${GDAL_SB_SRC}/makefile.vc MSVC_VER=${MSVC_VERSION} EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt
       INSTALL_COMMAND nmake /f ${GDAL_SB_SRC}/makefile.vc devinstall MSVC_VER=${MSVC_VERSION} EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt
    )

  endif()

  set(_SB_GDAL_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_GDAL_LIBRARY ${SB_INSTALL_PREFIX}/lib/gdal_i.lib)
  elseif(UNIX)
    set(_SB_GDAL_LIBRARY ${SB_INSTALL_PREFIX}/lib/libgdal${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
