message(STATUS "Importing Jpeg2000...")
# Use the openjpeg library.
option(OTB_USE_JPEG2000 "Use to support jpeg2000 image file format." ON)
mark_as_advanced(OTB_USE_JPEG2000)

if(OTB_USE_JPEG2000)
 message(STATUS "  Enabling Jpeg2000 support")

 set(OPENJPEG_SOURCE_DIR ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg)
 set(OPENJPEG_BINARY_DIR ${OTB_BINARY_DIR}/Utilities/otbopenjpeg)

 set(OPENJPEG_NAMESPACE "OTBOPENJPEG")
 set(OPJ_MANGLE_PREFIX "otbopenjpeg")
 set(OPENJPEG_INSTALL_LIB_DIR ${OTB_INSTALL_LIB_DIR})

  if(NOT BUILD_SHARED_LIBS)
        add_definitions(-DOPJ_STATIC)
  endif(NOT BUILD_SHARED_LIBS)

else( OTB_USE_JPEG2000 )
  message(STATUS "  Disabling Jpeg2000 support")
endif(OTB_USE_JPEG2000)
