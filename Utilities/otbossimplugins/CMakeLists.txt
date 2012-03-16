PROJECT(ossimplugins)

SET(ossimplugins_VERSION_MAJOR "1")
SET(ossimplugins_VERSION_MINOR "8")
SET(ossimplugins_VERSION_PATCH "4")

# svn revision number of ossim (used for .so versioning)
SET(OSSIMPLUGINS_SVN_REVISION "18162")
# patch number for OTB modifications applied on top of OSSIM_SVN_REVISION (used for .so versioning)
SET(OSSIMPLUGINS_OTB_PATCH "1")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/ossim
#  ${OTB_SOURCE_DIR}/Utilities/otbossim/include
#  ${OTB_BINARY_DIR}/Utilities/otbossim/include
)
IF(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES(${OSSIM_INCLUDE_DIR})
  
  # When using External ossim, compile ossimplugin as a DLL
  # even if we compile OTB as a static lib
  IF (WIN32)
    SET(ossimplugins_LIBTYPE SHARED)
    ADD_DEFINITIONS(-DOSSIMPLUGINSMAKINGDLL)
  ENDIF (WIN32)
  
ELSE(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Utilities/otbossim/include)
  INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbossim/include)
ENDIF(OTB_USE_EXTERNAL_OSSIM)

INCLUDE_DIRECTORIES(AFTER ${GEOTIFF_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})

SET(ossimplugins_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB_RECURSE ossimplugins_gdal_SRCS "${ossimplugins_SOURCES_DIR}/gdal/*.cpp")
FILE(GLOB_RECURSE ossimplugins_ossim_SRCS "${ossimplugins_SOURCES_DIR}/ossim/*.cpp")

SET(ossimplugins_SOURCES
  ${ossimplugins_gdal_SRCS}
  ${ossimplugins_ossim_SRCS}
)

ADD_LIBRARY(otbossimplugins ${ossimplugins_LIBTYPE} ${ossimplugins_SOURCES} )
TARGET_LINK_LIBRARIES(otbossimplugins ${GEOTIFF_LIBRARY} ${GDAL_LIBRARY} ${OGR_LIBRARY} ${OSSIM_LIBRARIES})
SET_TARGET_PROPERTIES(otbossimplugins
                      PROPERTIES VERSION "${OSSIMPLUGINS_SVN_REVISION}.${OSSIMPLUGINS_OTB_PATCH}.0"
                                 SOVERSION "${OSSIMPLUGINS_SVN_REVISION}.${OSSIMPLUGINS_OTB_PATCH}")

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbossimplugins
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)
