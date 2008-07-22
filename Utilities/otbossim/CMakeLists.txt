PROJECT(otbossim)

INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE("dirent.h" CMAKE_HAVE_DIRENT_H)
CHECK_INCLUDE_FILE("unistd.h" CMAKE_HAVE_UNISTD_H)
CHECK_INCLUDE_FILE("getopt.h" CMAKE_HAVE_GETOPT_H)
CHECK_INCLUDE_FILE("fcntl.h" CMAKE_HAVE_FCNTL_H)
CHECK_INCLUDE_FILE("dbmalloc.h" CMAKE_HAVE_DBMALLOC_H)
CHECK_INCLUDE_FILE("malloc.h" CMAKE_HAVE_MALLOC_H)
CHECK_INCLUDE_FILE("dlfcn.h" CMAKE_HAVE_DLFCN_H)

# Cf OSSIM configurations : "nmake.opt" for Visual and "configure" for MinGW (and normaly Cygwin)
IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      # For MinGW and Visual platforms
      IF(BUILD_SHARED_LIBS)
        ADD_DEFINITIONS(-DOSSIMMAKINGDLL)
      ENDIF(BUILD_SHARED_LIBS)
      # For Visual
      IF(NOT MINGW)  
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /nologo /EHsc ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /nologo /EHsc ")
      ENDIF(NOT MINGW)  
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)


# itkjpeg headers are not generated and put into the include dir automatically. 
# Therefore we have to do it ourselves.

IF(OTB_USE_EXTERNAL_ITK)
CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/.NoDartCoverage
               ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/.NoDartCoverage)
CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/jpegDllConfig.h.in
               ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/jpegDllConfig.h)
FOREACH(bits 8 12 16)
SET(CMAKE_BITS_IN_JSAMPLE ${bits})
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/jmorecfg.h.in
                 ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/${bits}/jmorecfg.h @ONLY
IMMEDIATE)
SET(BITS_TYPE_JPEG_MANGLE_NAME "itkjpeg/mangle_jpeg${bits}bits.h")
CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/jconfig.h.in
                 ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/${bits}/jconfig.h
@ONLY
IMMEDIATE)
CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/jpeglib.h
                 ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/${bits}/jpeglib.h
@ONLY
IMMEDIATE)
CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/jerror.h
                 ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/${bits}/jerror.h
@ONLY
IMMEDIATE)
CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkjpeg/jinclude.h
                 ${OTB_BINARY_DIR}/Utilities/ITK/Utilities/itkjpeg/${bits}/jinclude.h
@ONLY
IMMEDIATE)
ENDFOREACH(bits)
INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities)
INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/ITK/Utilities)
ENDIF(OTB_USE_EXTERNAL_ITK)

CONFIGURE_FILE(${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h.in
	       ${OTB_BINARY_DIR}/Utilities/otbossim/include/ossim/ossimConfig.h)

SUBDIRS(include)
# ADDED BY OTB DEVELOPPERS TO AVOID INTENSIVE RECOMPILATION
IF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
SUBDIRS(src)
ENDIF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
