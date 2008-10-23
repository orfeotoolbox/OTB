# Sources of non-templated classes.

PROJECT(OTBGALIB)

SET(OTBGALIB_VERSION_MAJOR "2")
SET(OTBGALIB_VERSION_MINOR "4")
SET(OTBGALIB_VERSION_PATCH "7")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
SET(OTBGALIB_VERSION_STRING "${OTBGALIB_VERSION_MAJOR}.${OTBGALIB_VERSION_MINOR}.${OTBGALIB_VERSION_PATCH}")


# Allow sources in subdirectories to see the include files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Build otbgalib library
FILE(GLOB GALIB_SRCS "ga/*.C" )

# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f ${GALIB_SRCS} )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_LIBRARY(otbgalib ${GALIB_SRCS})
TARGET_LINK_LIBRARIES (otbgalib)


INSTALL(TARGETS otbgalib
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/ga/*.h")

INSTALL(FILES ${__files} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbgalib/ga
    COMPONENT Development)

