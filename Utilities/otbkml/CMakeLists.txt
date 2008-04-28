# Sources of non-templated classes.

PROJECT(libkml)

SET(libkml_VERSION_MAJOR "0")
SET(libkml_VERSION_MINOR "1")
SET(libkml_VERSION_PATCH "3")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
SET(libkml_VERSION_STRING "${libkml_VERSION_MAJOR}.${libkml_VERSION_MINOR}.${libkml_VERSION_PATCH}")


# Allow sources in subdirectories to see the include files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/kml)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib)

# Build libkml library
FILE(GLOB KML_DOM_SRCS "src/kml/dom/*.cc" )
FILE(GLOB KML_UTIL_SRCS "src/kml/util/*.cc" )

SET(KML_ZIP_SRCS 
third_party/zlib-1.2.3/contrib/minizip/ioapi.c
third_party/zlib-1.2.3/contrib/minizip/mztools.c
third_party/zlib-1.2.3/contrib/minizip/unzip.c
third_party/zlib-1.2.3/contrib/minizip/zip.c
)
# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX)
  FOREACH(f ${KML_DOM_SRCS} ${KML_UTIL_SRCS} ${KML_ZIP_SRCS} )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_LIBRARY(otbkml ${KML_DOM_SRCS} ${KML_UTIL_SRCS} ${KML_ZIP_SRCS})
IF( OTB_USE_EXTERNAL_EXPAT )
        TARGET_LINK_LIBRARIES(otbkml expat)
ELSE( OTB_USE_EXTERNAL_EXPAT  )
        TARGET_LINK_LIBRARIES(otbkml otbexpat)
ENDIF( OTB_USE_EXTERNAL_EXPAT )


INSTALL(TARGETS otbkml
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom/*.h")
FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/util/*.h")
FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/*.h")

INSTALL(FILES ${__files1} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/dom
    COMPONENT Development)
INSTALL(FILES ${__files2} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml/util
    COMPONENT Development)
INSTALL(FILES ${__files2} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/kml
    COMPONENT Development)

