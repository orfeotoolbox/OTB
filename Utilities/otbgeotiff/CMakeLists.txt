PROJECT(libgeotiff)

FILE(GLOB libgeotiff_SRCS "*.c")
FILE(GLOB libgeotiff_HDRS "*.h")

CONFIGURE_FILE(geo_config.h.in ${OTB_BINARY_DIR}/Utilities/otbgeotiff/geo_config.h)

INCLUDE_DIRECTORIES(${OTB_BINARY_DIR}/Utilities/otbgeotiff/)

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
