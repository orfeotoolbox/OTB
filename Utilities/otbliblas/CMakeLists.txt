PROJECT(liblas)

SET(liblas_VERSION_MAJOR "1")
SET(liblas_VERSION_MINOR "0")
SET(liblas_VERSION_PATCH "0")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(liblas_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(liblas_SOURCES 
  ${liblas_SOURCES_DIR}/las_c_api.cpp
  ${liblas_SOURCES_DIR}/lasfile.cpp
  ${liblas_SOURCES_DIR}/laspoint.cpp
  ${liblas_SOURCES_DIR}/lasrecordheader.cpp
  ${liblas_SOURCES_DIR}/laserror.cpp
  ${liblas_SOURCES_DIR}/lasheader.cpp
  ${liblas_SOURCES_DIR}/lasreader.cpp
  ${liblas_SOURCES_DIR}/laswriter.cpp
  ${liblas_SOURCES_DIR}/detail/file.cpp
  ${liblas_SOURCES_DIR}/detail/reader10.cpp
  ${liblas_SOURCES_DIR}/detail/reader11.cpp
  ${liblas_SOURCES_DIR}/detail/reader.cpp
  ${liblas_SOURCES_DIR}/detail/writer10.cpp
  ${liblas_SOURCES_DIR}/detail/writer11.cpp
  ${liblas_SOURCES_DIR}/detail/writer.cpp
)

ADD_LIBRARY(otbliblas ${liblas_SOURCES} )

INSTALL(TARGETS otbliblas
RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)

FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/*.hpp")
INSTALL(FILES ${__files1} 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas
    COMPONENT Development)

