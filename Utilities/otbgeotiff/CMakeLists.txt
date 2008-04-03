PROJECT(libgeotiff)

FILE(GLOB libgeotiff_SRCS "*.c")
FILE(GLOB libgeotiff_HDRS "*.h")

CONFIGURE_FILE(geo_config.h.in ${OTB_BINARY_DIR}/Utilities/otbgeotiff/geo_config.h)

INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbgeotiff/)

# Adjust the compiler flags to avoid problems with geotiff code.
IF(CMAKE_COMPILER_IS_GNUCC)
  # Hide warnings in geotiff code.
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

  # CMake versions before 2.4 did not put the source-file-specific flags
  # after the directory-level flags.  Remove optimization flags from the
  # CMAKE_C_FLAGS* variables so they can be overridden.
  IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)
    FOREACH(c "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
      STRING(REGEX REPLACE
        "-O[^ ]*( |$)" "" CMAKE_C_FLAGS${c} "${CMAKE_C_FLAGS${c}}")
    ENDFOREACH(c)
  ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.4)

ENDIF(CMAKE_COMPILER_IS_GNUCC)

ADD_LIBRARY(otbgeotiff ${libgeotiff_SRCS})
TARGET_LINK_LIBRARIES(otbgeotiff)

INSTALL(TARGETS otbgeotiff
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

INSTALL(FILES ${libgeotiff_HDRS_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbgeotiff
    COMPONENT Development)

INSTALL(FILES ${OTB_BINARY_DIR}/Utilities/otbgeotiff/geo_config.h
	      DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbgeotiff
	      COMPONENT Development)

SUBDIRS(libxtiff)
