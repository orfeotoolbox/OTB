if( __EXTERNAL_GDAL__)
  return()
else()
  set(__EXTERNAL_GDAL__ 1)
endif()

if(USE_SYSTEM_GDAL)
  message(STATUS "  Using GDAL system version")
  return()
endif()

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

set(GDAL_PATCH_COMMAND)
set(GDAL_CONFIGURE_COMMAND)
set(GDAL_BUILD_COMMAND)
set(GDAL_INSTALL_COMMAND)

if(UNIX)
  set(GDAL_SB_EXTRA_OPTIONS "" CACHE STRING "Extra options to be passed to GDAL configure script")
  mark_as_advanced(GDAL_SB_EXTRA_OPTIONS)
  #Convert GDAL_SB_EXTRA_OPTIONS to a list to allow to add multiple instructions to the CONFIGURE_COMMAND
  separate_arguments(GDAL_SB_EXTRA_OPTIONS)

  #we dont do any framework build on osx. So let's be sure on case of gdal
  if(APPLE)
    list(APPEND GDAL_SB_CONFIG "--with-macosx-framework=no")
  endif()

  # PATCH_COMMAND ${CMAKE_COMMAND} -E touch ${GDAL_SB_SRC}/config.rpath
  # COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/GDAL/GNUmakefile ${GDAL_SB_SRC}/swig/python/GNUmakefile

  set(GDAL_PATCH_COMMAND)

  set(GDAL_CONFIGURE_COMMAND  "${SB_ENV_CONFIGURE_CMD};./configure"
    --prefix=${SB_INSTALL_PREFIX}
    --enable-static=no
    --enable-shared=yes
    --with-cfitsio=no
    --with-dods-root=no
    --with-dwgdirect=no
    --with-ecw=no
    --with-epsilon=no
    --with-fme=no
    --with-gif=no
    --with-grass=no
    --with-gta=no
    --with-hdf4=no
    --with-hdf5=no
    --with-idb=no
    --with-ingres=no
    --with-jp2mrsid=no
    --with-kakadu=no
    --with-libgrass=no
    --with-libjson-c=internal
    --with-mrsid=no
    --with-msg=no
    --with-mysql=no
    --with-netcdf=no
    --with-oci=no
    --with-odbc=no
    --with-ogdi=no
    --with-pam
    --with-pcidsk=no
    --with-pcraster=no
    --with-pcre=no
    --with-perl=no
    --with-php=no
    --with-python=no
    --with-qhull=internal
    --with-sde=no
    --with-spatialite=no
    --with-xerces=no
    --with-xml2=no
    ${GDAL_SB_CONFIG}
    ${GDAL_SB_EXTRA_OPTIONS}
    )
  set(GDAL_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM})
  set(GDAL_INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install)

else(MSVC)

  ##remove libkml?
  list(REMOVE_ITEM GDAL_DEPENDENCIES LIBKML)

  STRING(REGEX REPLACE "/$" "" CMAKE_WIN_INSTALL_PREFIX ${SB_INSTALL_PREFIX})
  STRING(REGEX REPLACE "/" "\\\\" CMAKE_WIN_INSTALL_PREFIX ${CMAKE_WIN_INSTALL_PREFIX})
  configure_file(${CMAKE_SOURCE_DIR}/patches/GDAL/nmake_gdal_extra.opt.in ${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt)
  set(GDAL_PATCH_COMMAND)
  set(GDAL_CONFIGURE_COMMAND)
  set(GDAL_BUILD_COMMAND nmake
    /f ${GDAL_SB_SRC}/makefile.vc
    MSVC_VER=${MSVC_VERSION}
    EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt
    )
  set(GDAL_INSTALL_COMMAND nmake
    /f ${GDAL_SB_SRC}/makefile.vc devinstall
    MSVC_VER=${MSVC_VERSION}
    EXT_NMAKE_OPT=${CMAKE_BINARY_DIR}/nmake_gdal_extra.opt
    )

endif()

#message(FATAL_ERROR "GDAL_CONFIGURE_COMMAND=${GDAL_CONFIGURE_COMMAND}")
ExternalProject_Add(GDAL
  PREFIX GDAL
  URL "http://download.osgeo.org/gdal/2.1.0/gdal-2.1.0.tar.gz"
  URL_MD5 0fc165cd947c54b132204233dfb243f1
  SOURCE_DIR ${GDAL_SB_SRC}
  BINARY_DIR ${GDAL_SB_SRC}
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${GDAL_DEPENDENCIES}
  PATCH_COMMAND ${GDAL_PATCH_COMMAND}
  CONFIGURE_COMMAND ${GDAL_CONFIGURE_COMMAND}
  BUILD_COMMAND ${GDAL_BUILD_COMMAND}
  INSTALL_COMMAND ${GDAL_INSTALL_COMMAND}
  )

#avoid/fixup  second arg "GDAL-custom-patch" in macro
SUPERBUILD_PATCH_SOURCE(GDAL "GDAL-custom-patch")

set(_SB_GDAL_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
if(WIN32)
  set(_SB_GDAL_LIBRARY ${SB_INSTALL_PREFIX}/lib/gdal_i.lib)
elseif(UNIX)
  set(_SB_GDAL_LIBRARY ${SB_INSTALL_PREFIX}/lib/libgdal${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()
