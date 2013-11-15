project(ossimplugins)

set(ossimplugins_VERSION_MAJOR "1")
set(ossimplugins_VERSION_MINOR "8")
set(ossimplugins_VERSION_PATCH "4")

# svn revision number of ossim (used for .so versioning)
set(OSSIMPLUGINS_SVN_REVISION "18162")
# patch number for OTB modifications applied on top of OSSIM_SVN_REVISION (used for .so versioning)
set(OSSIMPLUGINS_OTB_PATCH "1")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/ossim
#  ${OTB_SOURCE_DIR}/Utilities/otbossim/include
#  ${OTB_BINARY_DIR}/Utilities/otbossim/include
)
if(OTB_USE_EXTERNAL_OSSIM)
  include_directories(${OSSIM_INCLUDE_DIR})

  # When using External ossim, compile ossimplugin as a DLL
  # even if we compile OTB as a static lib
  if(WIN32)
    set(ossimplugins_LIBTYPE SHARED)
    add_definitions(-DOSSIMPLUGINSMAKINGDLL)
  endif()

else()
  include_directories(${OTB_SOURCE_DIR}/Utilities/otbossim/include)
  include_directories(${OTB_BINARY_DIR}/Utilities/otbossim/include)
endif()

include_directories(AFTER ${GEOTIFF_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})

set(ossimplugins_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE ossimplugins_gdal_SRCS "${ossimplugins_SOURCES_DIR}/gdal/*.cpp")
file(GLOB_RECURSE ossimplugins_ossim_SRCS "${ossimplugins_SOURCES_DIR}/ossim/*.cpp")

set(ossimplugins_SOURCES
  ${ossimplugins_gdal_SRCS}
  ${ossimplugins_ossim_SRCS}
)

add_library(otbossimplugins ${ossimplugins_LIBTYPE} ${ossimplugins_SOURCES} )
target_link_libraries(otbossimplugins ${GEOTIFF_LIBRARY} ${GDAL_LIBRARY} ${OGR_LIBRARY} ${OSSIM_LIBRARIES})
set_target_properties(otbossimplugins
                      PROPERTIES VERSION "${OSSIMPLUGINS_SVN_REVISION}.${OSSIMPLUGINS_OTB_PATCH}.0"
                                 SOVERSION "${OSSIMPLUGINS_SVN_REVISION}.${OSSIMPLUGINS_OTB_PATCH}")

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbossimplugins
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()
