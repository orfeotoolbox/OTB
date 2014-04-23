message(STATUS "Importing Jpeg2000...")
# Use the openjpeg library.
if(GDAL_HAS_J2K)
 set(USE_JPEG2000_DEFAULT OFF)
else()
 set(USE_JPEG2000_DEFAULT ON)
endif()

option(OTB_USE_JPEG2000 "Enable internal support for jpeg2000 image file format." ${USE_JPEG2000_DEFAULT})
mark_as_advanced(OTB_USE_JPEG2000)

cmake_dependent_option(OTB_USE_JPEG2000_TESTING "Use jpeg2000 images in tests." OFF
                       "NOT OTB_USE_JPEG2000;NOT GDAL_HAS_J2K" ON)
mark_as_advanced(OTB_USE_JPEG2000_TESTING)

if(OTB_USE_JPEG2000)
 message(STATUS "  Enabling Jpeg2000 support")

 set(OPENJPEG_SOURCE_DIR ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg)
 set(OPENJPEG_BINARY_DIR ${OTB_BINARY_DIR}/Utilities/otbopenjpeg)

 set(OPENJPEG_NAMESPACE "OTBOPENJPEG")
 set(OPJ_MANGLE_PREFIX "otbopenjpeg")
 set(OPENJPEG_INSTALL_LIB_DIR ${OTB_INSTALL_LIB_DIR})

  if(NOT BUILD_SHARED_LIBS)
        add_definitions(-DOPJ_STATIC)
  endif()

else()
  message(STATUS "  Disabling Jpeg2000 support")
endif()
